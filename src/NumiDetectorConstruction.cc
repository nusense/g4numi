
#include "NumiDetectorConstruction.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trap.hh"
#include "G4Cons.hh"
#include "G4Torus.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4PVReplica.hh"
#include "G4AssemblyVolume.hh"
#include "NumiMagneticField.hh"
#include "G4Transform3D.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"

#include "G4RegionStore.hh"
#include "G4SolidStore.hh"
#include "G4GeometryManager.hh"
#include "G4RunManager.hh"


NumiDetectorConstruction::NumiDetectorConstruction()
{
  //Scan the input file     
  NumiData=new NumiDataInput();

  // Pointers for magnetic fields ***    
  numiMagField = new NumiMagneticField(); 
  numiMagFieldIC = new NumiMagneticFieldIC();
  numiMagFieldOC = new NumiMagneticFieldOC();
  
  // create commands for interactive definition of the geometry
  detectorMessenger = new NumiDetectorMessenger(this);
  DefineMaterials();
}

NumiDetectorConstruction::~NumiDetectorConstruction()
{
  delete numiMagField; 
  delete numiMagFieldIC;
  delete numiMagFieldOC; 
  delete NumiData;
  DestroyMaterials();
  delete detectorMessenger;
}

G4VPhysicalVolume* NumiDetectorConstruction::Construct()
{

   //Define world volume 
  G4Tubs* ROCK_solid = new G4Tubs("ROCK_solid",0.,NumiData->RockRadius,NumiData->RockHalfLen,0,360.*deg);
  ROCK_log = new G4LogicalVolume(ROCK_solid,Concrete,"ROCK_log",0,0,0); 
  ROCK_log->SetVisAttributes(G4VisAttributes::Invisible);
  ROCK = new G4PVPlacement(0,G4ThreeVector(),ROCK_log,"ROCK",0,false,0);
 
  ConstructTargetHall();
  ConstructDecayPipe();
  ConstructBaffle();
  ConstructTarget();
  ConstructHorn1();
  ConstructHorn2();
  ConstructHadronAbsorber(); 
  ConstructSecMonitors();

  //Set Vis Attributes according to solid material (only for volumes not explicitly set)
  G4LogicalVolumeStore* lvStore=G4LogicalVolumeStore::GetInstance();
  lvStore=G4LogicalVolumeStore::GetInstance();

  for (size_t ii=0;ii<(*lvStore).size();ii++){   
    if ((*lvStore)[ii]->GetVisAttributes()==0) {
      G4String matName=(*lvStore)[ii]->GetMaterial()->GetName();
      (*lvStore)[ii]->SetVisAttributes(GetMaterialVisAttrib(matName));
    }
  }

  return ROCK;
}
 
G4VPhysicalVolume* NumiDetectorConstruction::GetPhysicalVolume(G4String PVname)
{
  G4PhysicalVolumeStore* PVStore=G4PhysicalVolumeStore::GetInstance();
  for (size_t ii=0;ii<(*PVStore).size();ii++){
    if ((*PVStore)[ii]->GetName()==PVname) return (*PVStore)[ii];
  }
  G4cout << "NumiDetectorConstruction: Volume "<<PVname<< " is not in Physical Volume Store"<<G4endl;
  return 0;
}
/*
void NumiDetectorConstruction::SetTargetGas(G4String gasChoice) {

	G4Material* pttoMaterial = G4Material::GetMaterial(gasChoice);
	if (pttoMaterial)
*/

void NumiDetectorConstruction::SetTargetZ0(G4double val) {

	NumiData->SetTargetZ0(val);
	G4cout << " Geometry changed: target position Z0 = " << NumiData->TargetZ0/cm << " cm"<<G4endl;
	G4cout << " Geometry changed: baffle position Z0 = " << NumiData->HPBaffleZ0/cm << " cm"<<G4endl;
	
}

void NumiDetectorConstruction::SetHornCurrent(G4double val) {

	NumiData->SetHornCurrent(val);
	G4cout << " Geometry changed: Horn current = " << NumiData->HornCurrent/ampere<<" A"<< G4endl;

}

void NumiDetectorConstruction::UpdateGeometry() {


  // Clean old geometry
  //
  G4GeometryManager::GetInstance()->OpenGeometry();
  //G4RegionStore::GetInstance()->Clean();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
  
}
