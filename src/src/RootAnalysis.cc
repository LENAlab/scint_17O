//----------------------------------------------------------------------
// RootAnalysis.cc
//
// Description: Sorts out the root analysis
//
// Version 0.1, 8/7/06
// Changes: Nothing!
//----------------------------------------------------------------------
//#ifdef G4ANALYSIS_USE

#include <sstream>
#include <fstream>

#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>

#include "globals.hh"
#include "G4Event.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"

#include "TTree.h"

#include "RootAnalysis.hh"
#include "RootOutputData.hh"
#include "RunAction.hh"
#include "AnalysisManager.hh"

//---------------------------------------------------------------------------------//
RootAnalysis::RootAnalysis(){
  fT = new RootOutputData();

  // Set up the hit collections
  targetCollID = -1;
}

//---------------------------------------------------------------------------------//

RootAnalysis::~RootAnalysis()
{ 
  fT->DeAllocateArrays();
}

//---------------------------------------------------------------------------------//

void RootAnalysis::BeginOfRunAction(){
  //Root init stuff
  //  fT->AllocateArrays();
  OpenFile();
  DefineSchema();
}

//---------------------------------------------------------------------------------//

void RootAnalysis::EndOfRunAction(){
  CloseFile();
}

//----------------------------------------------------------------------------------//

void RootAnalysis::BeginOfEventAction(const G4Event *evt){
  // Zero stuff and get ready...
  fT->fEventID = evt->GetEventID();
  fStepIndex = fT->fTotalNumberOfSteps = 0;
  fT->fEnergy = 0;
  fT->fParticleNamePos=0;

  // Get ID for Target hits collection
  if (targetCollID==-1){
    G4SDManager *SDman = G4SDManager::GetSDMpointer();
    targetCollID = SDman->GetCollectionID("targetCollection");
  }
}

//-----------------------------------------------------------------------------------//

void RootAnalysis::EndOfEventAction(const G4Event *evt){
  // Output all stepping info and total event stuff here.
  // Use arrays for steps. This way, can have total even info and
  // step stuff all in one tuple

  // All quantities that are saved to the TTree are preceded by an "f"
  // The event number is fEventID
  if(targetCollID<0) return;

  //Address the Hit Collections
  TargetHitsCollection* THC = NULL;
  G4HCofThisEvent * HCE = evt->GetHCofThisEvent();
  if(HCE){
    THC = (TargetHitsCollection*)(HCE->GetHC(targetCollID));
  }

  // Get the step and track info and write it to the Root pointers
  hits=0;
  if(THC){
    fT->fhits = THC->entries();
    //    G4cout << fT->fhits << " hits" << G4endl;
    fT->fEnergy = 0.;
    // Create arrays with a size big enough for the number of steps
    fT->AllocateArrays(fT->fhits);
    // Stuff to write for each step
    for(G4int i=0;i<fT->fhits;i++){
      TargetHit* aHit = (*THC)[i];
      fT->fhitEnergy[i] = aHit->GetEdep() / keV;
      //   G4cout << fT->fhitEnergy[i] << " hit energy" << G4endl;
      fT-> fEnergy += fT->fhitEnergy[i];
      fT->fLocalPosx[i] = aHit->GetLocalPos().x() / mm;
      //    G4cout << fT->fLocalPosx[i] << " pos x" << G4endl;
      fT->fLocalPosy[i] = aHit->GetLocalPos().y() / mm;
      fT->fLocalPosz[i] = aHit->GetLocalPos().z() / mm;
      fT->fTime[i] = aHit->GetTime() / ns;
      //      fT->fGePartID[i] = aHit->GetPartID();
    /** Ions are not included in the PDG numbering scheme and are assigned
        a PDG encoding of 0 by Geant 4.
        All particles in the PDG scheme have an encoding < 10^8
        Define an ion's encoding as:
        10^8 + 1000 * A + Z
    */
    fT->fPartID[i] = 
      G4ToRoot(aHit->GetPartID()->GetPDGEncoding());
    if(fT->fPartID[i] == 0) 
      if(aHit->GetPartID()->GetParticleType() == "nucleus") 
	fT->fPartID[i] = 
	  G4ToRoot(100000000 + 
		   aHit->GetPartID()->GetBaryonNumber() * 1000 +
		   (G4int)(aHit->GetPartID()->GetPDGCharge() / eplus));

      // Particle name (not working well at the moment
      ParticleName=aHit->GetParticleName();
      G4int sizeName = ParticleName.size();
      strcpy(&fT->fParticleName[fT->fParticleNamePos],
	     ParticleName.c_str());
      fT->fParticleNamePos += sizeName + 1; // Add 1 for '\0'

      fT->fParentID[i] = aHit->GetParentID();
      fT->fKE[i] = aHit->GetKE() / keV;
      fT->fDirectionx[i] = aHit->GetDirection().x();
      fT->fDirectiony[i] = aHit->GetDirection().y();
      fT->fDirectionz[i] = aHit->GetDirection().z();
      fT->fCreationPtx[i] = aHit->GetCreationPt().x() / mm;
      fT->fCreationPty[i] = aHit->GetCreationPt().y() / mm;
      fT->fCreationPtz[i] = aHit->GetCreationPt().z() / mm;
      fT->fCopyNo[i] = aHit->GetCopyNo();
      // Info for the first hit in the detector
       if (i==0){
	 fT->ffirstPartID = fT->fPartID[i];
	 fT->ffirstParentID = fT->fParentID[i];
	 fT->ffirstTime = fT->fTime[i];
	 fT->ffirstCreationPtx = fT->fCreationPtx[i];
	 fT->ffirstCreationPty = fT->fCreationPty[i];
	 fT->ffirstCreationPtz = fT->fCreationPtz[i];
       }
    }

  }

  // If there are any hits, write the tuple
  if(fT->fhits>0 ){
    FillTree();
  }
  // Delete the arrays. Reallocate them at the end of the next event
  fT->DeAllocateArrays();
}

//-----------------------------------------------------------------------------------//

void RootAnalysis::DefineSchema(){
  TTree *nT = 0;
  
  // Create output tree if it hasn't been defined
  if(fTree){
    nT = fTree;
  } else {
    nT = fTree = new TTree("fTree","Generic Step output Tree");
    
    // Start a root branch
    nT->Branch("RawMC","RootOutputData",&fT,32000,2);    
  }
}

//---------------------------------------------------------------------------------------//
//#endif // G4ANALYSIS_USE
