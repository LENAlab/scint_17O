//--------------------------------------------------------------------
// DetectorConstruction.hh
//
// Description: The detector definitions, materials etc.
// Changes: 7/18/05 None yet
//--------------------------------------------------------------------

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4Box;
class G4Tubs;
class G4UnionSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class DetectorMessenger;

class TargetSD;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:

  DetectorConstruction();
  ~DetectorConstruction();

public:

  G4VPhysicalVolume* Construct();

  void UpdateGeometry();
  G4bool SDflag;

private:
  void DefineMaterials();

  // Materials
  G4Material* Al;
  G4Material* Pb;
  G4Material* Cu;
  G4Material* Ta;
  G4Material* Ge;
  G4Material* Li;
  G4Material* B;
  G4Material* Si;
  G4Material* Air;
  G4Material* vacuum;
  G4Material* Water;
  G4Material* NaITl;
  G4Material* Sci;
  G4Material* ssteel;
  G4Material* mylar;
  G4Material* TargetMat;
  G4Material* CH2;
  G4Material* CD2;
  G4Material* Carbon;

  G4double LENALabx;                // x length of world
  G4double LENALaby;                // y length of world
  G4double LENALabz;                // z length of world
  G4double Source_Detectorz;         // The dist between source and det face

  // The Sensitive Detectors
  TargetSD* DetectorSD;

  DetectorMessenger* detectorMessenger; // pointer to messenger

};

#endif
