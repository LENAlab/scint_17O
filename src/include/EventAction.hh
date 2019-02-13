//--------------------------------------------------------------------
// EventAction.hh
//
// Description: Events! Include File.
// Changes: 7/18/05 None yet
//--------------------------------------------------------------------

#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

#include "TargetHit.hh"

//#ifdef G4ANALYSIS_USE
//#include "AIDA/AIDA.h"
//using namespace AIDA;
//#endif // G4ANALYSIS_USE

//class G4Event;
class RunAction;
class PrimaryGeneratorAction;
class RootAnalysis;

class EventAction : public G4UserEventAction
{
public:
  EventAction(RunAction*,PrimaryGeneratorAction*,RootAnalysis*);
  ~EventAction();

  static EventAction* getInstance();

public:
  virtual void BeginOfEventAction(const G4Event*);
  virtual void EndOfEventAction(const G4Event*);

  // The number of events to use in ascii file autoscale
  void SetNumberOfParticles(G4int nEvent);

private:
  G4int event_id;
  const long* seeds;
  G4double energy_pri;

  // hits collections
  G4int targetCollID;
  G4int hits;

  // Event Summary (not used, but leave for now)
  G4double Energy;
  G4double hitEnergy;
  G4ThreeVector LocalPos;
  G4double Time;
  G4String PartID;
  G4int ParentID;
  G4double KE;
  G4ThreeVector Direction;
  G4ThreeVector CreationPt;
  // First particle
  G4String firstPartID;
  G4int firstParentID;
  G4ThreeVector firstCreationPt;

  G4double energy;
  G4double MaxE;
  G4int nEvent;

  G4int verboseLevel;

  RunAction* runaction;
  PrimaryGeneratorAction* genaction;
  RootAnalysis* rootanalysis;

  public:
    inline void SetVerbose(G4int val) { verboseLevel = val; }
    inline G4int GetVerbose() const { return verboseLevel; }
};

#endif
