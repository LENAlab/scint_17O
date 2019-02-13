//--------------------------------------------------------------------
// RunAction.cc
//
// Description: Defines the run itself.
// Changes: 7/18/05 None yet
//--------------------------------------------------------------------

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <time.h>

#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RootAnalysis.hh"
#include "EventAction.hh"
#include "G4GeneralParticleSource.hh"
#include "G4UIcommand.hh"

//#ifdef G4ANALYSIS_USE
//#include <AIDA/AIDA.h>
#include "AnalysisManager.hh"
//#endif

RunAction::RunAction(PrimaryGeneratorAction* gen, RootAnalysis* root)
  : genAction(gen),rootanalysis(root)
{ }

RunAction::~RunAction()
{}

void RunAction::BeginOfRunAction(const G4Run* run)
{
  G4long seed=time(0);
  CLHEP::HepRandom::setTheSeed(seed);

  // Start the Analysis
  //#ifdef G4ANALYSIS_USE
  // If we're creating an ascii file, allocate array, if not, call root
  if(AnalysisManager::getInstance()->GetasciiFileState()){
    nEvents=run->GetNumberOfEventToBeProcessed();
    EDeposit = new G4double[nEvents];
    for(int i=0;i<nEvents;i++){
      EDeposit[i]=0.0;
    }
    MaxE=0.;
  }
  else{
    rootanalysis->BeginOfRunAction();
  }
  //#endif


}

void RunAction::EndOfRunAction(const G4Run*)
{ 
  //#ifdef G4ANALYSIS_USE
  // either write ascii file, or call root
  if(AnalysisManager::getInstance()->GetasciiFileState()){
    // First put the deposited energy into the bins
    G4double binwidth = AnalysisManager::getInstance()->GetBinWidth();
    G4int* bins = new G4int[int(MaxE/binwidth)+5];
    for(int i=0;i<int(MaxE/binwidth)+5;i++){
      bins[i]=0;
    }
    for(int i = 0;i<nEvents;i++){
      binno = int((EDeposit[i]/binwidth) + 0.5);
      bins[binno]++;
    }

    G4double energy = genAction->GetParticleEnergy() ;
    G4String FileName = G4UIcommand::ConvertToString(energy/keV) + ".dat";
    G4cout << "File Name = " << FileName << G4endl;
    std::ofstream asciiFile(FileName);

    if(asciiFile.is_open()) {
      // Output stuff to ascii file
      for(G4int i=0;i<int(MaxE/binwidth)+5;i++){
	asciiFile << std::setiosflags(std::ios::fixed)
		  << std::setprecision(3)
		  << std::setiosflags(std::ios::right)
		  << std::setw(10);
	asciiFile << i;
	asciiFile << std::setiosflags(std::ios::fixed)
		  << std::setprecision(3)
		  << std::setiosflags(std::ios::right)
		  << std::setw(10);
	asciiFile << bins[i];
	asciiFile  << G4endl;
      }
      asciiFile.close();
    }
  }
  else{
    rootanalysis->EndOfRunAction();;
  }
  //#endif //G4ANALYSIS_USE

}
