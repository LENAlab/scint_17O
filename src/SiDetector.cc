//----------------------------------------------------------------------
// SiDetector.cc
//
// Description: Simulation of a simple silicon detector
//
//----------------------------------------------------------------------

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

//#ifdef G4ANALYSIS_USE
//#include <AIDA/AIDA.h>
#include "AnalysisManager.hh"
//#endif

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "RootAnalysis.hh"
//#include "LHEP_PRECO_HP.hh"

int main(int argc,char** argv)
{
  // choose the Random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

  // Construct the run manager
  G4RunManager* runManager = new G4RunManager;

  //#ifdef G4ANALYSIS_USE
  //constructe the analysis manager (need here to activate the UI)
  RootAnalysis* rootanalysis = new RootAnalysis; 
  AnalysisManager* aMgr = AnalysisManager::getInstance(); 
  //#endif

  // Set needed initialization classes
  DetectorConstruction* detector = new DetectorConstruction;
  runManager->SetUserInitialization(detector);
  //runManager->SetUserInitialization(new LHEP_PRECO_HP);
  runManager->SetUserInitialization(new PhysicsList);

  // Visualization
  G4VisManager* visManager = new G4VisExecutive;
  // uncomment when using X
  visManager->Initialize();

  // Set User Action classes
  RunAction* runaction;
  EventAction* eventaction;
  PrimaryGeneratorAction* genaction = new PrimaryGeneratorAction;

  runManager->SetUserAction(genaction);
  runManager->SetUserAction(runaction = new RunAction(genaction,rootanalysis));
  runManager->SetUserAction(eventaction = new EventAction(runaction,genaction,rootanalysis)); 
  // Initialize the Kernel!
  runManager->Initialize();

  // Get pointer to UI manager
  G4UImanager* UI = G4UImanager::GetUIpointer();

  if(argc==1)
    // if no argument to program, start vis manager
    {
#ifdef G4UI_USE
      G4UIExecutive* ui = new G4UIExecutive(argc, argv);
      UI->ApplyCommand("/control/execute macs/init_vis.mac"); 
      ui->SessionStart();
      delete ui;
#endif
      //      G4UIsession * session = new G4UIterminal(new G4UItcsh);
      //      session->SessionStart();
      //      delete session;
    }
  else
    // Else run the argument in batch mode
    { 
      G4String command = "/control/execute ";
      G4String filename = argv[1];
      UI->ApplyCommand(command+filename);
      //   G4UIsession * session = new G4UIterminal(new G4UItcsh);
      //session->SessionStart();
      //delete session;
    }

  //kill everything and end
  //#ifdef G4ANALYSIS_USE
  delete aMgr;
  //#endif
  delete visManager;
  delete runManager;

  return 0;
}
