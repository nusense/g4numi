//
// ********************************************************************

#ifndef NA49DetectorConstruction_h
#define NA49DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4Material.hh"

class NA49CheckVolumeSD;
class NA49TargetSD;
class G4LogicalVolume;
class NA49DetectorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class NA49DetectorConstruction : public G4VUserDetectorConstruction
{
public:

  NA49DetectorConstruction();
  virtual ~NA49DetectorConstruction();

public:

  G4VPhysicalVolume* Construct();

  void SetWorldMaterial(const G4String&);
  void SetTargetMaterial(const G4String&);

  void SetTargetRadius(G4double val);
  void SetTargetLenght(G4double val);

  void UpdateGeometry();

private:

  NA49DetectorConstruction & operator=(const NA49DetectorConstruction &right);
  NA49DetectorConstruction(const NA49DetectorConstruction&);

  G4double radius;
  G4double targetZ;

  G4Material*  targetMaterial;
  G4Material*  worldMaterial;

  NA49CheckVolumeSD*   checkSD;
  NA49TargetSD*        targetSD;

  G4LogicalVolume* logicTarget;
  G4LogicalVolume* logicCheck;
  G4LogicalVolume* logicWorld;

  NA49DetectorMessenger* detectorMessenger;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

#endif

