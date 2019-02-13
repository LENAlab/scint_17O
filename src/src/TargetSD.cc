//--------------------------------------------------------------------
// GeSD.cc
//
// Description: The Sensitive Ge detector 
//              (based on SLAC HadCalorimeter example)
// Changes: 11/17/05 None yet
//--------------------------------------------------------------------

#include "TargetSD.hh"
#include "TargetHit.hh"
#include "DetectorConstruction.hh"

#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

TargetSD::TargetSD(G4String name,DetectorConstruction* LENASD)
  :G4VSensitiveDetector(name),LENADetector(LENASD)
{
  G4String HCname="targetCollection";
  collectionName.insert(HCname);

}

TargetSD::~TargetSD(){;}

void TargetSD::Initialize(G4HCofThisEvent*)
{

  targetCollection = new TargetHitsCollection
    (SensitiveDetectorName,collectionName[0]);

  TargetHitID = -1;

}

G4bool TargetSD::ProcessHits(G4Step*aStep,G4TouchableHistory*)
{
  G4double edep = aStep->GetTotalEnergyDeposit();
  if(edep==0.) return false; // used to be true

  // Point to track to get hit information, and write it to the hit
  // For later readout (in analysis section)
  TargetHit* GenewHit = new TargetHit();
  G4Track* track = aStep->GetTrack();
  GenewHit->SetEdep(edep);
  GenewHit->SetLocalPos(track->GetPosition());
  GenewHit->SetTime(track->GetGlobalTime());
  GenewHit->SetPartID(track->GetDefinition());
  GenewHit->SetParticleName(track->GetDefinition()->GetParticleName());
  GenewHit->SetParentID(track->GetParentID());
  GenewHit->SetKE(track->GetKineticEnergy());
  GenewHit->SetDirection(track->GetMomentumDirection());
  GenewHit->SetCreationPt(track->GetVertexPosition());
  GenewHit->SetCopyNo(track->GetTouchable()->GetCopyNumber());
  TargetHitID = targetCollection->insert(GenewHit);

  return true;
}

void TargetSD::EndOfEvent(G4HCofThisEvent* HCE)
{

  G4String HCname = collectionName[0];
  static G4int HCID = -1;
  if(HCID<0)
    HCID = G4SDManager::GetSDMpointer()->GetCollectionID(HCname);
  // Add all hits to the targetanium
  HCE->AddHitsCollection(HCID,targetCollection);

  G4int nHits = targetCollection->entries();
  if (verboseLevel>=1)
    G4cout << "   Ge Collection: " << nHits << G4endl;
  if (verboseLevel>=2)
    targetCollection->PrintAllHits();
}

