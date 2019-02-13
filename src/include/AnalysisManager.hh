//----------------------------------------------------------------------
// AnalysisManager.hh
//
// Description: Sorts out the analysis, Include file
//
// Version 0.1, 8/3/05
// Changes: Nothing!
//----------------------------------------------------------------------
#ifndef AnalysisManager_h
#define AnalysisManager_h 1

//#ifdef G4ANALYSIS_USE

#include "G4ThreeVector.hh"
//#include "AIDA/AIDA.h"
#include "globals.hh"
#include "G4SystemOfUnits.hh"

//using namespace AIDA;

//class AIDA::IAnalysisFactory;
//class AIDA::ITree;
//class AIDA::ITupleFactory;

class G4Track;
class AnalysisMessenger;

class AnalysisManager {
public:

  virtual ~AnalysisManager();
  static AnalysisManager* getInstance();
  static void dispose();

  //  ITupleFactory* getTupleFactory();

public:
  void BeginOfRun();
  void EndOfRun();

  // All the commands 
  void SetFileName(G4String filename) {fileName = filename;};
  void SetasciiFile(G4bool asciibool){ asciiFileBool = asciibool;  };
  void SetBinWidth(G4double binwidth){binWidth = binwidth;};
  void SetDetailBool(G4bool detailbool){detailBool = detailbool;};
  void SetResolutionBool(G4bool resolutionbool){resolutionBool = resolutionbool;};
  void SetGeResolution(G4double geresolution){GeResolution = geresolution*keV;};

  // Routines to fill aida tuples
  void Fill(G4double,G4ThreeVector,G4double,G4String,
	    G4int,G4double,G4ThreeVector,G4ThreeVector,G4int,G4int);
  void Fill2(G4double,G4String,G4int,G4ThreeVector,G4int);

  // Get what the commands are set to
  G4bool GetasciiFileState(){ return asciiFileBool ;};
  G4double GetBinWidth(){ return binWidth;};
  G4bool GetDetailBool(){return detailBool;};
  G4bool GetResolutionBool(){return resolutionBool;};
  G4double GetGeResolution(){return GeResolution;};
  G4String GetFileName(){return fileName;};

private:

  AnalysisManager();
  static AnalysisManager* instance;

  // IAnalysisFactory* analysisFactory;
  // ITupleFactory* tFactory;
  // ITree* tree;

  // ITuple* tuple;
  // ITuple* tuple2;

  G4String fileName;
  G4bool asciiFileBool;
  G4double binWidth;
  G4bool detailBool;
  G4bool resolutionBool;
  G4double GeResolution;

  AnalysisMessenger* analysisMessenger;
};
//#endif // G4ANALYSIS_USE

#endif

