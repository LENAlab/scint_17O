//--------------------------------------------------------------------
// PrimaryGeneratorAction.cc
//
// Description: Sorts out generation of events. Use GPS
// Changes: 7/18/05 None Yet
//          1/16/06 Co 60 test
//--------------------------------------------------------------------

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "RunAction.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4PhysicalConstants.hh"
#include "PrimaryGeneratorAction.hh"
#include "Randomize.hh"
#include <cmath>
#include <cstdlib>

#include "G4SystemOfUnits.hh"


PrimaryGeneratorAction::PrimaryGeneratorAction()
{
	particleGun = new G4ParticleGun();
	nParticles=1;
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	G4ParticleDefinition* particle = particleTable->FindParticle("alpha");
	particleGun->SetParticleDefinition(particle);}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete particleGun;
}

// set up the position of the gun
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	// angles for momentum
	G4double	phi1=twopi*G4UniformRand();
	G4double   	theta=.5*pi*G4UniformRand();
	
	// angle and radius for position
	G4double   	phi2=twopi*G4UniformRand();
	G4double   	radius=G4UniformRand()*10*mm;
	// convert angle to kinematic frame of reference for equation
	G4double   	exp_angle=(pi-theta)/pi*180;
	
	//particle momentum direction;
	G4double 	x_dir=std::sin(theta)*std::cos(phi1);
	G4double 	y_dir=std::sin(theta)*std::sin(phi1);
	G4double 	z_dir=std::cos(theta);
	
	//particle positions
	G4double 	x_pos= radius*std::cos(phi2)*mm;
	G4double 	y_pos= radius*std::sin(phi2)*mm;
	G4double 	z_pos=.24995*mm; // is this location correct?
	// 65 KeV for theta>90)
    angular_energy=((5.5921e-6*(exp_angle*exp_angle)-2.1426e-3*exp_angle+1.12244)*MeV);
	//particle direction vector
	G4ThreeVector dir(x_dir,y_dir,z_dir);
	//particle location vector
	G4ThreeVector pos(x_pos,y_pos,z_pos);
	
	particleGun->SetParticlePosition(pos);  
	particleGun->SetParticleMomentumDirection(dir);
	particleGun->GeneratePrimaryVertex(anEvent);
	particleGun->SetParticleEnergy(angular_energy);
	particleGun->SetNumberOfParticles(nParticles);
}

