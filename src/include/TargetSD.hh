//--------------------------------------------------------------------
// TargetSD.hh
//
// Description: The Sensitive Ge detector (based on SLAC example)
// Changes: 11/17/05 None yet
//--------------------------------------------------------------------
#ifndef TargetSD_h
#define TargetSD_h 1

#include "G4VSensitiveDetector.hh"
#include "TargetHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;
class DetectorConstruction;

class TargetSD : public G4VSensitiveDetector
{

public:
  TargetSD(G4String, DetectorConstruction*);
  ~TargetSD();
  
  void Initialize(G4HCofThisEvent*);
  // For each hit, call ProcessHits
  G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  void EndOfEvent(G4HCofThisEvent*);

private:
  TargetHitsCollection* targetCollection;
  DetectorConstruction* LENADetector;
  G4int TargetHitID;
};


#endif
