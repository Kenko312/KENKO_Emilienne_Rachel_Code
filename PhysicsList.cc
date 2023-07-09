#include "PhysicsList.hh"
#include "G4SystemOfUnits.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"

PhysicsList::PhysicsList()
: G4VModularPhysicsList(){
  SetVerboseLevel(4);
  defaultCutValue = 0.05 * mm;

  RegisterPhysics(new G4DecayPhysics());
  RegisterPhysics(new G4RadioactiveDecayPhysics());
  RegisterPhysics(new G4EmStandardPhysics_option4());
}
PhysicsList::~PhysicsList()
{ 
}
void PhysicsList::SetCuts()
{
  G4VUserPhysicsList::SetCuts();
}  
