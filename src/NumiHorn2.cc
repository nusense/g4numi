#include "NumiDetectorConstruction.hh"

#include "G4Material.hh"
#include "NumiDataInput.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Torus.hh"
#include "G4Polycone.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "NumiMagneticField.hh"
#include "G4FieldManager.hh"

static const G4double in=2.54*cm;

void NumiDetectorConstruction::ConstructHorn2(G4ThreeVector hornpos, G4RotationMatrix hornrot)
{
  G4ThreeVector translation;
  G4RotationMatrix rotation;
  NumiDataInput* ND=NumiDataInput::GetNumiDataInput();
  // subtract TargetHallPosition to get origin at the face of Horn2
  G4ThreeVector TargetHallPosition=G4ThreeVector(0,0,NumiData->TargetAreaLength/2.+NumiData->TargetAreaZ0);
  //G4double MHorn2Length=133.966*2.54*cm+3.938*in+1.75*in+2.5*in;
  G4double Horn2Z0=-1.889*in;// HornZ0 in Horn coordinate system
  //G4double Horn2Z1=

  G4double OCZ0=0.295*in;
  G4double ICZ0=0.295*in;
  G4double OCZ1=135.861*in;
  G4double ICZ1=139.22*in;
  G4double Fgap=0.5*mm;
  G4double FZ0=0.*in;
  G4double FZ1=137.96*in-Fgap;

  if(ND->jCompare){
    OCZ0=0.*in;
    ICZ0=0.*in;
    OCZ1=118.11*in;
    ICZ1=118.11*in;
    FZ0=0.*in;
    FZ1=118.11*in-Fgap;

  }
  else{
    OCZ0=0.295*in;
    ICZ0=0.295*in;
    OCZ1=135.861*in;
    ICZ1=139.22*in;
    FZ0=0.*in;
    FZ1=137.96*in-Fgap;

  }


  G4double frontRmin=1.859*in;
  G4double frontRmax=2.184*in;
  G4double frontRtor=12.756*in;
  G4double MVgap=0.1*mm;

  G4ThreeVector MHorn2Origin=G4ThreeVector(0,0,0);
  G4double epsilon=.0000001*mm;
  G4double maxDev=0.5*mm;
  
  G4int nPoints=G4int((ND->PHorn2EndZ0[ND->NPHorn2EndN-1]+ND->PHorn2EndLength[ND->NPHorn2EndN-1]+frontRmax)/(.05*mm));
  G4double deltaZ=(ND->PHorn2EndZ0[ND->NPHorn2EndN-1]+frontRmax)/nPoints;
  
  G4int nOut(0),nIn(0),nMV(0),nF(0);
  G4double zPos(0);
  vdouble_t OCzPos,OCRout,OCRin, ICzPos,ICRout,ICRin;
  vdouble_t FzPos, FRin, FRout, MVzPos,MVRout,MVRin;
  
  OCzPos.push_back(OCZ0); OCRin.push_back(PHorn2OCRin(OCzPos[0],ND->jCompare)); OCRout.push_back(PHorn2OCRout(OCzPos[0],ND->jCompare));
  ICzPos.push_back(ICZ0); ICRin.push_back(PHorn2ICRin(ICzPos[0],ND->jCompare)); ICRout.push_back(PHorn2ICRout(ICzPos[0], ND->jCompare));
  FzPos.push_back(ICZ0) ; FRin.push_back(PHorn2ICRout(FzPos[0], ND->jCompare)+Fgap) ; FRout.push_back(PHorn2OCRin(FzPos[0],ND->jCompare)-Fgap);
  MVzPos.push_back(Horn2Z0-MVgap); MVRin.push_back(PHorn2ICRin(MVzPos[0], ND->jCompare)-MVgap); MVRout.push_back(PHorn2OCRout(MVzPos[0],ND->jCompare)+MVgap);

  G4double lastICzPos=ICzPos[0];
  G4double maxR,minR,endZ;
  maxR=ND->PHorn2EndRout[0]; minR=ND->PHorn2EndRin[0]; endZ=ND->PHorn2EndZ0[0]+ND->PHorn2EndLength[0];
  for (G4int ii=1;ii<ND->NPHorn2EndN;ii++){
    if (maxR<ND->PHorn2EndRout[ii]) maxR=ND->PHorn2EndRout[ii];
    if (minR<ND->PHorn2EndRin[ii]) minR=ND->PHorn2EndRin[ii];
    if (endZ<ND->PHorn2EndZ0[ii]+ND->PHorn2EndLength[ii]) endZ=ND->PHorn2EndZ0[ii]+ND->PHorn2EndLength[ii];
  }

  for (G4int ii=1;ii<nPoints;ii++){
    zPos=Horn2Z0+deltaZ*ii;
    if ((fabs(PHorn2OCRout(zPos,ND->jCompare)-(PHorn2OCRout(zPos-deltaZ,ND->jCompare)+PHorn2OCRout(zPos+deltaZ,ND->jCompare))/2.)>maxDev)||
	(fabs(PHorn2OCRin(zPos,ND->jCompare)-(PHorn2OCRin(zPos-deltaZ,ND->jCompare)+PHorn2OCRin(zPos+deltaZ,ND->jCompare))/2.)>maxDev)||
	((fabs(PHorn2OCRout(zPos,ND->jCompare)-PHorn2OCRout(zPos-deltaZ,ND->jCompare))<epsilon)&&(fabs(PHorn2OCRout(zPos,ND->jCompare)-PHorn2OCRout(zPos+deltaZ,ND->jCompare))>epsilon))||
	((fabs(PHorn2OCRout(zPos,ND->jCompare)-PHorn2OCRout(zPos-deltaZ,ND->jCompare))>epsilon)&&(fabs(PHorn2OCRout(zPos,ND->jCompare)-PHorn2OCRout(zPos+deltaZ,ND->jCompare))<epsilon)&&(zPos>OCZ0&&zPos<OCZ1))||
	((fabs(PHorn2OCRin(zPos,ND->jCompare)-PHorn2OCRin(zPos-deltaZ,ND->jCompare))<epsilon)&&(fabs(PHorn2OCRin(zPos,ND->jCompare)-PHorn2OCRin(zPos+deltaZ,ND->jCompare))>epsilon))||
	((fabs(PHorn2OCRin(zPos,ND->jCompare)-PHorn2OCRin(zPos-deltaZ,ND->jCompare))>epsilon)&&(fabs(PHorn2OCRin(zPos,ND->jCompare)-PHorn2OCRin(zPos+deltaZ,ND->jCompare))<epsilon)&&(zPos>ICZ0)))  
      { 
	if (zPos>OCZ0&&zPos<OCZ1){
	  nOut++;
	  OCzPos.push_back(zPos); OCRout.push_back(PHorn2OCRout(zPos,ND->jCompare)); OCRin.push_back(PHorn2OCRin(zPos,ND->jCompare));
	  if (zPos>=FZ0&&zPos<FZ1){
	    nF++;
	    FzPos.push_back(zPos); FRin.push_back(PHorn2ICRout(FzPos[nF], ND->jCompare)+Fgap); FRout.push_back(PHorn2OCRin(FzPos[nF],ND->jCompare)-Fgap);}
	  nIn++;
	  ICzPos.push_back(zPos); ICRout.push_back(PHorn2ICRout(zPos, ND->jCompare)); ICRin.push_back(PHorn2ICRin(zPos,ND->jCompare));
	  nMV++;
	  MVzPos.push_back(zPos); MVRout.push_back(PHorn2OCRout(zPos,ND->jCompare)+MVgap);	MVRin.push_back(PHorn2ICRin(zPos, ND->jCompare)-MVgap);
	}
	else if (zPos>=OCZ1){
	  if (zPos>=FZ0&&zPos<FZ1){
	    nF++;
	    FzPos.push_back(zPos); FRin.push_back(PHorn2ICRout(FzPos[nF],ND->jCompare)+Fgap); FRout.push_back(PHorn2OCRin(FzPos[nF],ND->jCompare)-Fgap);}
	  nMV++;	  
	  MVzPos.push_back(zPos); 
	  if (maxR>=PHorn2OCRout(zPos,ND->jCompare)) MVRout.push_back(maxR+MVgap);
	  else MVRout.push_back(PHorn2OCRout(zPos,ND->jCompare)+MVgap);
	  if (minR<=PHorn2ICRin(zPos,ND->jCompare)) MVRin.push_back(minR-MVgap);
	  else 	MVRin.push_back(PHorn2ICRin(zPos, ND->jCompare)-MVgap);
	}
      }
    if ((fabs(PHorn2ICRout((lastICzPos+zPos)/2., ND->jCompare)-(PHorn2ICRout(lastICzPos, ND->jCompare)+PHorn2ICRout(zPos, ND->jCompare))/2.)>maxDev)||
	((fabs(PHorn2ICRin((lastICzPos+zPos)/2., ND->jCompare)-(PHorn2ICRin(lastICzPos, ND->jCompare)+PHorn2ICRin(zPos, ND->jCompare))/2.)>maxDev))||
	((fabs(PHorn2ICRout(zPos, ND->jCompare)-PHorn2ICRout(zPos-deltaZ, ND->jCompare))<epsilon)&&(fabs(PHorn2ICRout(zPos, ND->jCompare)-PHorn2ICRout(zPos+deltaZ,ND->jCompare))>epsilon))||
	((fabs(PHorn2ICRout(zPos,ND->jCompare)-PHorn2ICRout(zPos-deltaZ,ND->jCompare))>epsilon)&&(fabs(PHorn2ICRout(zPos, ND->jCompare)-PHorn2ICRout(zPos+deltaZ,ND->jCompare))<epsilon)))
      {
	lastICzPos=zPos;
	if (zPos>ICZ0&&zPos<=ICZ1){
	  nIn++;
	  ICzPos.push_back(zPos); ICRout.push_back(PHorn2ICRout(zPos, ND->jCompare)); ICRin.push_back(PHorn2ICRin(zPos, ND->jCompare));
	  if (zPos>=FZ0&&zPos<FZ1){
	    nF++;
	    FzPos.push_back(zPos); FRin.push_back(PHorn2ICRout(FzPos[nF], ND->jCompare)+Fgap); FRout.push_back(PHorn2OCRin(FzPos[nF],ND->jCompare)-Fgap);}
	  nMV++;
	  MVzPos.push_back(zPos); MVRout.push_back(PHorn2OCRout(zPos,ND->jCompare)+MVgap); MVRin.push_back(PHorn2ICRin(zPos, ND->jCompare)-MVgap);
	}
	else {
	  nMV++;
	  MVzPos.push_back(zPos); MVRout.push_back(PHorn2OCRout(zPos,ND->jCompare)+MVgap); MVRin.push_back(PHorn2ICRin(zPos, ND->jCompare)-MVgap);
	}
      }
  }
  
  nF++;
  FzPos.push_back(FZ1); FRin.push_back(PHorn2ICRout(FzPos[nF], ND->jCompare)+Fgap); FRout.push_back(PHorn2OCRin(FzPos[nF],ND->jCompare)-Fgap);
  nIn++;
  ICzPos.push_back(ICZ1); ICRin.push_back(PHorn2ICRin(ICzPos[nIn], ND->jCompare));ICRout.push_back(PHorn2ICRout(ICzPos[nIn], ND->jCompare));
  nOut++;
  OCzPos.push_back(OCZ1); OCRin.push_back(PHorn2OCRin(OCzPos[nOut],ND->jCompare));OCRout.push_back(PHorn2OCRout(OCzPos[nOut],ND->jCompare));
  nMV++;
  MVzPos.push_back(endZ+MVgap);
  if (maxR>=PHorn2OCRout(zPos,ND->jCompare)) MVRout.push_back(maxR+MVgap);
  else MVRout.push_back(PHorn2OCRout(zPos,ND->jCompare)+MVgap);
  if (minR<=PHorn2ICRin(zPos, ND->jCompare)) MVRin.push_back(minR-MVgap);
  else MVRin.push_back(PHorn2ICRin(zPos, ND->jCompare)-MVgap);
    
  // Create Mother Volume
  G4VSolid* sMHorn2;
  G4Material* material=Vacuum; 
  sMHorn2= new G4Polycone("sMH",0.,360.*deg,nMV+1,&MVzPos[0],&MVRin[0],&MVRout[0]);
  G4LogicalVolume *lvMHorn2 = new G4LogicalVolume(sMHorn2,material,"lvMHorn2",0,0,0);
  ND->ApplyStepLimits(lvMHorn2); // Limit Step Size
  G4VisAttributes* invisible=new G4VisAttributes(false);
  lvMHorn2->SetVisAttributes(invisible);
  rotation=hornrot;
  translation=hornpos-TargetHallPosition;
  G4VPhysicalVolume* pvMHorn2 = new G4PVPlacement(G4Transform3D(rotation,translation),"MHorn2",lvMHorn2,TGAR,false,0);
      
  /**
   * FLUGG - Volume added to follow particles by Alex Himmel 3-21-07
   */
  G4double boxX = NumiData->TargetAreaWidth/2.;
  G4double boxY = NumiData->TargetAreaHeight/2.;
  G4double boxZ = 1*cm;
	
  G4Box *sHorn2Box = new G4Box("sHorn2Box", boxX, boxY, boxZ);
  G4Material *boxmat = TGAR->GetLogicalVolume()->GetMaterial();
  G4LogicalVolume *lvHorn2Box = new G4LogicalVolume(sHorn2Box, boxmat, "lvHorn2Box",0,0,0);
  ND->ApplyStepLimits(lvHorn2Box); // Limit Step Size
  translation += G4ThreeVector(0.,0.,(MVzPos[nMV] - MVzPos[0])/2.+.5*cm);
  new G4PVPlacement(G4Transform3D(rotation,translation),"Horn2Box",lvHorn2Box,TGAR,false,0);


  //Front part
  G4VSolid* sHorn2Front;
  if(ND->jCompare){
    sHorn2Front = new G4Tubs("sHorn2Frontgnumi",PHorn2ICRin(ICZ0,ND->jCompare),PHorn2OCRout(OCZ0,ND->jCompare),0.15*cm,0.,360.*deg);
    translation=-MHorn2Origin+G4ThreeVector(0.,0.,OCZ0-.1501*cm);

  }
  else{
  G4Torus* sFrontTorus=new G4Torus("sFrontTorus",frontRmin,frontRmax,frontRtor,0,360.*deg);
  G4Box* sBox=new G4Box("sBox",frontRtor*2.,frontRtor*2.,frontRmax*2.);
  rotation=G4RotationMatrix(0.,0.,0.);
  translation=G4ThreeVector(0.,0.,frontRmax*2);
  sHorn2Front=new G4SubtractionSolid("sHorn2Front",sFrontTorus,sBox,G4Transform3D(rotation,translation)); //need only half of torus
  translation=-MHorn2Origin+G4ThreeVector(0.,0.,OCZ0);

  }
  G4LogicalVolume* lvHorn2Front=new G4LogicalVolume(sHorn2Front,GetMaterial(ND->hrnmat),"lvHorn2Front",0,0,0);
  ND->ApplyStepLimits(lvHorn2Front); // Limit Step Size
  rotation=G4RotationMatrix(0.,0.,0.);

  new G4PVPlacement(G4Transform3D(rotation,translation),"PHorn2Front",lvHorn2Front,pvMHorn2,false,0);
     
  //Outer Conductor
  G4Polycone* sPHorn2OC=new G4Polycone("sPHorn2OC",0.,360.*deg,nOut+1,&OCzPos[0],&OCRin[0],&OCRout[0]);
  G4LogicalVolume* lvPHorn2OC=new G4LogicalVolume(sPHorn2OC,GetMaterial(ND->hrnmat),"lvPHorn2OC",0,0,0);
  ND->ApplyStepLimits(lvPHorn2OC); // Limit Step Size
  G4FieldManager* FieldMgr2 = new G4FieldManager(numiMagFieldOC); //create a local field
  FieldMgr2->SetDetectorField(numiMagFieldOC); //set the field 
  FieldMgr2->CreateChordFinder(numiMagFieldOC); //create the objects which calculate the trajectory
  lvPHorn2OC->SetFieldManager(FieldMgr2,true); //attach the local field to logical volume
  rotation=G4RotationMatrix(0.,0.,0.);
  translation=G4ThreeVector(0.,0.,0.)-MHorn2Origin;
  new G4PVPlacement(G4Transform3D(rotation,translation),"PHorn2OC",lvPHorn2OC,pvMHorn2,false,0);

  //Inner Conductor
  G4Polycone* sPHorn2IC=new G4Polycone("sPHorn2IC",0.,360.*deg,nIn+1,&ICzPos[0],&ICRin[0],&ICRout[0]);
  G4LogicalVolume* lvPHorn2IC=new G4LogicalVolume(sPHorn2IC,GetMaterial(ND->hrnmat),"lvPHorn2IC",0,0,0);
  ND->ApplyStepLimits(lvPHorn2IC); // Limit Step Size
  G4FieldManager* FieldMgr = new G4FieldManager(numiMagFieldIC); //create a local field		 
  FieldMgr->SetDetectorField(numiMagFieldIC); //set the field 
  FieldMgr->CreateChordFinder(numiMagFieldIC); //create the objects which calculate the trajectory
  lvPHorn2IC->SetFieldManager(FieldMgr,true); //attach the local field to logical volume
  rotation=G4RotationMatrix(0.,0.,0.);
  translation=G4ThreeVector(0.,0.,0)-MHorn2Origin;
  new G4PVPlacement(G4Transform3D(rotation,translation),"PHorn2IC",lvPHorn2IC,pvMHorn2,false,0);
      
  //Field Part
  G4VSolid* sPHorn2F;
  if(ND->jCompare){
    sPHorn2F=(G4VSolid*) new G4Polycone("sPHorn2F",0.,360.*deg,nF+1,&FzPos[0],&FRin[0],&FRout[0]);
  }
  else{
  G4Polycone* sPConeF=new G4Polycone("sPCone2F",0.,360.*deg,nF+1,&FzPos[0],&FRin[0],&FRout[0]);
  G4Torus* sTorusF=new G4Torus("sTorusF",0.,frontRmin-Fgap,frontRtor,0,360.*deg);
  rotation=G4RotationMatrix(0.,0.,0.); translation =G4ThreeVector(0.,0.,Horn2Z0+frontRmax);
  sPHorn2F=new G4UnionSolid("sPHorn2F",sPConeF,sTorusF,G4Transform3D(rotation,translation));
  }
  G4LogicalVolume* lvPHorn2F=new G4LogicalVolume(sPHorn2F,Air,"lvPHorn2F",0,0,0);
  ND->ApplyStepLimits(lvPHorn2F); // Limit Step Size
  lvPHorn2F->SetVisAttributes(invisible);
  //------ Modification by Alex Himmel 3-19-07----------
  lvPHorn2F->SetOptimisation(false);
  //------ End of Modification -------------------------
  
  G4FieldManager* FieldMgr3 = new G4FieldManager(numiMagField); //create a local field      
  FieldMgr3->SetDetectorField(numiMagField); //set the field 
  FieldMgr3->CreateChordFinder(numiMagField); //create the objects which calculate the trajectory 
  lvPHorn2F->SetFieldManager(FieldMgr3,true); //attach the local field to logical volume
  rotation=G4RotationMatrix(0.,0.,0.);
  translation=G4ThreeVector(0.,0.,0)-MHorn2Origin;
  G4VPhysicalVolume *pvPHorn2F=new G4PVPlacement(G4Transform3D(rotation,translation),"PHorn2F",lvPHorn2F,pvMHorn2,false,0);


  //Spider support
  if(!ND->jCompare){
    //no spider constructionf or gnumi like horns
    for (G4int ii=0;ii<G4int(ND->Horn2SS.size());ii++){
      for (G4int jj=0;jj<ND->NHorn2SpidersPerPlaneN;jj++){
	G4double angle=G4double(360.*deg*jj/ND->NHorn2SpidersPerPlaneN);
	G4double rIn=PHorn2ICRout(ND->Horn2SpiderSupportZ0[ii], ND->jCompare)+Fgap;//
	G4double rOut=PHorn2OCRin(ND->Horn2SpiderSupportZ0[ii],ND->jCompare)-Fgap;//In and out radius of mother vol.
	ConstructSpiderSupport(&(ND->Horn2SS[ii]),angle,ND->Horn2SpiderSupportZ0[ii],rIn,rOut,pvPHorn2F,ii+jj);
      }
    }
  }
  //Horn end
  if(!ND->jCompare){
    G4VSolid* sPHorn2End;
    for (G4int ii=0;ii<ND->NPHorn2EndN;ii++)
      {
	G4String volName=ND->PHorn2EndVolName[ii];
	sPHorn2End=new G4Tubs(volName.append("s"),ND->PHorn2EndRin[ii],ND->PHorn2EndRout[ii],ND->PHorn2EndLength[ii]/2.,0.,360.*deg);
	G4LogicalVolume* lvPHorn2End=new G4LogicalVolume(sPHorn2End,GetMaterial(ND->PHorn2EndGeantMat[ii]),volName.append("lv"),0,0,0);
	ND->ApplyStepLimits(lvPHorn2End); // Limit Step Size
	rotation=G4RotationMatrix(0.,0.,0.);
	translation=G4ThreeVector(0.,0.,ND->PHorn2EndZ0[ii]+ND->PHorn2EndLength[ii]/2.)-MHorn2Origin;
	new G4PVPlacement(G4Transform3D(rotation,translation),volName,lvPHorn2End,pvMHorn2,false,0);
      }
  }
  else{
    G4VSolid* sPHorn2End;
    G4String volName = "PHorn2gnumiEnd";
    sPHorn2End=new G4Tubs(volName.append("s"),PHorn2ICRin(ICZ1,ND->jCompare),PHorn2OCRout(OCZ1,ND->jCompare),0.15*cm,0.,360.*deg);
    G4LogicalVolume* lvPHorn2End=new G4LogicalVolume(sPHorn2End,GetMaterial(ND->PHorn2EndGeantMat[2]),volName.append("lv"),0,0,0);
    ND->ApplyStepLimits(lvPHorn2End); // Limit Step Size
    rotation=G4RotationMatrix(0.,0.,0.);
    translation=G4ThreeVector(0.,0.,OCZ1+0.1501*cm)-MHorn2Origin;
    new G4PVPlacement(G4Transform3D(rotation,translation),volName,lvPHorn2End,pvMHorn2,false,0);

  }

}
G4double NumiDetectorConstruction::PHorn2OCRout(G4double z, G4bool dognumi)
{

  if(dognumi){
    return 14.909*in; // from gnumi files
  }
  G4double r=0;
   if (z<0.295*in){
     r=14.94*in; // for mother vol.
  }

   //OC dimensions from drawings
   else if ((z>=0.295*in)&&(z<0.97*in)){
    r=14.94*in;
  }
  else if ((z>=0.97*in)&&(z<3.26*in)){
    r=16.75*in;
  }
  else if ((z>=3.26*in)&&(z<125.34*in)){
    r=15.567*in;
  }
  else if ((z>=125.34*in)&&(z<126.212*in)){
    r=(15.567+(z/in-125.34)/(126.212-125.34)*(16.5-15.567))*in;
  }
  else if ((z>=126.212*in)&&(z<133.455*in)){
    r=16.5*in;
  }
  else if ((z>=133.455*in)&&(z<134.955*in)){
    r=18.5*in;
  }
   else if ((z>=134.955*in)&&(z<135.861*in)){
     r=14.41*in;
   }
   
   //for mother vol. purposes
   else if ((z>=135.861*in)&&(z<137.611*in)){
     r=14.405*in; // for mother vol.
   }
   else if ((z>=137.611*in)){
     r=14.469*in; // for mother vol.
   }
   return r;
}
G4double NumiDetectorConstruction::PHorn2OCRin(G4double z, G4bool dognumi)
{
  G4double r=0;
  if(dognumi){
    return 14.5669*in;//as in gnumi
  }
  if (z<0.295*in){
    r=14.615*in; // for mother vol.
  }
  
  //OC dimensions from drawings
  else if ((z>=0.295*in)&&(z<1.97*in)){
    r=14.615*in;
  }
  else if ((z>=1.97*in)&&(z<125.34*in)){
    r=14.567*in;
  }
  else if ((z>=125.34*in)&&(z<126.773*in)){
    r=(14.567+(z/in-125.34)/(126.773-125.34)*(16.-14.567))*in;
  }
  else if ((z>=126.773*in)&&(z<132.111*in)){
    r=16.*in;
  }
  else if ((z>=132.111*in)&&(z<133.922*in)){
    r=(16.-(z/in-132.111)/(133.922-132.111)*(16.-12.719))*in;
  }

  else if ((z>=133.922*in)&&(z<137.611*in)){
    r=12.532*in;
    //r=12.719*in; // for mother vol. & for field vol
  }
  else if ((z>=137.611*in)&&(z<139.486*in)){
    r=12.532*in; // for mother vol. & for field vol
  }
  else if (z>=139.486*in){
    r=12.532*in; // for mother vol. & for field vol
  }
  return r;
}
G4double NumiDetectorConstruction::PHorn2ICRout(G4double z, G4bool dognumi)
{
  G4double r=0.;
  //  NumiDataInput* ND=NumiDataInput::GetNumiDataInput();
  if (z<-0.00331*in){
    r=(10.9108+(0.1780*0.00331))*in; //for MV
  }
  else if(z/cm<118.156*in){
    
    if(dognumi){ // here I added some functionality "gnumi-like horns" resets parabolic shape
      //===========================================
      // These are the equations i gnumi used by me (jasmine ma) commented here
      // because no one else uses them  
      
      if((z/cm)>=-0.00840   && (z/cm)< 97.617) r= (sqrt((100-(z/cm))/0.1351))*cm;
      else if( (z/cm)>=97.617 && (z/cm)<104.803)  r = 4.2*cm;
      else if((z/cm)>=104.803 && (z/cm)<118.156*in)   r= (sqrt(((z/cm)-100)/0.2723))*cm ;
      //===========================================
    }
    else{
    
      //IC from drawings
      
      if (z<-0.00331*in){
	r=(10.9108+(0.1780*0.00331))*in; //for MV
      }
      else if ((z>=-0.00331*in)&&(z<5.862*in)){
	r=(10.9108-(0.1780)*(z/in))*in;
      }
      //else if ((z>=5.3874*in)&&(z<5.8620*in)){
      //  r=9.9675*in; //! this is not from eq.
      // }
      //else if ((z>=5.862*in)&&(z<6.3629*in)){
      //  r=(9.9675-(z/in-5.8622)/(6.3629-5.862)*(0.15997))*in; //! this is not from eq.
      //}
      else if ((z>=5.862*in)&&(z<30.3611*in)){
	r=sqrt(114.73006-(2.91414)*(z/in))*in;
      }
      //else if ((z>=29.4686*in)&&(z<30.3611*in)){
      //  r=sqrt(114.73006-(2.91414)*(z/in))*in+1.*mm; //! this is not from eq.
      //}
      else if ((z>=30.3611*in)&&(z<36.8888*in)){
	r=sqrt(114.73006-(2.91414)*(z/in))*in;
      }   
      else if ((z>=36.8888*in)&&(z<38.2521*in)){
	r=(22.68402-(0.54203)*(z/in))*in;
      }
      // I think I got this right
      else if ((z>=38.2521*in)&&(z<39.1092*in)){
	r=(1.7325+(1.8-sqrt(1.8*1.8-sqr(z/in-39.1092))))*in; //! this is not from eq. ! neck
      }
      else if ((z>=39.1092*in)&&(z<40.8688*in)){
	r=1.7325*in; //! this is not from eq. ! neck
      }
      else if ((z>=40.8688*in)&&(z<41.3864*in)){
	r=(1.7325+(1.8-sqrt(1.8*1.8-sqr(z/in-40.8688))))*in; //! this is not from eq. ! neck
      }
      
      else if ((z>=41.3864*in)&&(z<43.3660*in)){
	r=(-10.63139+(0.30058)*(z/in))*in;
      } 
      else if ((z>=43.3660*in)&&(z<50.2725*in)){
	r=sqrt(-56.922631+(1.44583)*(z/in))*in;
      }
      
      //else if ((z>=49.3279*in)&&(z<50.2725*in)){
      //  r=sqrt(-56.922631+(1.44583)*(z/in))*in;// joint , need to correct this
      //}
      
      else if ((z>=50.2725*in)&&(z<69.892*in)){
	r=sqrt(-56.922631+(1.44583)*(z/in))*in;
      }
      
      //else if ((z>=69.3337*in)&&(z<69.8920*in)){
      //  r=sqrt(-56.922631+(1.44583)*(z/in))*in;// joint , need to correct this
      //}
      //else if ((z>=69.8920*in)&&(z<70.3095*in)){
      //  r=(0.12835+(0.0932)*(z/in))*in;// joint , need to correct this
      //}

      else if ((z>=69.8920*in)&&(z<93.892*in)){
	r=(0.12835+(0.0932)*(z/in))*in;
      }
      
      //else if ((z>=93.3359*in)&&(z<93.8920*in)){
      //  r=(0.12835+(0.0932)*(z/in))*in;// joint , need to correct this
      //}
      //else if ((z>=93.8920*in)&&(z<=94.3159*in)){
      //  r=(1.93227+(0.07398)*(z/in))*in;// joint , need to correct this
      //}
      
      else if ((z>=93.892*in)&&(z<=118.156*in)){
	r=(1.93227+(0.07398)*(z/in))*in;
      }
    
  
  //else if ((z>=117.6060*in)&&(z<118.156*in)){
  //  r=(1.93227+(0.07398)*(z/in))*in;// joint , need to correct this
  //}
  //Flange inner downstream
    }
  }
  else if ((z>=118.156*in)&&(z<=121.388*in)){
    r=21.498/2.*in+(z/in-118.156)/(121.388-118.156)*(21.853-21.498)/2.;
  }
  else if ((z>=121.388*in)&&(z<137.96*in)){
    r=21.853/2.*in;
  }
  else if ((z>=137.96*in)&&(z<139.22*in)){
    r=24.706/2.*in;
  }

  //for MV
  else if (z>=139.22*in){
    r=24.706/2.*in;
  }
  
 
return r;
}
G4double NumiDetectorConstruction::PHorn2ICRin(G4double z, G4bool dognumi)
{
  G4double r=0.;
  
  //  NumiDataInput* ND=NumiDataInput::GetNumiDataInput();
  if (z<0.0*in){
    r=sqrt(114.73006)*in-0.11811*in-0.05*in;//for MV; added 0.05 because ic was outside of mv
  }
  else if (z<118.156*in){
    if(dognumi){ // here I added some functionality - "gnumi like horns" resets the parabolic shape of horn 2
      if(z/cm>=0.0 && (z/cm)< 97.617)  r= (sqrt((100-(z/cm))/0.1351)-0.2)*cm;
      else if( (z/cm)>=97.617 && (z/cm)<104.803)  r = 4*cm;
      else if((z/cm)>=104.803 && (z/cm)<118.156*in) r= (sqrt(((z/cm)-100)/0.2723)-0.2)*cm ;
    }
    else{    
      
      //---------------------------------------------------------
      // This stuff is originally what zarko had in here from drawings
      //IC from drawings
      if ((z>=0.0*in)&&(z<5.8*in)){
	r=sqrt(114.73006-(2.91414)*(z/in))*in-0.11811*in;
      }
      else if ((z>=5.8*in)&&(z<29.8300*in)){
	r=sqrt(114.73007-(2.91414)*(z/in))*in-0.11811*in;
      }
      else if ((z>=29.8300*in)&&(z<37.9435*in)){
	r=sqrt(114.73006-(2.91414)*(z/in))*in-0.11811*in;
      }         
      else if ((z>=37.9435*in)&&(z<39.12373*in)){
	r=(1.5355+(2.-sqrt(4.-sqr(z/in-39.12373))))*in; //! NECK !
      }  
      else if ((z>=39.12373*in)&&(z<40.86009*in)){
	r=1.5355*in; //! NECK !
      }  
      else if ((z>=40.86009*in)&&(z<41.6144*in)){
	r=(1.5355+(2.-sqrt(4.-sqr(z/in-40.86009))))*in; //! NECK !
      }  
      
      else if ((z>=41.6144*in)&&(z<49.8920*in)){
	r=sqrt(-56.92263+(1.44583)*(z/in))*in-0.11811*in;
      } 
      else if ((z>=49.8920*in)&&(z<69.8920*in)){
	r=sqrt(-56.922631+(1.44583)*(z/in))*in-0.11811*in;
      } 
      else if ((z>=69.8920*in)&&(z<93.8920*in)){	
	r=(0.01064+(0.0932)*(z/in))*in;
      }
      else if ((z>=93.8920*in)&&(z<118.156*in)){
	r=(1.81456+(0.07398)*(z/in))*in;
      }
    } // not jCompare
  }  // if z<118.156
  //-----differences in g4numi/gnumi end here -----------
  //Flange inner downstream
  else if ((z>=118.156*in)&&(z<139.21*in)){
      r=21.342/2.*in;
  }

  //for mother volume i need r defined up to the end of horn
  else if (z>=139.21*in){
    r=21.342/2.*in;
  }
  return r;
}
