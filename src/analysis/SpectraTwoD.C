//----------------------------------------------------------------------
// MyAnalysis.C
//
// Description: Post-processing for Trojan Horse Measurements
//
// Version 0.1, 7/22/08
// Changes: None
//----------------------------------------------------------------------

//#include "/home/unc/longland/geant4/dictionary/libBC523ADICT.so"

// #if defined(__CINT__) && !defined(__MAKECINT__)
// {
//    gSystem->CompileMacro("MyAnalysis.C", "k");
//    MyAnalysis();
// }
// #else


#include <iostream>
#include <fstream>
#include <sstream>
#include "TFile.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TRandom3.h"
#include "TMath.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TCut.h"
#include "TCutG.h"

using namespace TMath;

// Beam energy
Double_t Ea = 20000.;  // keV

// Masses in AMU
Double_t ma = 7.01600;
Double_t mA = 2.01410;
Double_t mb = 1.00866;
Double_t mB1 = 4.00260;
Double_t mB2 = 4.00260;
Double_t mx = 1.00783;

// Distance from target to detector
Double_t SourceDetector = 130.; // mm

// The detector angles
Double_t Th1 = 45.*Pi()/180.;
Double_t Th2 = 45.*Pi()/180.;

// Collimator sizes
Double_t span[2][4];

// Detector resolution
Double_t sig1 = 15.;
Double_t sig2 = 30.;

Double_t c2 = 931.494028;  // MeV/c2

// Functions
Double_t calcQ(Double_t,Double_t);
Double_t calcEb(Double_t,Double_t);
Double_t calcEp(Double_t,Double_t);

// Structure for an event
class Event{
public:
  Double_t EB1;
  Double_t EB2;
  Double_t ThB1;
  Double_t ThB2;
  Double_t Ep;
  Double_t Q;
};

void SpectraTwoD(){

  // Detector 1 (the "good" detector) is [0]
  span[0][0] = atan(0.25/SourceDetector); // 0.5mm collimator
  span[0][1] = atan(0.55/SourceDetector); // 1.1mm
  span[0][2] = atan(1.0/SourceDetector); // 2mm
  span[0][3] = atan(1.5/SourceDetector); // 3mm
  // Detector 2 is [1]
  span[1][0] = atan(0.5/SourceDetector); // 1mm collimator
  span[1][1] = atan(1.0/SourceDetector); // 2mm
  span[1][2] = atan(1.5/SourceDetector); // 3mm
  span[1][3] = atan(2.0/SourceDetector); // 4mm

  Int_t nhts;
  Double_t x,y,z,d;

  // Get Raw Geant4 data
  TFile *f = new TFile("THM.root"); 
  TTree *fTree = (TTree*)f->Get("fTree");

  // New tree to store data. This will keep stuff from span[][1] pair
  // for later analysis
  Event *e = new Event;
  TTree *aTree = new TTree("aTree","Newly Created tree for data analysis");
  aTree->SetDirectory(0);
  aTree->Branch("event",&e);

  // Create the plotting canvases
  TCanvas* Ecanvas = new TCanvas("Energy canvas","E2 vs E1");
  TCanvas* Qcanvas = new TCanvas("Q-value canvas","Q");
  TCanvas* Epcanvas = new TCanvas("Ep canvas","Ep");
  Ecanvas->Divide(2,2);
  Qcanvas->Divide(2,2);
  Epcanvas->Divide(2,2);


  // Get all of the leaves (not branches?) from the root file. This is
  // a little hacky, but seems to work. Make sure the leaf names
  // correspond with those output by geant4
  TLeaf *fnhits = fTree->GetLeaf("fhits");   
  TLeaf *fKE = fTree->GetLeaf("fKE");
  TLeaf *fLocalPosy = fTree->GetLeaf("fLocalPosy");
  TLeaf *fLocalPosx = fTree->GetLeaf("fLocalPosx");
  TLeaf *fLocalPosz = fTree->GetLeaf("fLocalPosz");
  TLeaf *fHitE = fTree->GetLeaf("fhitEnergy");

  // The number of events in the geant file
  Int_t nentries = fTree->GetEntries();

  // Construct all of the histograms for each pair of detector
  // collimators
  TH2F *E2_vs_E1[4];
  TH1F *Qh[4];
  TH1F *Eph[4];
  for (Int_t coll=0; coll<4; coll++){
    cout << "Collimator pair: " << coll << endl;
    cout << "With detector spans of " << span[0][coll]*180./Pi() 
	 <<  " and " << span[1][coll]*180./Pi() << " degrees" << endl;
    cout << "(i.e., " << 2.*SourceDetector*tan(span[0][coll]) << " mm and " 
	 << 2.*SourceDetector*tan(span[1][coll]) << "mm)" << endl << endl;

    // names of histograms must be different because root is a dick
    char *histname = new char[15];
    sprintf(histname, "Energies %d",coll); 
    // 2D Ea1 vs Ea2 
    E2_vs_E1[coll] = new TH2F(histname,"E2 vs E1",
			      1024,0.,25000.,
			      1024,0.,25000.);

    // Q-value histogram
    sprintf(histname, "Q-values %d",coll);
    Qh[coll] = new TH1F(histname,"Q (keV)",1024,0,25000);

    // Corresponding proton energy histogram
    sprintf(histname, "Ep %d",coll);
    Eph[coll] = new TH1F(histname,"E (keV)",1024,0,1000);    
  }

  Int_t hitcount[4] = {0};

  // No go through the Geant4 file event by event, so some analysis on
  // the raw data, and fill the event tree with more useful
  // information
  cout << "Sorting events..." << endl;
  for (Int_t i=0;i<nentries;i++) {
    if((i % 100000) == 0.) 
      cout << " Event Number " << i << endl;
    
    // Get this event
    fTree->GetEntry(i); 
    nhts = fnhits->GetValue(0);
    
    // Just use the first hit in the detctor to determine its position
    // (angle). 
    Double_t ke,ke1,ke2,hite,edep;

    // First particle position and angle
    y = fLocalPosy->GetValue(0);
    z = fLocalPosz->GetValue(0);
    x = fLocalPosx->GetValue(0);
    d = sqrt(pow(x,2) + pow(y,2) + pow(z,2));
    Double_t ThB1 = asin(y/d);

    // step through this particle until it stops to get its energy
    // (sum of energy deposited in the silicon), then look at the next
    // one
    edep = 0.; 
    Int_t j;
    for(j=0; j<nhts; j++){
      ke = fKE->GetValue(j);
      hite = fHitE->GetValue(j);
      edep += hite;
      if(ke==0.)break;
    }
    ke1 = edep;

    // Second particle
    y = fLocalPosy->GetValue(j+1);
    z = fLocalPosz->GetValue(j+1);
    x = fLocalPosx->GetValue(j+1);
    d = sqrt(pow(x,2) + pow(y,2) + pow(z,2));
    Double_t ThB2 = asin(y/d);
    // Make sure that this particle actually hit the detector
    if(fabs(d-SourceDetector) > 10.)ThB2=0.;
    
    edep = 0.; 
    for(Int_t k=j+1;k<nhts;k++){
      ke = fKE->GetValue(k);
      hite = fHitE->GetValue(k);
      edep += hite;
      if(ke==0.)break;
    }
    ke2 = edep;

    // Re-assign the particles according to which detector they
    // hit. So "alpha1" becomes whichever particle that hit detector
    // 1. Detector 1 is at +45 degrees, and detector 2 is at -45
    // degrees
    if(ThB1 < 0.){
      Double_t Thtmp = ThB2;
      Double_t ketmp = ke2;
      ThB2 = ThB1;
      ke2 = ke1;
      ThB1 = Thtmp;
      ke1 = ketmp;
    }

    Bool_t goodcount;
    // fill the histograms by going through the collimators in
    // decreasing size order, this should save time
    goodcount = 
      ((fabs(fabs(ThB1)-Th1) < span[0][3]) & 
       (fabs(fabs(ThB2)-Th2) < span[1][3]));
    if(goodcount){
      hitcount[3]++;

      // Convolute the detector energies with detector resolution
      Double_t EB1 = gRandom->Gaus(ke1,sig1);
      Double_t EB2 = gRandom->Gaus(ke2,sig2);

      // Fill the event tree with anything that made it through the
      // first cut. So solid angle cuts must be made on that data
      // later on when it is re-processed
      e->EB1 = EB1;
      e->EB2 = EB2;
      e->ThB1 = ThB1;
      e->ThB2 = ThB2;
      // Calculate the Q-value measured for this hit
      Double_t Q = calcQ(EB1,EB2);
      e->Q=Q;
      Double_t Ep = calcEp(EB1,EB2);
      e->Ep=Ep;

      // Fill the event tree
      aTree->Fill();

      E2_vs_E1[3]->Fill(EB1,EB2);
      Qh[3]->Fill(Q);
      Eph[3]->Fill(Ep);

      // Now see if it's in the next smaller collimator pair
      goodcount = 
	((fabs(fabs(ThB1)-Th1) < span[0][2]) & 
	 (fabs(fabs(ThB2)-Th2) < span[1][2]));
      if(goodcount){
	hitcount[2]++;
	E2_vs_E1[2]->Fill(EB1,EB2);
	Qh[2]->Fill(Q);
	Eph[2]->Fill(Ep);

	// Next smaller pair
	goodcount = 
	  ((fabs(fabs(ThB1)-Th1) < span[0][1]) & 
	   (fabs(fabs(ThB2)-Th2) < span[1][1]));
	if(goodcount){
	  hitcount[1]++;
	  E2_vs_E1[1]->Fill(EB1,EB2);
	  Qh[1]->Fill(Q);
	  Eph[1]->Fill(Ep);

	  // Final collimator pair - the smallest ones
	  goodcount = 
	    ((fabs(fabs(ThB1)-Th1) < span[0][0]) & 
	     (fabs(fabs(ThB2)-Th2) < span[1][0]));
	  if(goodcount){
	    hitcount[0]++;
	    E2_vs_E1[0]->Fill(EB1,EB2);
	    Qh[0]->Fill(Q);
	    Eph[0]->Fill(Ep);
	  }
	}
      }
    }

  }

  // Copy the event tree to a file
  TFile ff("EventData.root","recreate");
  TTree *TC = (TTree*)aTree->CloneTree();
  TC->Write();
  ff.Close();

  // Finally, draw the plots
  cout << endl;
  for(Int_t coll=0;coll<4;coll++){
    cout << "Collimator pair: " << coll << ". Percentage counted = " << 
      100.*(Double_t)hitcount[coll]/(Double_t)nentries << endl;

    Ecanvas->cd(coll+1);
    E2_vs_E1[coll]->Draw("colz");

    Qcanvas->cd(coll+1);
    Qh[coll]->Draw();

    Epcanvas->cd(coll+1);
    Eph[coll]->Draw();
  }

  // See if a 2D cut file exists
  FileStat_t fstat;
  if(gSystem->GetPathInfo("Cut2D.root",fstat) == 0){
    cout << "A cut file: Cut2D.root, exists." << endl;
  } else {
    cout << "No 2D cut file exists. If you want to make one," 
	 << "\n please do so and save it as Cut2D.root" << endl;
  }

  // Ask user to make a new 2D cut if they want
  cout << "Do you want to make a new gate? y/n" << endl;
  string ans;
  cin >> ans;
  if(ans == "y" || ans == "yes"){
    cout << "Click on Graphical cut" << endl;
    Ecanvas->Update();
    TCutG *cut = new TCutG();
    cut = (TCutG*) gPad->WaitPrimitive("CUTG");
    cut->SetName("Cut2D"); 
    TFile *fcut = new TFile("Cut2D.root","RECREATE");
    cut->Write();
    fcut->Close();
  }

}

//------------------------------------------------------------------
// Routine to take the event tree that was created in the main routine
// above and re-process it as if it were recorded data.
void reAnalyse(){

  // There are two sets of geant4 files. The usual ones include the
  // solid angle effects of the detectors, while the other one was
  // generated by only emitting alphas at +-45 degrees
  TFile *f = new TFile("EventData.root");
  TFile *f_ea = new TFile("EventData-ExactAngles.root"); 
  TTree *aTree = (TTree*)f->Get("aTree");
  TTree *aTree_ea = (TTree*)f_ea->Get("aTree");
  
  // Each branch is contained in the Event class
  Event *nt_event;
  Event *nt_event_ea;
  aTree->SetBranchAddress("event",&nt_event);
  aTree_ea->SetBranchAddress("event",&nt_event_ea);

  // If it exists, open the 2D cut (a TCutG type cut)
  TCutG *Cut2D=0;
  FileStat_t fstat;
  if(gSystem->GetPathInfo("Cut2D.root",fstat) == 0){
    TFile *cutg = new TFile("Cut2D.root");
    Cut2D = (TCutG*)cutg->Get("CUTG");
    // Set its name for later use. What a pain in the arse.
    Cut2D->SetName("Cut2D");
    Cut2D->SetVarX("EB1");
    Cut2D->SetVarY("EB2");
    cutg->Close();
  }

  // Now make some angle cuts. Because root is a little bitch, these
  // will have to be re-assigned later becuase we need to do some
  // boolean operations on them. So if you change these angles, make
  // sure you change them everywhere. Why the hell isn't "IsInside"
  // implemented for TCuts?
  TCut angle1 = Form("abs(abs(ThB1)-%f)<%f",Th1,atan(0.55/130.));
  TCut angle2 = Form("abs(abs(ThB2)-%f)<%f",Th2,atan(1.0/130.));
  TCut angle = angle1&&angle2;

  // The final cut including 2D cut anf angle cuts
  TCut finalcut;
  if(Cut2D){
    finalcut = Form("(Cut2D)&&(%s)",angle.GetTitle());
  } else {
    finalcut = angle;
  }

  // Draw the Ea1 vs Ea2 plot
  TCanvas* Ecanvas = new TCanvas("Energy canvas","E2 vs E1");
  Ecanvas->Divide(2);

  // The data including solid angle effects
  Ecanvas->cd(1);
  TH2D *h = new TH2D("h","Solid angle included",1024,0,25000,1024,0,25000);
  aTree->Draw("EB1:EB2>>h",finalcut,"colz");
  Cut2D->Draw("same");
  h->Print();

  // No solid angle effects
  Ecanvas->cd(2);
  TH2D *h_ea = new TH2D("h_ea","Point detectors",1024,0,25000,1024,0,25000);
  aTree_ea->Draw("EB1:EB2>>h_ea",finalcut,"colz");
  Cut2D->Draw("same");
  h_ea->Print();
  
  // Make the corresponding proton energy plots
  TCanvas* Epcanvas = new TCanvas("Ep canvas","Ep");
  Epcanvas->Divide(2);
  TH1F *hEp = new TH1F("Ep Values","Ep (keV)",1024,0,1000);
  TH1F *hEp_cut = new TH1F("Ep Values (cut)","cut Ep (keV)",1024,0,1000);
  TH1F *hEp_ea = new TH1F("Ep Values ExactAngle",
			  "Point detectors Ep (keV)",1024,0,1000);
  TH1F *hEp_cut_ea = new TH1F("Ep Values ExactAngle (cut)",
			      "cut point detectors Ep (keV)",1024,0,1000);
  
  // Try processing the data. Go through event by event and analyse it
  // how we would analyse the experimental data. Remember, that we
  // need to cut these events to make sure they correspond with the
  // collimator sizes.
  Int_t nentries = aTree->GetEntries();
  for(Int_t evt=0; evt<nentries; evt++){
    aTree->GetEntry(evt);
    aTree_ea->GetEntry(evt);

    // Get the data out of the event
    Double_t EB1=nt_event->EB1;
    Double_t EB2=nt_event->EB2;
    Double_t ThB1=nt_event->ThB1;
    Double_t ThB2=nt_event->ThB2;
    Double_t EB1_ea=nt_event_ea->EB1;
    Double_t EB2_ea=nt_event_ea->EB2;
    Double_t ThB1_ea=nt_event_ea->ThB1;
    Double_t ThB2_ea=nt_event_ea->ThB2;

    // Claculate the proton energy from the alpha energies
    Double_t Ep;
    Double_t Ep_ea;
    Ep = calcEp(EB1,EB2);
    Ep_ea = calcEp(EB1_ea,EB2_ea);

    // Make the angle and 2D cuts
    Bool_t good2D = Cut2D->IsInside(EB1,EB2);
    Bool_t goodAngle = abs(abs(ThB1)-Th1)<atan(1.0/130.) &&
      abs(abs(ThB2)-Th2)<atan(1.0/130.);
    Bool_t goodEvt = good2D && goodAngle;
    
    Bool_t good2D_ea = Cut2D->IsInside(EB1_ea,EB2_ea);
    Bool_t goodAngle_ea = abs(abs(ThB1_ea)-Th1)<atan(1.0/130.) &&
      abs(abs(ThB2_ea)-Th2)<atan(1.0/130.);
    Bool_t goodEvt_ea = good2D_ea && goodAngle_ea;

    // If they are good events, fill the histograms. Only make solid
    // angle cuts on the "uncut" histograms
    if(goodAngle)
      hEp->Fill(Ep);
    if(goodAngle_ea)
      hEp_ea->Fill(Ep_ea);

    if(goodEvt){
      hEp_cut->Fill(Ep);
    }
    if(goodEvt_ea){
      hEp_cut_ea->Fill(Ep_ea);
    }
  }

  // Now draw everything
  Epcanvas->cd(1);
  hEp_cut->Draw();
  hEp->SetLineColor(2);
  hEp->Draw("same");


  Epcanvas->cd(2);
  hEp_cut_ea->Draw();
  hEp_ea->SetLineColor(2);
  hEp_ea->Draw("same");

  f->Close();
  f_ea->Close();

}

//----------------------------------------------------------------------
// Calculate the Q value for a particular pair of energies
Double_t calcQ(Double_t EB1, Double_t EB2){

  Double_t Q,Eb;

  Eb = calcEb(EB1,EB2);
  Q = EB1 + EB2 + Eb - Ea;

  return Q;
}

//----------------------------------------------------------------------
// Calculate the spectator momentum
Double_t calcEb(Double_t EB1, Double_t EB2){

  Double_t Eb;

  Eb = (Ea*ma + EB1*mB1 + EB2*mB2 - 
	2.*(std::sqrt(ma*Ea*mB1*EB1)*cos(Th1) + 
	    std::sqrt(ma*Ea*mB2*EB2)*cos(Th2)))/mb;

  return Eb;
}

//----------------------------------------------------------------------
// Calculate the proton energy that corresponds to this pair of alpha
// energies
Double_t calcEp(Double_t EB1, Double_t EB2){

  Double_t Ep,Q;

  Q = ((mx + ma) - (mB1 + mB2))*c2;

  Ep = (1./(mB1*mB2*(mB1+mB2)))*(mB1*pow(mB2,2)*EB1 + mB2*pow(mB1,2)*EB2 - 
	      2.*mB1*mB2*sqrt(mB1*mB2*EB1*EB2)*cos(Th1-Th2)) - Q;

  return Ep;
}
