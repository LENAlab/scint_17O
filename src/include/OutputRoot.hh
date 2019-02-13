//----------------------------------------------------------------------
// OutputRoot.hh
//
// Description: Root book keeping stuff. 
//
// Version 0.1, 8/7/06
// Changes: Nothing!
//----------------------------------------------------------------------
#ifndef OutputRoot_h
#define OutputRoot_h 1

//CLHEP
#include <CLHEP/Units/SystemOfUnits.h>
//#include <CLHEP/config/CLHEP.h>

// Root
#include "TFile.h"
#include "TObject.h"
#include "TTree.h"

// Geant 4
#include "globals.hh"
#include "G4ios.hh"
#include "G4Event.hh"
#include "G4Step.hh"

class OutputRoot {
public:
  OutputRoot();
  virtual ~OutputRoot(){}

  void OpenFile();
  void CloseFile();
  void WriteFile();

  void FillTree(){fTree->Fill();} // Do this every step in sensitive detector

  virtual void DefineSchema()=0;

    /** Action to perform at beginning and end of events and runs
     Detector specific.
     Only include actions that directly affect the Root tree.
    */
    virtual void BeginOfEventAction(const G4Event*) {;}
    virtual void BeginOfRunAction() {;}
    virtual void EndOfEventAction(const G4Event*) {;}
    virtual void EndOfRunAction() {;}

    // Overloaded Conversion Methods: Root --> Geant 4
    G4double  RootToG4(Double_t XX) { return (G4double) XX; }
    G4float   RootToG4(Float_t XX) { return (G4float) XX; }
    G4int     RootToG4(Int_t XX) { return (G4int) XX; }
    G4long    RootToG4(Long_t XX) { return (G4long) XX; }

    // Overloaded Conversion Methods: Geant4 --> Root
    Double_t  G4ToRoot(G4double XX) { return (Double_t) XX; }
    Float_t   G4ToRoot(G4float XX) { return (Float_t) XX; }
    Int_t     G4ToRoot(G4int XX) { return (Int_t) XX; }
    Long_t    G4ToRoot(G4long XX) { return (Long_t) XX; }

    // Setters and Getters.
    // G4String GetFileName() { return fFileName; }
    TFile *GetRootFile() { return fRootFile; }
    TTree *GetTree() { return fTree; }
  //    void SetRootFileName(G4String name) { SetFileName(name);}
    void SetRootFile(TFile *rfile) { fRootFile = rfile; }
    void SetTree(TTree *XtalTree) { fTree = XtalTree; }

  protected:
    static TTree *fTree; 
    // Tree that is written to file that contains event information.

  private:
    static TFile *fRootFile;
    // File that tree and other information that may be defined in the 
    // inherited class is written to.

  G4String fFileName;

};
#endif
