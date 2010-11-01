//----------------------------------------------------------------------
//$Id: NumiBaffle.cc,v 1.3.4.2 2010/11/01 21:51:36 minervacvs Exp $
//----------------------------------------------------------------------


#include "NumiDetectorConstruction.hh"

#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "NumiDataInput.hh"

void NumiDetectorConstruction::ConstructBaffle()

{ 
  G4ThreeVector targetHallPosition=G4ThreeVector(0,0,NumiData->TargetAreaLength/2.+NumiData->TargetAreaZ0);

 // Baffle 
  G4double HPBRin=NumiData->HPBaffleRin;
  G4double HPBRout=NumiData->HPBaffleRout;
  G4double HPBlength=NumiData->HPBaffleLength;
  G4Tubs* sBaffle=new G4Tubs("sBaffle",HPBRin,HPBRout,HPBlength/2.,0.,360.*deg);
  G4LogicalVolume* lvBaffle=new G4LogicalVolume(sBaffle,Target,"lvBaffle",0,0,0);
  G4ThreeVector bafflePos=G4ThreeVector(NumiData->HPBaffleX0,NumiData->HPBaffleY0,NumiData->HPBaffleZ0+HPBlength/2.)-targetHallPosition;
  G4PVPlacement* pvbaffle = new G4PVPlacement(0,bafflePos,"pvBaffleMother",lvBaffle,TGAR,false,0);
  pvbaffle -> CheckOverlaps();

  if(NumiData->GetPrintGeometry())
  {
     G4cout << "Z0   Position of baffle = " << (bafflePos.z() - (HPBlength/2.) )/m << " m " << G4endl;
     G4cout << "Z    Position of baffle = " << (bafflePos.z())/m << " m " << G4endl;
     G4cout << "ZEnd Position of baffle = " << (bafflePos.z() + (HPBlength/2.) )/m << " m " << G4endl;
  }
}
