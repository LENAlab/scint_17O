//--------------------------------------------------------------------
// RunAction.hh
//
// Description: Defines the run itself. include file
// Changes: 7/18/05 None yet
//--------------------------------------------------------------------

#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;
class PrimaryGeneratorAction;
class EventAction;
class RootAnalysis;

class RunAction : public G4UserRunAction
{
public:
  RunAction(PrimaryGeneratorAction*,RootAnalysis*);
  ~RunAction();

public:
  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);

  //Use max E for ascii file autoscale
  void SetMaxE(G4double maxe){MaxE=maxe;};
  G4double GetMaxE(){return MaxE;};

  void SetEDeposit(G4double edeposit,G4int i){
    EDeposit[i]=edeposit;
  }

  private:
  G4double MaxE;
  G4double* EDeposit;
  G4int binno;
  G4int nEvents;

  PrimaryGeneratorAction* genAction;
  EventAction* evtAction;
  RootAnalysis* rootanalysis;

};

#endif
