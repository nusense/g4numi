// NumiDetectorConstruction.hh
//------------------------------------------------------------

#ifndef NumiDetectorConstruction_H
#define NumiDetectorConstruction_H 1

#include "G4VUserDetectorConstruction.hh"
#include "NumiDetectorMessenger.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include <vector>

class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class NumiDataInput;
class NumiMagneticField;
class NumiMagneticFieldIC;
class NumiMagneticFieldOC;
class NumiHornSpiderSupport;
class G4VisAttributes;

typedef std::vector<G4double> vdouble_t; 
typedef std::vector<G4int> vint_t;

class NumiDetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    NumiDetectorConstruction();
    ~NumiDetectorConstruction();

  G4VPhysicalVolume* Construct();

	void SetTargetZ0(G4double val);
	void SetHornCurrent(G4double val);
	void UpdateGeometry();
	
  private:

  NumiDataInput* NumiData;
  NumiMagneticField* numiMagField;
  NumiMagneticFieldIC* numiMagFieldIC;
  NumiMagneticFieldOC* numiMagFieldOC;
  
  void ConstructTargetHall();
  void ConstructTarget();
  void ConstructBaffle();
  void ConstructDecayPipe();
  void ConstructHadronAbsorber();  
  void ConstructHorns();  
  void ConstructHorn1();
  void ConstructSpiderSupport(NumiHornSpiderSupport *HSS,
			      G4double angle,
			      G4double zPos,
			      G4double rIn,
			      G4double rOut,
			      G4VPhysicalVolume *motherVolume,
			      G4int copyNo); 
  void ConstructHorn2();
  void ConstructSecMonitors();
  void DefineMaterials();
  G4VisAttributes* GetMaterialVisAttrib(G4int matCode);
  G4VisAttributes* GetMaterialVisAttrib(G4String matName);
  void DestroyMaterials();

	// Messenger
	NumiDetectorMessenger *detectorMessenger;
	
  G4VPhysicalVolume* GetPhysicalVolume(G4String PVname);
  G4Material* GetMaterial(G4int matcode);
  G4double phornRgivenZ(G4double a, G4double b, G4double c, G4double z);
  G4double PHorn2ICRin(G4double z);
  G4double PHorn2ICRout(G4double z);
  G4double PHorn2OCRin(G4double z);
  G4double PHorn2OCRout(G4double z);
  G4double PHorn1ICRin(G4double z);
  G4double PHorn1ICRout(G4double z);
  G4double PHorn1OCRin(G4double z);
  G4double PHorn1OCRout(G4double z);

  // Materials
  G4Material* Vacuum;
  G4Material* DecayPipeVacuum;
  G4Material* Air;
  G4Material* Water;
  G4Material* He;
  G4Material* Be;
  G4Material* C;
  G4Material* Al;
  G4Material* Ar;
  G4Material* Pb;
  G4Material* Fe;
  G4Material* CT852;
  G4Material* Concrete;
  G4Material* Target;
  G4Material* DolomiteRock;

    // Logical volumes
    //
    G4LogicalVolume* ROCK_log;
    G4LogicalVolume* TRGT_lv;
  // G4LogicalVolume* lvTUNE;
    G4LogicalVolume* BLK_log[20]; 
    G4LogicalVolume* TGAR_log;
    G4LogicalVolume* Horn_PM_lv[8];
    G4LogicalVolume* LVCPipe[20];
    G4LogicalVolume* LVCPipeW[20];

    // Physical volumes
    //
    G4VPhysicalVolume* ROCK;
    G4VPhysicalVolume* pvTUNE;
    G4VPhysicalVolume* TGAR;
    G4VPhysicalVolume* TRGT;
    G4VPhysicalVolume* PHORN[8];
    G4VPhysicalVolume* PVCPipe[20];
    G4VPhysicalVolume* CNT[20];

    //Solids
    //
    G4VSolid* BLK_solid[20];
    G4VSolid* Horn_PM[8];
};

#endif

