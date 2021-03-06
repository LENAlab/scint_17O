//--------------------------------------------------------------------
// PrimaryGeneratorAction.hh
//
// Description: Sorts out generation, events etc. inlcude file
// Changes: 7/18/05 None yet
//--------------------------------------------------------------------

#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"

class G4Event;
class G4ParticleGun;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGeneratorAction();
  ~PrimaryGeneratorAction();

public:
  // Make the particle and fire it
  void GeneratePrimaries(G4Event*);
  // Use this for ascii file naming
  G4double GetParticleEnergy(){return angular_energy;};
  // Get number of particles produced for ascii file stuff
  G4int GetNumberOfParticles(){return nParticles;};

private:
  G4ParticleGun* particleGun;
  G4double angular_energy;
  G4int nParticles;

};


#endif
