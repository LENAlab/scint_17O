//--------------------------------------------------------------------
// EventAction.cc
//
// Description: Events!
// Changes: 7/18/05 None yet
//--------------------------------------------------------------------

#include "G4Event.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4GeneralParticleSource.hh"

#include "Randomize.hh"

#include "EventAction.hh"
#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RootAnalysis.hh"

//#ifdef G4ANALYSIS_USE
#include "AnalysisManager.hh"
//#endif // G4ANALYSIS_USE

EventAction::EventAction(RunAction* run,PrimaryGeneratorAction* gen,
			 RootAnalysis* root)
  :runaction(run),genaction(gen),rootanalysis(root)
{
  //Set up Hits Collections
  targetCollID = -1;

  energy_pri=0;
  seeds=NULL;

  verboseLevel = 0;
}

EventAction::~EventAction()
{ }

void EventAction::BeginOfEventAction(const G4Event* evt)
{

  event_id = evt->GetEventID();
  // Print out event number every 10000 events
  if((event_id % 1000) == 0.) 
    G4cout << " Event Number " << event_id << G4endl;


  // If we should create an ascii, initialise stuff, otherwise, call root stuff
  if(AnalysisManager::getInstance()->GetasciiFileState()){
    Energy = 0.;
    energy = genaction->GetParticleEnergy();
    MaxE = runaction->GetMaxE();
    // Get ID for Germ hits collection
    if (targetCollID==-1){
      G4SDManager *SDman = G4SDManager::GetSDMpointer();
      targetCollID = SDman->GetCollectionID("targetCollection");
    }
  } else {
    // Start the root analysis
    rootanalysis->BeginOfEventAction(evt);
  }

}

void EventAction::EndOfEventAction(const G4Event* evt)
{
  // If we're creating an ascii, doo all this, otherwise, just call root
  if(AnalysisManager::getInstance()->GetasciiFileState()){
    int evtNo = evt->GetEventID();

    // Check to make sure the collections have been defined
    if(targetCollID<0) return;
 
    //Address Hits Collections
    TargetHitsCollection* THC = NULL;
    G4HCofThisEvent * HCE = evt->GetHCofThisEvent();
    if(HCE){
      THC = (TargetHitsCollection*)(HCE->GetHC(targetCollID));
    }
  
    // Diagnostics
    hits=0;
    if(THC){
      hits = THC->entries();
      Energy = 0.;
      for(G4int i=0; i<hits; i++){
	TargetHit* aHit = (*THC)[i];
	hitEnergy = aHit->GetEdep();
	Energy += hitEnergy;
      }
  
    runaction->SetEDeposit(Energy,evtNo);
    // This is for auto scaling of ascii output
    if(Energy>MaxE)runaction->SetMaxE(Energy);
    }
  } else{
    rootanalysis->EndOfEventAction(evt);
  }
      
}
