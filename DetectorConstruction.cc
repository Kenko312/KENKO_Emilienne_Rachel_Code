#include "DetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Tubs.hh"

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction()
{ }

DetectorConstruction::~DetectorConstruction()
{ }

G4VPhysicalVolume* DetectorConstruction::Construct()
{  
  G4NistManager* nist = G4NistManager::Instance();
  //G4Material* titanium = nist -> FindOrBuildMaterial("G4_Ti");
  //G4Material* iodine = nist -> FindOrBuildMaterial("G4_I");
  G4Material* radium = nist -> FindOrBuildMaterial("G4_Ra");
  G4Material* air = nist -> FindOrBuildMaterial("G4_AIR");
  
  G4Colour  red     (1.0, 0.0, 0.0);
  G4Colour  magenta (1.0, 0.0, 1.0);
  G4Colour  lblue   (0.0, 0.0, .75);
  
  
G4Material* matAir = nist->FindOrBuildMaterial("G4_AIR");
	G4Material* matWater = nist->FindOrBuildMaterial("G4_WATER");
	//G4Material* matCo = nist->FindOrBuildMaterial("G4_Co");

	//=========================================================================================
	G4Box* SBox = new G4Box("World", 10. * cm, 10. * cm, 10. * cm);
	G4LogicalVolume* LBox = new G4LogicalVolume(SBox, matAir, "World");
	G4VPhysicalVolume* PBox = new G4PVPlacement(0, G4ThreeVector(), LBox, "World", 0, false, 0, true);

	G4VisAttributes* worldvisAtt = new G4VisAttributes(true);
	LBox->SetVisAttributes(worldvisAtt);

	//==========================================================================================

	G4Box* solidPhantom = new G4Box("Phantom", 7.5 * cm, 7.5 * cm, 7.5 * cm);

	G4LogicalVolume* logicPhantom = new G4LogicalVolume(solidPhantom, matWater, "Phantom");
	G4VPhysicalVolume* physPhantom = new G4PVPlacement(0, G4ThreeVector(), logicPhantom, "Phantom", LBox, false, 0, true);

	G4VisAttributes* PhantomVisAtt = new G4VisAttributes();
	PhantomVisAtt->SetForceSolid(true);
	PhantomVisAtt->SetColor(135 / 255., 206 / 255., 235 / 255., 0.7);

	logicPhantom->SetVisAttributes(PhantomVisAtt);
 // Air tub
G4VSolid* fDefaultTub = new G4Tubs("DefaultTub",0.*mm, 0.40*mm, 1.84*mm, 0.*deg, 360.*deg);
G4LogicalVolume* fDefaultTubLog = new G4LogicalVolume(fDefaultTub,air,"DefaultTub_Log");
G4VPhysicalVolume* fDefaultTubPhys = new G4PVPlacement(nullptr,
                                  G4ThreeVector(),
                                  "defaultTub_Phys",
                                  fDefaultTubLog,
                                  physPhantom,
                                  false,
                                  0, true); 
// Capsule main body ...
G4VSolid* fCapsule = new G4Tubs("fCapsule", 0.35*mm,0.40*mm,1.84*mm,0.*deg,360.*deg);
//G4LogicalVolume* fCapsuleLog = new G4LogicalVolume(fCapsule,titanium,"fCapsuleLog");
G4LogicalVolume* fCapsuleLog = new G4LogicalVolume(fCapsule,air,"fCapsuleLog");
new G4PVPlacement(nullptr,
                  G4ThreeVector(),
                  "fCapsulePhys",
                  fCapsuleLog,
                  fDefaultTubPhys,
                  false,
                  0, true);
// fCapsule tips
G4Sphere* fCapsuleTip = new G4Sphere("fCapsuleTip",
                            0.*mm,
                            0.40*mm,
                            0.*deg,
                            360.*deg,
                            0.*deg,
                            90.*deg);
                            
G4LogicalVolume* fCapsuleTipLog = new G4LogicalVolume(fCapsuleTip,air,"fCapsuleTipLog");
new G4PVPlacement(nullptr,
                  G4ThreeVector(0.,0.,1.84*mm),
                  "IodinefCapsuleTipPhys1",
                  fCapsuleTipLog,
                  physPhantom,
                  false,
                  0, true);

auto rotateMatrix = new G4RotationMatrix();
rotateMatrix -> rotateX(180.0*deg);
new G4PVPlacement(rotateMatrix,
                  G4ThreeVector(0,0,-1.84*mm),
                  "IodinefCapsuleTipPhys2",
                  fCapsuleTipLog,
                  physPhantom,
                  false,
                  0, true);
 
// Radiactive core ...
G4Tubs* fIodineCore = new G4Tubs("ICore",0.085*mm,0.35*mm,1.75*mm,0.*deg,360.*deg);
G4LogicalVolume* fIodineCoreLog = new G4LogicalVolume(fIodineCore,radium,"IodineCoreLog");
new G4PVPlacement(nullptr,
                  G4ThreeVector(0.,0.,0.),
                  "IodineCorePhys",
                  fIodineCoreLog,
                  fDefaultTubPhys,
                  false,
                  0, true);

 
  // Visual attributes ...
  
  G4VisAttributes* fSimpleIodineVisAtt = new G4VisAttributes(magenta);
  fSimpleIodineVisAtt -> SetVisibility(true);
  fSimpleIodineVisAtt -> SetForceSolid(true);
  fIodineCoreLog -> SetVisAttributes(fSimpleIodineVisAtt);

  G4VisAttributes* fSimpleCapsuleVisAtt = new G4VisAttributes(red);
  fSimpleCapsuleVisAtt -> SetVisibility(true);  
  fSimpleCapsuleVisAtt -> SetForceWireframe(true);
  fCapsuleLog -> SetVisAttributes( fSimpleCapsuleVisAtt);

  G4VisAttributes* fSimpleCapsuleTipVisAtt = new G4VisAttributes(red);
  fSimpleCapsuleTipVisAtt -> SetVisibility(true); 
  fSimpleCapsuleTipVisAtt -> SetForceSolid(true);
  fCapsuleTipLog -> SetVisAttributes(fSimpleCapsuleTipVisAtt);
/*
	G4NistManager* nist = G4NistManager::Instance();

	//G4Element* elFr = nist->FindOrBuildElement("Fr");
	G4Element* elCr = nist->FindOrBuildElement("Cr");
	G4Element* elFe = nist->FindOrBuildElement("Fe");
	G4Element* elNi = nist->FindOrBuildElement("Ni");
	G4Element* elMo = nist->FindOrBuildElement("Mo");
	G4Element* elMn = nist->FindOrBuildElement("Mn");

	G4double density;

	//mat SST_source
	density = 8. * g / cm3;
	G4Material* SSTMaterial = new G4Material("SST_Source", density, 5);
	SSTMaterial->AddElement(elFe, 0.63);
	SSTMaterial->AddElement(elCr, 0.19);
	SSTMaterial->AddElement(elNi, 0.12);
	SSTMaterial->AddElement(elMo, 0.3);
	SSTMaterial->AddElement(elMn, 0.3);

	//mat SST_Cable
	density = 5.75 * g / cm3;
	G4Material* cableMaterial = new G4Material("SST_Cable", density, 5);
	cableMaterial->AddElement(elFe, 0.67);
	cableMaterial->AddElement(elCr, 0.17);
	cableMaterial->AddElement(elNi, 0.11);
	cableMaterial->AddElement(elMo, 0.2);
	cableMaterial->AddElement(elMn, 0.3);

	G4Material* matAir = nist->FindOrBuildMaterial("G4_AIR");
	G4Material* matWater = nist->FindOrBuildMaterial("G4_WATER");
	G4Material* matCo = nist->FindOrBuildMaterial("G4_Co");

	//=========================================================================================
	G4Box* SBox = new G4Box("World", 10. * cm, 10. * cm, 10. * cm);
	G4LogicalVolume* LBox = new G4LogicalVolume(SBox, matAir, "World");
	G4VPhysicalVolume* PBox = new G4PVPlacement(0, G4ThreeVector(), LBox, "World", 0, false, 0, true);

	G4VisAttributes* worldvisAtt = new G4VisAttributes(true);
	LBox->SetVisAttributes(worldvisAtt);

	//==========================================================================================

	G4Box* solidPhantom = new G4Box("Phantom", 7.5 * cm, 7.5 * cm, 7.5 * cm);

	G4LogicalVolume* logicPhantom = new G4LogicalVolume(solidPhantom, matWater, "Phantom");
	new G4PVPlacement(0, G4ThreeVector(), logicPhantom, "Phantom", LBox, false, 0, true);

	G4VisAttributes* PhantomVisAtt = new G4VisAttributes();
	PhantomVisAtt->SetForceSolid(true);
	PhantomVisAtt->SetColor(135 / 255., 206 / 255., 235 / 255., 0.7);

	logicPhantom->SetVisAttributes(PhantomVisAtt);

	//==========================================================================================
		//////capsule tip

	G4Sphere* solidTip = new G4Sphere("solid_Tip", 0. * mm, 0.5 * mm, 0. * deg, 180 * deg, 0. * deg, 360 * deg);
	G4VisAttributes* TipVisAtt = new G4VisAttributes();
	TipVisAtt->SetForceSolid(true);
	TipVisAtt->SetColor(0., 0., 1., 0.7);

	G4RotationMatrix* rot = new G4RotationMatrix();
	rot->rotateX(90. * deg);
	G4LogicalVolume* logicTip = new G4LogicalVolume(solidTip, SSTMaterial, "logic_Tip");
	new G4PVPlacement(rot, G4ThreeVector(0, 0, -2. * mm), logicTip, "Phys_Tip", logicPhantom, false, 0, true);
	logicTip->SetVisAttributes(TipVisAtt);

	//------------------------------------------------------------------------------------------------------
		//////capsule main body
	G4Tubs* solidCapsule = new G4Tubs("solid_Capsule", 0. * mm, 0.5 * mm, 2. * mm, 0. * deg, 360. * deg);

	G4LogicalVolume* logicCapsule = new G4LogicalVolume(solidCapsule, SSTMaterial, "logic_Capsule");
	logicCapsule->SetVisAttributes(TipVisAtt);

	new G4PVPlacement(0, G4ThreeVector(), logicCapsule, "phys_Capsule", logicPhantom, false, 0, true);

	//=========================================================================================0.021

	  //////Air cone
	G4Cons* solidCone = new G4Cons("solid_Cone", 0. * mm, 0. * mm, 0. * mm, 0.35 * mm, 0.1051 * mm, 0. * deg, 360. * deg);

	G4VisAttributes* AirVisAtt = new G4VisAttributes();
	AirVisAtt->SetForceSolid(true);
	AirVisAtt->SetColor(1., 1., 1., 0.7);

	G4LogicalVolume* logicCone = new G4LogicalVolume(solidCone, matAir, "logic_Cone");
	logicCone->SetVisAttributes(AirVisAtt);

	new G4PVPlacement(0, G4ThreeVector(0. * mm, 0. * mm, (-1.75 - 0.1051) * mm), logicCone, "phys_Cone", logicCapsule, false, 0, true);

	//=========================================================================================
	 //////Air body  white 
	G4Tubs* solidAir = new G4Tubs("solid_Air", 0. * mm, 0.35 * mm, 1.75 * mm, 0. * deg, 360. * deg);

	G4LogicalVolume* logicAir = new G4LogicalVolume(solidAir, matAir, "logic_Air");
	logicAir->SetVisAttributes(AirVisAtt);

	new G4PVPlacement(0, G4ThreeVector(), logicAir, "phys_Air", logicCapsule, false, 0, true);


	//=========================================================================================
	   //////Co Source red one in geometry
	G4Tubs* solidCo = new G4Tubs("solid_Co", 0. * mm, 0.25 * mm, 1.75 * mm, 0. * deg, 360. * deg);

	G4VisAttributes* CoVisAtt = new G4VisAttributes();
	CoVisAtt->SetForceSolid(true);
	CoVisAtt->SetColor(1., 0., 0., 0.7);

	G4LogicalVolume* logicCo = new G4LogicalVolume(solidCo, matCo, "logic_Co");
	logicCo->SetVisAttributes(CoVisAtt);

	new G4PVPlacement(0, G4ThreeVector(), logicCo, "phys_Co", logicAir, false, 0, true);

	//=========================================================================================

*/
	return PBox;
}
