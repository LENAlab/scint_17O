//----------------------------------------------------------------------
// AnalysisMessenger.hh
//
// Description: Sends info to the Analysis Manager
//
// Version 0.1, 1/18/06
// Changes: Nothing!
//----------------------------------------------------------------------
#ifndef exGPSAnalysisMessenger_h
#define exGPSAnalysisMessenger_h 1

//#ifdef G4ANALYSIS_USE

#include "globals.hh"
#include "G4UImessenger.hh"

class AnalysisManager;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWithADouble;

class AnalysisMessenger: public G4UImessenger
{
public:AnalysisMessenger(AnalysisManager* );
  ~AnalysisMessenger();
  
  void SetNewValue(G4UIcommand*, G4String);
  
private:
  AnalysisManager* Analysis;
  G4UIdirectory* AnalysisDir;
  
  // The commands
  G4UIcmdWithAString* FileNameCmd;
  G4UIcmdWithABool* AsciiFileCmd;
  G4UIcmdWithADouble* BinWidthCmd;
  G4UIcmdWithABool* DetailCmd;
  G4UIcmdWithABool* ResolutionCmd;
  G4UIcmdWithADouble* GeResolutionCmd;
};

//#endif // G4ANALYSIS_USE

#endif
