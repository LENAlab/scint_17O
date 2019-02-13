//----------------------------------------------------------------------
// RootOutputData.hh
//
// Description: The class for the stored ROOT data
//
// Version 0.1, 8/7/06
// Changes: Nothing!
//----------------------------------------------------------------------

#ifndef RootOutputData_h
#define RootOutputData_h

#include "TObject.h"

//---------------------------------------------------------------------------//

class RootOutputData : public TObject
{
public:
  RootOutputData();
  ~RootOutputData();

  void AllocateArrays(Int_t fhits);
  void DeAllocateArrays();

  // The Pointers that save the stepping action

  Int_t fEventID;      // The event ID
  Int_t fhits;        // Number of Hits
  Int_t fTotalNumberOfSteps;
  Int_t fMaxArrayLength;

  Float_t *fhitEnergy;     //[fhits] 
  Float_t fEnergy;    // Quick and dirty total energy dep

  Float_t *fLocalPosx;   //[fhits] 
  Float_t *fLocalPosy;   //[fhits] 
  Float_t *fLocalPosz;   //[fhits] 

  Float_t *fTime;     //[fhits] 
  
  Int_t *fPartID;   //[fhits] 
  Int_t fParticleNamePos;
  // Length of array containing particle name info

  Char_t * fParticleName;     //[fParticleNamePos]

  Int_t *fParentID;   //[fhits] 

  Float_t *fKE;    //[fhits] 
  Float_t *fDirectionx;     //[fhits] 
  Float_t *fDirectiony;    //[fhits] 
  Float_t *fDirectionz;    //[fhits] 

  Float_t *fCreationPtx;    //[fhits] 
  Float_t *fCreationPty;    //[fhits] 
  Float_t *fCreationPtz;    //[fhits] 

  Int_t *fCopyNo;         //[fhits]

  // The first particle data
  Int_t ffirstPartID;
  Int_t ffirstParentID;
  Float_t ffirstTime;
  Float_t ffirstCreationPtx;
  Float_t ffirstCreationPty;
  Float_t ffirstCreationPtz;

  //  ClassDef(RootOutputData,1)
};

#endif
