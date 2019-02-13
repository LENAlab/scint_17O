//----------------------------------------------------------------------
// RootOutputData.cc
//
// Description: The class for the stored ROOT data
//
// Version 0.1, 8/7/06
// Changes: Nothing!
//----------------------------------------------------------------------

#include "RootOutputData.hh"

//ClassImp(RootOutputData)

  RootOutputData::RootOutputData() :
    fhitEnergy(0),
    fLocalPosx(0),
    fLocalPosy(0),
    fLocalPosz(0),
    fTime(0),
    fPartID(0),
    fParticleName(0),
    fParentID(0),
    fKE(0),
    fDirectionx(0),
    fDirectiony(0),
    fDirectionz(0),
    fCreationPtx(0),
    fCreationPty(0),
    fCreationPtz(0),
  fCopyNo(0)
{
  fMaxArrayLength = 10000;
}

//--------------------------------------------------------------------------------//

RootOutputData::~RootOutputData(){;}

//--------------------------------------------------------------------------------//

void RootOutputData::AllocateArrays(Int_t fhits){

  // Allocate the arrays with the number of hits in each event
  fhitEnergy = new Float_t[fhits];
  fLocalPosx = new Float_t[fhits];
  fLocalPosy = new Float_t[fhits];  
  fLocalPosz = new Float_t[fhits];
  fTime = new Float_t[fhits];
  fPartID = new Int_t[fhits];
  // Assume the average particle had a name of length 7
  fParticleName = new Char_t[10*fhits];
  fParentID = new Int_t[fhits];
  fKE = new Float_t[fhits];
  fDirectionx = new Float_t[fhits];
  fDirectiony = new Float_t[fhits];
  fDirectionz = new Float_t[fhits];
  fCreationPtx = new Float_t[fhits];
  fCreationPty = new Float_t[fhits];
  fCreationPtz = new Float_t[fhits];
  fCopyNo = new Int_t[fhits];
}

//--------------------------------------------------------------------------------------//

void RootOutputData::DeAllocateArrays(){
  // Clear the arrays and set pointers to zero.
  delete [] fhitEnergy;
  delete [] fLocalPosx;
  delete [] fLocalPosy;
  delete [] fLocalPosz;
  delete [] fTime;
  delete [] fPartID;
  delete [] fParticleName;
  delete [] fParentID;
  delete [] fKE;
  delete [] fDirectionx;
  delete [] fDirectiony;
  delete [] fDirectionz;
  delete [] fCreationPtx;
  delete [] fCreationPty;
  delete [] fCreationPtz;
  delete [] fCopyNo;

  fhitEnergy = fLocalPosx = fLocalPosy = fLocalPosz = fTime
    = fKE = fDirectionx = fDirectiony = fDirectionz = fCreationPtx
    = fCreationPty = fCreationPtz = 0;
  fPartID = fParentID = 0;
  fParticleName = 0;
  fCopyNo = 0;
}

