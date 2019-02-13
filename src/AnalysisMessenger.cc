//----------------------------------------------------------------------
// AnalysisMessenger.cc
//
// Description: Sends info to the Analysis Manager
//                     Most generic comands are here too
//
// Version 0.1, 1/18/06
// Changes: Nothing!
//----------------------------------------------------------------------
//#ifdef G4ANALYSIS_USE

//#include <AIDA/AIDA.h>

#include "AnalysisMessenger.hh"
#include "AnalysisManager.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"

AnalysisMessenger::AnalysisMessenger(AnalysisManager* analysisManager)
  :Analysis(analysisManager)

{
  AnalysisDir = new G4UIdirectory("/analysis/");
  AnalysisDir->SetGuidance("analysis control.");

  FileNameCmd = new G4UIcmdWithAString("/analysis/filename",this);
  FileNameCmd->SetGuidance("Input the name for the AIDA output file.");
  FileNameCmd->SetParameterName("filename",true,true);
  FileNameCmd->SetDefaultValue("LENAGe.aida");

  AsciiFileCmd = new G4UIcmdWithABool("/analysis/asciifile",this);
  AsciiFileCmd->SetGuidance("Set to true if you want an ascii spectrum exported.");
  AsciiFileCmd->SetParameterName("asciiFileBool",true,false);
  AsciiFileCmd->SetDefaultValue(false);
}

AnalysisMessenger::~AnalysisMessenger()
{
  delete FileNameCmd; 
  delete AsciiFileCmd;
}

void AnalysisMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 

  // Where the commands are executed. 
  if( command == FileNameCmd ) { 
    Analysis->SetFileName(newValue);
  }
  if( command == AsciiFileCmd ) {
    Analysis->SetasciiFile(AsciiFileCmd->GetNewBoolValue(newValue));
  }
}

//#endif // G4ANALYSIS_USE
