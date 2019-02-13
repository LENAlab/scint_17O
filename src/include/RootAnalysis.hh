//----------------------------------------------------------------------
// RootManager.hh
//
// Description: Sorts out the root analysis
//
// Version 0.1, 8/7/06
// Changes: Nothing!
//----------------------------------------------------------------------

#ifndef RootAnalysis_h
#define RootAnalysis_h 1

#include "TTree.h"

#include "OutputRoot.hh"

#include "TargetHit.hh"

class RootOutputData;

class RootAnalysis : public OutputRoot {

public:
  RootAnalysis();
  ~RootAnalysis();

  void BeginOfRunAction();
  void EndOfRunAction();
  void BeginOfEventAction(const G4Event *evt);
  void EndOfEventAction(const G4Event *evt);
  void DefineSchema();

private:

  RootOutputData *fT;      // The root tree class

  G4int fEventID;
  G4int fStepIndex;    // The number of steps
  
  // hits collections
  G4int targetCollID;
  G4int hits;

  G4String ParticleName;

};

#endif
