//----------------------------------------------------------------------
// OutputRoot.cc
//
// Description: Root book keeping stuff. Opens file etc.
//
// Version 0.1, 8/7/06
// Changes: Nothing!
//----------------------------------------------------------------------
#include "TFile.h"
#include "TTree.h"

#include "G4ios.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"

#include "OutputRoot.hh"
#include "AnalysisManager.hh"

TTree* OutputRoot::fTree = NULL;
TFile* OutputRoot::fRootFile = NULL;

OutputRoot::OutputRoot()
{ }

//-------------------------------------------------------------------------------//

void OutputRoot::OpenFile()
{
  // Open the file to write
  fFileName = AnalysisManager::getInstance()->GetFileName();
  fRootFile = new TFile(fFileName.c_str(),"RECREATE");
  // RECREATE overwries files
}

//-------------------------------------------------------------------------------//

void OutputRoot::CloseFile(){
  // Make sure all data is written
  WriteFile();
  // Delete everything if it's open
  if(fTree) fTree->Print();   // Print info on the file created
  if(fRootFile != NULL){
    fRootFile->Close();    // Delete the fTree
    delete fRootFile;
    fRootFile = NULL;
    fTree = NULL;
  }
}

//-------------------------------------------------------------------------------//

void OutputRoot::WriteFile(){
  // Write out stuff
  if(fTree) fTree->Write(fTree->GetName(),TObject::kOverwrite);
}
