#include "PrimaryGeneratorAction.hh"
#include "G4SystemOfUnits.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
  : G4VUserPrimaryGeneratorAction(),
    fGeneralParticleSource(0)
{
  fGeneralParticleSource  = new G4GeneralParticleSource();
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fGeneralParticleSource;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  fGeneralParticleSource->GeneratePrimaryVertex(anEvent);
}

