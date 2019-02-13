//--------------------------------------------------------------------
// TargetHit.cc
//
// Description: Hits in the Target (based on SLAC example)
// Changes: 11/17/05 None yet
//--------------------------------------------------------------------

#include "TargetHit.hh"
#include "DetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4LogicalVolume.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

G4Allocator<TargetHit> TargetHitAllocator;

TargetHit::TargetHit()
{
  // initialise everything
  edep = 0.;
  Pos = G4ThreeVector(0.,0.,0.);
  Time = 0.;
  PartID = 0;
  PartName = "";
  ParentID = 0;
  KE = 0;
  Direction = G4ThreeVector(0.,0.,0.);
  CreationPt = G4ThreeVector(0.,0.,0.);
  CopyNo = 0;
}

TargetHit::~TargetHit()
{;}

TargetHit::TargetHit(const TargetHit &right)
  : G4VHit(right) {
  // I don't know what this stuff really is. What is "right?"
  edep = right.edep;
  Time = right.Time;
  PartID = right.PartID;
  PartName = right.PartName;
  ParentID = right.ParentID;
  KE = right.KE;
  Direction = right.Direction;
  CreationPt = right.CreationPt;
  CopyNo = right.CopyNo;
}

const TargetHit& TargetHit::operator=(const TargetHit &right)
{
  edep = right.edep;
  Time = right.Time;
  PartID = right.PartID;
  PartName = right.PartName;
  ParentID = right.ParentID;
  KE = right.KE;
  Direction = right.Direction;
  CreationPt = right.CreationPt;
  CopyNo = right.CopyNo;
  return *this;
}

int TargetHit::operator==(const TargetHit& right) const
{
  return (this==&right) ? 1 : 0;
}

void TargetHit::Draw() {;}

void TargetHit::Print()
{
  G4cout << "Target " << edep/keV << " (keV) " << G4endl;
}
