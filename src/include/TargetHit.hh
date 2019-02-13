//--------------------------------------------------------------------
// TargetHit.hh
//
// Description: Hits in the Target (based on SLAC example)
// Changes: 11/17/05 None yet
//--------------------------------------------------------------------
#ifndef TargetHit_h
#define TargetHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"

class TargetHit : public G4VHit
{
public:

  TargetHit();
  ~TargetHit();
  TargetHit(const TargetHit&);
  const TargetHit& operator=(const TargetHit&);
  int operator==(const TargetHit&) const;

  inline void* operator new(size_t);
  inline void operator delete(void*);

  void Draw();
  void Print();

private:
  // The hit info
  G4double edep;
  G4ThreeVector Pos;
  G4double Time;
  G4ParticleDefinition* PartID;
  G4String PartName;
  G4int ParentID;
  G4double KE;
  G4ThreeVector Direction;
  G4ThreeVector CreationPt;
  G4int CopyNo;

public:
  // Get and set the hit info for each hit.
  void SetEdep(G4double de) { edep = de;}
  G4double GetEdep() const { return edep;}
  
   void SetLocalPos(G4ThreeVector pos){Pos = pos;}
  G4ThreeVector GetLocalPos() const { return Pos;}

  void SetTime(G4double time){Time=time;}
  G4double GetTime() const { return Time;}

  void SetPartID(G4ParticleDefinition* part){PartID=part;}
  G4ParticleDefinition* GetPartID() const { return PartID;}

  void SetParticleName(G4String name){PartName=name;}
  G4String GetParticleName() const {return PartName;}

  void SetParentID(G4int parent){ParentID=parent;}
  G4int GetParentID() const { return ParentID;}

  void SetKE(G4double ke){KE=ke;}
  G4double GetKE() const { return KE;}

  void SetDirection(G4ThreeVector dir){Direction=dir;}
  G4ThreeVector GetDirection() const { return Direction;}

  void SetCreationPt(G4ThreeVector cpt){CreationPt=cpt;}
  G4ThreeVector GetCreationPt() const { return CreationPt;} 

  void SetCopyNo(G4int copy){CopyNo=copy;}
  G4int GetCopyNo() const { return CopyNo; }

};

typedef G4THitsCollection<TargetHit> TargetHitsCollection;

extern G4Allocator<TargetHit> TargetHitAllocator;

inline void* TargetHit::operator new(size_t)
{
  void* aHit;
  aHit = (void*)TargetHitAllocator.MallocSingle();
  return aHit;
}

inline void TargetHit::operator delete(void* aHit)
{
  TargetHitAllocator.FreeSingle((TargetHit*) aHit);
}

#endif
