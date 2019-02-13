//--------------------------------------------------------------------
// DetectorConstruction.cc
//
// Description: The detector definitions, materials etc.
// Changes: 7/15/05 None yet
//--------------------------------------------------------------------

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4PVReplica.hh"
#include "G4RotationMatrix.hh"

#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "TargetSD.hh"

#include "G4NistManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"

#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "DetectorConstruction.hh"
#include <cmath>

DetectorConstruction::DetectorConstruction()
{

  DefineMaterials();
  SDflag=false;
}

DetectorConstruction::~DetectorConstruction()
{ }

void DetectorConstruction::DefineMaterials()
{
  //---------------Materials-----------------

  G4double a; //atomic mass
  G4double z; //atomic number
  G4double density, fractionmass,temperature,pressure;
  G4int ncomponents; //natoms;

  Al = 
     new G4Material("Aluminum", z= 13., a= 26.98*g/mole, density= 2.7*g/cm3);

  Pb =
     new G4Material("Lead", z=82.,a=207.2*g/mole, density=11340.*kg/m3);

  Cu = 
     new G4Material("Copper", z=29.,a=63.546*g/mole, density=8920.*kg/m3);

  Ta =
    new G4Material("Tantalum",z=73.,a=180.9479*g/mole,density=16650*kg/m3);


  Ge =
     new G4Material("Germanium",z=32.,a=72.61*g/mole,density=5323*kg/m3);

  Li = 
     new G4Material("Lithium",z=3.,a=6.941*g/mole,density=0.534*g/cm3);

  B =
     new G4Material("Boron",z=5.,a=10.811*g/mole,density=2.31*g/cm3);

  Si =
    new G4Material("Silicon",z=14.,a=28.0855*g/mole,density=2.33*g/cm3);

  // the elements
  a = 14.01*g/mole;
  G4Element* elN = new G4Element("Nitrogen","N",z=7.,a);
  a = 16.00*g/mole;
  G4Element* elO = new G4Element("Oxygen","O",z=8.,a);
  // 22Ne
  a = 21.991385*g/mole;
  G4Element* elNe = new G4Element("Neon","Ne",z=10.,a);
  a = 180.9479*g/mole;
  G4Element* elTa = new G4Element("Tantalum","Ta",z=73.,a);
  a = 12.011*g/mole;
  G4Element* elC = new G4Element("Carbon","C",z=6.,a);
  a=16.99*g/mole;
  G4Element* O17 = new G4Element("Oxygen17","17O",z=8.,a);
  // a = 1.00794*g/mole;
  // G4Element* elH = new G4Element("Hydrogen","H",z=1.,a);
//   a = 22.989768*g/mole;
//   G4Element* elNa = new G4Element("Sodium","Na",z=11.,a);
//   a = 126.90447*g/mole;
//   G4Element* elI  = new G4Element("Iodine","I",z=53.,a);
//   a = 204.3833*g/mole;
//   G4Element* elTl = new G4Element("Thallium","Tl",z=81.,a);
//   a = 58.9332*g/mole;
//   G4Element* elCo = new G4Element( "Cobalt", "Co", 27. ,a);
//   a = 55.85*g/mole;
//   G4Element* elFe = new G4Element("Iron" , "Fe" , z= 26., a);  

  // making vacuum
  vacuum = new G4Material("Vacuum", z=1., a=1.*g/mole, 
			  density=1.e-20*g/cm3,
			  kStateGas, temperature=0.1*kelvin, 
			  pressure=1.e-20*bar);

  // Air
  density = 1.290*mg/cm3;
  Air = new G4Material("Air",density,ncomponents=2);
  Air->AddElement(elN, fractionmass=0.7);
  Air->AddElement(elO, fractionmass=0.3);

  // Neon-22 implanted target
  //  G4double Tadensity = 16650*kg/m3;

  density = 8.2*g/cm3;
  TargetMat = new G4Material("TargetMat",density,ncomponents=2);
  TargetMat->AddElement(elTa,fractionmass=0.819);
  TargetMat->AddElement(O17,fractionmass=0.181);

  G4NistManager* man = G4NistManager::Instance();
  CH2  = man->FindOrBuildMaterial("G4_POLYETHYLENE");
  CD2  = man->FindOrBuildMaterial("G4_POLYETHYLENE");
  
  // Carbon target material
  density = 2.648*g/cm3;
  Carbon = new G4Material("Carbon",density,ncomponents=1);
  Carbon->AddElement(elC,fractionmass=1.0);
//  Carbon = man->FindOrBuildMaterial("G4_C");


}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // First clean out the old geom
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

//**************************************************************************

  //-------------------------------------------------
  //------------------Volumes------------------------
  //-------------------------------------------------
  // z - Along Beam
  // x - Horizontal plane
  // y - vertical plane

  // Define some lengths
  LENALabz = 1500*mm;
  LENALabx = 1500*mm;
  LENALaby = 1500*mm;
  Source_Detectorz = 10.525*mm; // from the drawings for the target holder
  // new source detector distances
  // Source_Detectorz = 5.525*mm;
  // ----------WORLD-------------

  G4double HalfLENALabz = LENALabz*0.5;
  G4double HalfLENALabx = LENALabx*0.5;
  G4double HalfLENALaby = LENALaby*0.5;

  G4Box* LENALab_solid
    = new G4Box("LENALab",HalfLENALabx,HalfLENALaby,HalfLENALabz);
  G4LogicalVolume* LENALab_log = 
    new G4LogicalVolume(LENALab_solid, vacuum, "LENALab_log", 0,0,0);
  G4VPhysicalVolume* LENALab_phys = 
    new G4PVPlacement(0,                // no rotation
		      G4ThreeVector(),  // at (0,0,0)
		      LENALab_log,      // Logical Volume
		      "LENALab",        // Name
		      0,                // Mother Volume
		      false,            // no boolean operations
		      0);               // Copy number



  // The Target
  G4double Target_outer = 10.*mm;
  G4double Target_Hthick = 0.25*mm; 
  G4Tubs* Target_solid = new G4Tubs("Target",0.*mm,Target_outer,Target_Hthick,0,twopi);
  G4LogicalVolume* Target_log = new G4LogicalVolume(Target_solid,TargetMat,"Target",0,0,0);

  G4RotationMatrix* Target_rot = new G4RotationMatrix();
  G4double angle = 0.0*deg;
  Target_rot->rotateX(angle);
  // G4VPhysicalVolume* TantBack_phys = 
  new G4PVPlacement(Target_rot,
		    G4ThreeVector(0,0,0),
		    Target_log,
		    "Target",
		    LENALab_log,
		    false,
		    1);
  

  // The detector plane
  G4double SourceDetector = 50.*mm;
  G4double Detector_Height = 20.*mm;
  G4double Detector_Hthick = 1.*mm;
  
  G4double det_angle=0;
  G4double det_angle_rad=det_angle/180*pi;
  G4double det_transx=-(SourceDetector*(std::sin(det_angle_rad)));
  G4double det_transz=(SourceDetector-SourceDetector*(std::cos(det_angle_rad)));
  G4double Detector_some_height=0*mm; //var to hold relevant detector height
  //G4Tubs("name",ir,or,hz,start angle,span angle)
  // G4Tubs* Detector_solid = new G4Tubs("Detector",SourceDetector,SourceDetector+Detector_Hthick, Detector_Height,0,pi);
  
  	  
  
  
  G4Tubs* Detector_solid1 = new G4Tubs("Detector1",SourceDetector,SourceDetector+Detector_Hthick,
		 Detector_Height,0,.5*pi);
  G4LogicalVolume* Detector_log1 = new G4LogicalVolume(Detector_solid1,Si,"Silicon",0,0,0);
  // G4VPhysicalVolume* TantBack_phys = 
  G4ThreeVector yAxis1(0,1.,0);
  G4ThreeVector xAxis1(1.,0,0);
  G4ThreeVector zAxis1(0,0,1.);

  G4RotationMatrix* Detector_rot1 = new G4RotationMatrix();
  angle = -90.*deg;
  Detector_rot1->rotateX(angle);
  Detector_rot1->rotateZ(det_angle*deg);
  new G4PVPlacement(Detector_rot1,
		    G4ThreeVector(Target_outer+det_transx,0,det_transz+Detector_some_height), //SourceDetector*sin(angle),SourceDetector*cos(angle)),
		    Detector_log1,
		    "Silicon",
		    LENALab_log,
		    false,
		    0);




  //------------------------------------------------------------
  //-------------- Sensitive Detectors -----------------
  //-----------------------------------------------------------
  
  // Sensitive Ge Detector
  if(!SDflag){
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    G4String SDname;
    
    DetectorSD = new TargetSD(SDname="/DetectorSD", this);
    SDman->AddNewDetector(DetectorSD);
  
    SDflag=true;
  }
  Detector_log1->SetSensitiveDetector(DetectorSD);
  Target_log->SetSensitiveDetector(DetectorSD);
// Scorer_log->SetSensitiveDetector(TaSD);
  //
  //------------------------------------------------------------
  //-------------- Visualization Stuff -----------------------
  //------------------------------------------------------------

  // The World is invisible
  LENALab_log ->SetVisAttributes(G4VisAttributes::Invisible);

  // Tantalum and steel are Dark Grey
  G4VisAttributes* TaVisAtt = new G4VisAttributes(G4Colour(0.4,0.4,0.4));
  Target_log->SetVisAttributes(TaVisAtt);

  // Detector is green
  G4VisAttributes* DetVisAtt = new G4VisAttributes(G4Colour(0,1,0));
  Detector_log1->SetVisAttributes(DetVisAtt);

  //-----------------------------------------------------------------
  return LENALab_phys;
}
