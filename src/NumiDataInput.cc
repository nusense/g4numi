//----------------------------------------------------------------------
//
//
// $Id: NumiDataInput.cc,v 1.29 2009/04/15 18:07:18 jyuko Exp $
//----------------------------------------------------------------------

#include "NumiDataInput.hh"
#include "G4ThreeVector.hh"
#include "NumiHornSpiderSupport.hh"
#include "G4UserLimits.hh"
//#include "globals.hh"
#include <math.h>

static const G4double in=2.54*cm;

NumiDataInput* NumiDataInput::fNumiDataInput = 0;

NumiDataInput* NumiDataInput::GetNumiDataInput(){
    G4cout << "Requesting NumiDataInput " << G4endl;
    if (!fNumiDataInput) {
        G4cout << "Constructing NumiDataInput " << G4endl;
        fNumiDataInput = new NumiDataInput();    
    }
    return fNumiDataInput;
}

NumiDataInput::NumiDataInput()
{
  G4cout << "NumiDataInput Constructor Called" << G4endl;
  if (fNumiDataInput)
    { G4Exception("NumiDataInput constructed twice.");}
//  fNumiDataInput = this;

  debugOn = false;
  NImpWeightOn = true; 
  createNuNtuple=false;  createHadmmNtuple=false;
  createASCII=false;     createBXDRAW = false;
  useFlukaInput = false; useMarsInput=false;

  //
  //can set useMuonInupt and useMuonBeam from macro
  //
  useMuonInput = false;  useMuonBeam = false; fNEvents = -1;
  useTestBeam = false;   
  useDecayPipeSelect = false;
  KillTracking = true; // false for ahimmel
  testTheta = M_PI/6.;
  
   StepLimit = 0.0; 

  extNtupleFileName=""; //fluka or mars or muon ntuple with particles coming of the target
  //Set the energy threshold for 'killing' particles
   KillTrackingThreshold = 0.05*GeV; //for defaut neutrino MC g4numi 
   //KillTrackingThreshold = 0.001*GeV; //for muon beam MC


   //base name for output files:
   nuNtupleName    = "nuNtuple";
   //
   //can set hadmmNtupleDir and hadmmNtupleName from macro
   //
   hadmmNtupleDir  = "./";
   hadmmNtupleName = "";
   asciiName       = "asciiOut";
   bxdrawName      = "bxdrawOut";
   RunNumber       = "0000";
   geometry        = "_ghorns6";

   //
   // Denotes the change in sigma to the
   // rock density and muon alcove wall location
   //
   materialSigma   = 0;
   

   //===================================
   //------ Use Macro---------------
   // useMacro = true if you actually want to change beam parameters
   // from the macro 
   
   useMacro = false;

  //====================================
  //-- Changes for "Air Horns"---------- >> G4numi validation
  //-- Changes for "Truncated Horns"-----
  airhrn =false; // airhrn must be changed before compilation
  vacuumworld=false;
  jCompare =true; // make horns have the same B field;
  
if(!vacuumworld && !airhrn){
  hrnmat = 9;
  hrnmatcr =31;
  hallmat=15;
}
 else {
   if(airhrn){
     hrnmat=15;
     hrnmatcr=15;
     hallmat=15;
    }
   if(vacuumworld){
     hrnmat=16;
     hrnmatcr=16;
     hallmat=16;
   }
 }
 
//======================================
 
//======================================
//--Ray Tracing parameter-------------
//--------------------------------------
 raytracing =false;
 //==============================
 //------------------------------
 
 //initialize zpoints
 NZpoint=48;
 Zpoint.push_back(0*m); //1
 Zpoint.push_back(0.5*m); //2
 Zpoint.push_back(0.7*m); //3
 Zpoint.push_back(0.75*m); //4
 Zpoint.push_back(0.8*m); //5
 Zpoint.push_back(0.85*m); //6
 Zpoint.push_back(0.9*m); //7
 Zpoint.push_back(1*m); //8
 Zpoint.push_back(1.25*m); //9
 Zpoint.push_back(1.5*m); //10
 Zpoint.push_back(1.75*m); //11
 Zpoint.push_back(2*m);//12
 Zpoint.push_back(2.25*m);//13
 Zpoint.push_back(2.5*m);//14
 Zpoint.push_back(2.75*m); //15
 Zpoint.push_back(3*m);//16
 Zpoint.push_back(3.25*m);//17
 Zpoint.push_back(3.5*m);//18
 Zpoint.push_back(4*m);//19
 Zpoint.push_back(6*m);//20
 Zpoint.push_back(9*m);//21
 Zpoint.push_back(9.5*m); //22
 Zpoint.push_back(9.25*m); //23
 Zpoint.push_back(10*m);//24
 Zpoint.push_back(10.25*m);//25
 Zpoint.push_back(10.5*m);//26
 Zpoint.push_back(10.75*m);//27
 Zpoint.push_back(11.80*m);//28
 Zpoint.push_back(11.9*m);//29
 Zpoint.push_back(11*m);//30
 Zpoint.push_back(11.1*m);//31
 Zpoint.push_back(11.2*m);//32
 Zpoint.push_back(11.25*m);//33
 Zpoint.push_back(11.5*m);//34
 Zpoint.push_back(11.75*m);//35
 Zpoint.push_back(12*m);//36
 Zpoint.push_back(12.25*m);//37
 Zpoint.push_back(12.5*m);//38
 Zpoint.push_back(12.75*m);//39
 Zpoint.push_back(13*m);//40
 Zpoint.push_back(13.25*m);//41
 Zpoint.push_back(13.5*m);//42
 Zpoint.push_back(13.75*m);//43
 Zpoint.push_back(14*m);//43
 Zpoint.push_back(14.5*m);//44
 Zpoint.push_back(15.*m);//45
 Zpoint.push_back(16.*m);//46
 Zpoint.push_back(20*m);//47
 Zpoint.push_back(45.7*m);//48
 createZpNtuple=false;
 zpNtupleName="zpNtuple";
 //==============================================================
  G4float beam_x_dir = 0;
  G4float beam_y_dir = 0;
  G4float beam_z_dir = 1;//cos(.01*pi/180);
  //actual dm is 5/13e-4 radians 
  G4float beam_x_pos = 0;
  G4float beam_y_pos = 0;
  G4float beam_z_pos = -4.0*m;
  // the reason for the beam_z_pos change was to move the beam to start
  // immediately before the target so that the beam spot interaction point
  // would remain constant, but the angle would change.

  protonMomentum = 120.*GeV;  
  beamSigmaY     = 0;//1.25*mm;
  beamSigmaX     = 0;//1.1*mm;
  beamDirection  = G4ThreeVector(beam_x_dir,beam_y_dir,beam_z_dir);
  beamPosition  = G4ThreeVector(beam_x_pos,beam_y_pos,beam_z_pos);

  protonKineticEnergy = sqrt(pow((.938*GeV),2)+pow(protonMomentum,2))-0.938*GeV;

    //Rock        1
  //=======================================================================
  RockRadius  = 10.0*m;
  RockHalfLen = 1200.0*m;
  RockDensity = 2.41*g/cm3; // not
  RockRadLen  = 0.0;        // used

  constructTarget = true;
  //TargetArea          1
  //=======================================================================
  TargetAreaZ0       = -6.7*m;//was -4.0*m (08/09/05);
  TargetAreaLength   = 52.398*m;//was 49.28*m (08/09/05);

  // TargetAreaHeight and TargetAreaWidth were 6.0 meters in Zarko's older version, 
  // but I had to extend them by 1 m to fit the concrete chase in TGAR 
  // (working from dimensions and placement given in the Numi Technical Design Handbook)
  // I (Zarko) had to add 1.5m more to fit all blocks
  TargetAreaHeight   = 8.5*m;
  TargetAreaWidth    = 8.5*m;
  TargetAreaGEANTmat = 15;

  // Target   1
  //=======================================================================
  TargetX0           = 0.0;
  TargetY0           = -1.1*mm;
  TargetZ0           = -0.45*m; //-0.35*m-10.*cm;
  TargetDxdz         = 0.0; // doesn't
  TargetDydz         = 0.0; // work properly yet
  TargetSLength      = 20.*mm;
  TargetSWidth       = 6.40E-03*m;
  TargetSHeight      = 18.0E-03*m;
  TargetCPGRadius    = 3.2*mm; // Cooling pipe groove
  TargetCPGPosition  = 10.7*mm;
  TargetEndRounded   = true;
  TargetSegmentNo    = 47;
  TargetSegmentPitch = 0.3*mm;
  TargetA            = 12.01*g/mole;
  TargetZ            = 6.;
  TargetDensity      = 1.78*g/cm3; //1.815*g/cm3;//1.754*g/cm3;
  TargetRL           = 25.692;
  TargetGEANTmat     = 18;

  //Budal Monitor
  BudalX0 = 0.0;
  BudalY0 = 2.26*mm;
  BudalZ0 = -16.72*cm;
  BudalDxdz = 0.0;
  BudalDydz = 0.0;
  
  //HPBaffle           1 // Only the length and position can be changed currently
  //=======================================================================
  HPBaffleGEANTMat   =  18;
  HPBaffleX0         =  0.00;
  HPBaffleY0         =  0.00;
  HPBaffleZ0         = -3.14*m;
  HPBaffleDXDZ       =  0.0;
  HPBaffleDYDZ       =  0.0;
  HPBaffleLength     =  1.20*m;
  HPBaffleRin        =  5.5*mm;
  HPBaffleRout       =  3.*cm;
 
  //Cooling pipes
  NCPipeN = 19;
  //=======================================================================
  // CPipeDXDZ=-99999 and CPipeDYDZ=0 puts curved part in z-y plane
  //=======================================================================
  G4int CPGeantMat_[]        = {10     ,   10   ,10     , 10     , 10    , 10    ,  10   , 10    , 10     , 10           ,10            , 31     , 31      ,10        ,10        ,  10     ,  10     , 10      ,10  };
  G4bool CPipeFilledWater_[] = { true  , true   ,true   , true   ,  true , true  ,false  ,true   , false  , true         ,true          , true   ,true     ,true      ,true      , true    , true    , true    , true};
  G4double CPipeX0_[]        = {0.     ,0.      ,0.     , 0.     ,   0.  ,0.     , 0.    , 0.    ,  0.    , 0.           ,0.            , 0      , 0       ,0         ,0         , 0       , 0       , 0       , 0};
  G4double CPipeY0_[]        = {1.05e-2,-1.05e-2,1.05e-2,-1.05e-2,3.5e-2 ,-3.5e-2, 0.    , 0.    ,  0.    , 1.05e-2      ,-1.05e-2      , 5.95e-2, -5.95e-2,5.95e-2   ,-5.95e-2  , 5.95e-2 , -5.95e-2,5.95e-2  ,-5.95e-2};
  G4double CPipeZ0_[]        = {-0.275 , -0.275 ,-0.30  ,-0.30   , -.3001,-.3001 ,.969   , .9695 , .9735  , 0.955        ,0.955         , -0.215 , -0.215  ,-.071     ,-.071     ,-0.287   ,-0.287   , -.30    , -.30};
  G4double CPipeDXDZ_[]      = {    0  ,  0     , 0     ,  0     ,-99999 ,-99999 , 0     ,  0    , 0      , 0            ,0             , 0.     , 0.      ,0.        ,0         , 0.      , 0.      , 0.      , 0.}; 
  G4double CPipeDYDZ_[]      = {    0  ,  0     , 0     ,  0     , 0     ,0      , 0     ,  0    , 0      , 0            ,0             , 0.     , 0.      ,0.        ,0         , 0.      , 0.      , 0.      , 0.};
  G4double CPipeLength_[]    = {1.230  , 1.230  , .025  , .025   , 0     ,0      , 5e-4  ,  4e-3 , 3e-3   , 14.e-3      ,14.e-3         , 14.4e-2, 14.4e-2 ,2e-2      ,2e-2      , 7.2e-2      ,7.2e-2  , 13e-3   ,13e-3};
  G4double CPipeRadiusOut_[] = {3e-3   , 3e-3   ,3e-3   ,3e-3    , 3e-3  ,3e-3   ,13.52e-3,14.1e-3,14.1e-3, 3.4e-3       , 3.4e-3       , 5e-3   , 5e-3    ,4e-3      ,4e-3      , 7.5e-3      , 7.5e-3 , 3e-3    ,3e-3};
  G4double CPipeRadiusIn_[]  = {   0.  ,    0.  ,   0.  ,   0.   , 0.    ,   0.  ,7.52e-3,7.3e-3,7.3e-3   , 0.           , 0.           , 0      , 0       ,0         ,0         , 0           , 0        , 0.      ,0.}; 
  //CPipeRadiusIn is not the inner pipe radius - use radiusOut and wallthickness for that
  G4double CPipeWallThick_[] = {4e-4   ,4e-4    ,4e-4   ,4e-4    ,4e-4   ,4e-4   , 0     ,1.6e-3 , 0      , 0.8e-3       , 0.8e-3       , 1.e-3  , 1.e-3   ,1e-3      ,1e-3      , 1.1e-3      , 1.1e-3  , 4e-4    , 4e-4}; 
  G4double CPipeCurvRad_[]   = {0      , 0      , 0     , 0      ,2.45e-2,2.45e-2, 0     ,   0   , 0      , 0            , 0            , 0      , 0       ,0         ,0         , 0           , 0        , 0       , 0}; // straight tube if this is 0;
  G4double CPipeOpenAng_[]   = {0      , 0      , 0     , 0      ,90.    ,90.    , 0     ,   0   , 0      , 0            , 0            , 0      , 0       ,0         ,0         , 0           , 0        , 0       , 0   };
  G4double CPipeCloseAng_[]  = {0      , 0      , 0     , 0      ,270.   ,270.   , 0     ,   0   , 0      , 0            , 0            , 0      , 0       ,0         ,0         , 0           , 0        , 0       , 0  };
  G4String CPipeVolName_[]   = {"Pipe1","Pipe2" ,"Pipe3","Pipe4" ,"Pipe5","Pipe6","Pipe7","Pipe8","Pipe9" ,"PipeAdapter1","PipeAdapter2","PipeC1","PipeC2" ,"PipeEndT","PipeEndB","PipeBellowT","PipeBellowB"  ,"Pipe1tp","Pipe2btm" };
  
  for (G4int ii=0;ii<NCPipeN;ii++){
    if(airhrn){
      CPGeantMat.push_back(15);
    }
    else{
    CPGeantMat.push_back(CPGeantMat_[ii]);
    }
    CPipeFilledWater.push_back(CPipeFilledWater_[ii]);
    CPipeX0.push_back(CPipeX0_[ii]*m);
    CPipeY0.push_back(CPipeY0_[ii]*m);    
    CPipeZ0.push_back(CPipeZ0_[ii]*m);
    CPipeDXDZ.push_back(CPipeDXDZ_[ii]);
    CPipeDYDZ.push_back(CPipeDYDZ_[ii]);
    CPipeLength.push_back(CPipeLength_[ii]*m);
    CPipeRadiusOut.push_back(CPipeRadiusOut_[ii]*m);
    CPipeRadiusIn.push_back(CPipeRadiusIn_[ii]*m);
    CPipeWallThick.push_back(CPipeWallThick_[ii]*m);
    CPipeCurvRad.push_back(CPipeCurvRad_[ii]*m);
    CPipeOpenAng.push_back(CPipeOpenAng_[ii]*deg);
    CPipeCloseAng.push_back(CPipeCloseAng_[ii]*deg);
    CPipeVolName.push_back(CPipeVolName_[ii]);
  }
  
  //Container
  //=======================================================================
  // Z0 with respect to the first target fin
  NContainerN=21;
  
  G4double CTubeZ0_[]     ={-.42181, -.421556, -.41555, -.35855 , -0.3522 ,-.3332 ,-0.0802, -0.123  ,-0.109  ,-0.0572 ,-0.0492 , -0.04  ,-0.042 ,-.022 , -0.013   , -0.0035  ,0.011     , 0.011       ,  0.011      , 0.9785 , 0.9896  };
  G4double CTubeLength_[] ={.254e-3, 6.006e-3, 5.7e-2 , 6.35e-3 , 19e-3   , 253e-3, 23e-3 , 11e-2   , 66e-3  ,8e-3   , 17e-3  , 6e-3   ,0.02   ,15e-3  , 24e-3    , 14.5e-3  , 0.9785   ,1e-6         , 0.9786      ,5e-4    , 1e-6};
  G4double CTubeRin_[]    ={  0.   , 1.27e-2 , 1.7e-2 , 22.22e-3, 17.5e-3 , 77e-3 , 77e-3 , 14.6e-3 , 16.e-3 , 74e-3 , 24e-3  , 18.5e-3,16e-3  ,16e-3  , 14.6e-3  , 16e-3    , 14.6e-3  ,15.101e-3    , 15.1e-3     , 0.0    , 0.};
  G4double CTubeRout_[]   ={22e-3  , 34.67e-3, 1.9e-2 , 34.67e-3, 107e-3  , 83e-3 , 120e-3, 16.0e-3 , 21.4e-3,106e-3 , 106e-3 , 24e-3  ,18.5e-3,18.5e-3, 16e-3    , 16.4e-3  , 15e-3    , .35         , 15.101e-3   ,14.15e-3, 15.1e-3  };
  G4int CTubeGeantMat_[]  ={   5   ,  10     ,   10   , 10      , 10      , 10    , 10    ,  31     , 10     , 10    , 10     ,  10    , 10    ,10     , 10       , 9        , 9        ,15           ,  15         ,  5     ,  15};
  G4String CTubeVolName_[]={"BeUp1", "BeUp2" , "Added", "BeUp3" , "BFront", "Body", "BEnd","CerTube", "Conn1","CLid1", "CLid2", "Conn2","Conn3","Tube1a" ,"Tube1b", "AlTube1", "AlTube2","TGTExitCyl1","TGTExitCyl2", "BeDW" ,"TGTExitTop"};

 for (G4int ii=0;ii<NContainerN;ii++){
    CTubeZ0.push_back(CTubeZ0_[ii]*m);
    CTubeLength.push_back(CTubeLength_[ii]*m);
    CTubeRin.push_back(CTubeRin_[ii]*m);
    CTubeRout.push_back(CTubeRout_[ii]*m);
    CTubeGeantMat.push_back(CTubeGeantMat_[ii]);
    CTubeVolName.push_back(CTubeVolName_[ii]);
 }
 //Rings holding target and cooling pipes
 NTgtRingN = 5;
 G4double TgtRingZ0_[]         = {-0.03   ,  0.212   , 0.455    , 0.697   , 0.939 };
 G4double TgtRingLength_[]     = {8e-3    ,  8e-3    , 8e-3     , 8e-3    , 8e-3 };
 G4double TgtRingRin_[]        = {9.5e-3  , 9.5e-3   , 9.5e-3   , 9.5e-3  , 9.5e-3};
 G4double TgtRingRout_[]       = {14.55e-3, 14.55e-3 , 14.55e-3 , 14.55e-3, 14.55e-3};
 G4int TgtRingGeantMaterial_[] = {9       , 9        , 9        , 9       , 9 };
 G4String TgtRingVolName_[]    = {"Ring1" , "Ring2"  , "Ring3"  , "Ring4" , "Ring5" };

for (G4int ii=0;ii<NTgtRingN;ii++){
    TgtRingZ0.push_back(TgtRingZ0_[ii]*m);
    TgtRingLength.push_back(TgtRingLength_[ii]*m);
    TgtRingRin.push_back(TgtRingRin_[ii]*m);
    TgtRingRout.push_back(TgtRingRout_[ii]*m);
    TgtRingGeantMaterial.push_back(TgtRingGeantMaterial_[ii]);
    TgtRingVolName.push_back(TgtRingVolName_[ii]);
 }

  //Tunnel         1
  //=======================================================================
  TunnelZ0       = 45.6985*m; //was 45.28*m (08/09/05);
  TunnelRadius   = 3.3*m+.5*m; //added .5m because hadron absorber does not fit entirely inside 3.3
  TunnelLength   = 693.4415*m; //was 693.86*m (08/09/05);
  TunnelA        = 0.0;
  TunnelZ        = 0.0;
  TunnelGEANTmat = 15;
  BeamAngle      = 0.05835; // .05835 in radians.

  //ShieldNshield  5 
  //======================================================================= 
  ShieldX0       = 0.0; 
  ShieldY0       = 0.0; 
  ShieldZ0       = 45.699*m; //was 45.28*m (08/09/05);   
  ShieldDxdz     = 0.0; // not 
  ShieldDydz     = 0.0; // used
  ShieldLength   = 676.681*m; //was 677.1*m (08/09/05);  
  ShieldRout     = 2.23*m; 
  ShieldRin      = 1.0097*m; 
  ShieldGEANTmat = 17;
  
  //DecayPipe          1    
  //=======================================================================
  DecayPipeZ0        = 45.699*m; //was 45.28*m (08/09/05);
  DecayPipeRadius    = 0.9716*m; // was 0.9906 but doesnt correlate w gnumi
  DecayPipeLength    = 676.681*m; //was 677.1*m (08/09/05);
  DecayPipeFWinThick = 1.60E-3*m;
  DecayPipeEWinThick = 4.76E-3*m;
  DecayPipeWallThick = 1.905E-2*m;
  DecayPipeA         = 55.85;
  DecayPipeZ         = 26.0;
  DecayPipeGEANTmat  = 10;
  DecayPipeFWinmat   = 9;
  DecayPipeEWinmat   = 10;

  // New Target Hall by Zach Barnett 
  
  //==========================================================================
  // TargetHallChase       1
  
  NTHConcreteSectionsN = 6;
  
  // units in meters, dimensions based on Numi note.
  G4double THConcreteX0_[]= {3.36073, 2.60670, 0.0, -2.60670, -3.36073, 0.0};
  G4double THConcreteY0_[]= {2.801, 0.0, -2.801, 0, 2.801, 3.45665};
  // these Z0 values are set with respect to the ROCK volume, but NumiTargetHall.cc adjusts them to the TGAR volume 
  G4double THConcreteZ0_[]= {19.499, 19.499, 19.499, 19.499, 19.499, 19.499};
  G4double THConcreteLength_[]= {52.397, 52.397, 52.397, 52.397, 52.397, 52.397};
  G4double THConcreteHdx_[]={.754025,.46675,3.07345,.46675,.754025, 2.921};
  G4double THConcreteHdy_[]={.42705,2.37395,.42705,2.37395,.42705, .2286};
  G4double THConcreteDxdz_[]={0,0,0,0,0, 0.0};
  G4double THConcreteDydz_[]={0,0,0,0,0, 0.0};
  G4int THConcreteGeantMaterial_[] = {17,17,17,17,17, 17};
  G4String THConcreteName_[]={"Section1","Section2","Section3","Section4","Section5","Section6(lid)"};

  for (G4int ii=0;ii<NTHConcreteSectionsN;ii++){
	  THConcreteX0.push_back(THConcreteX0_[ii]*m);
	  THConcreteY0.push_back(THConcreteY0_[ii]*m);
	  THConcreteZ0.push_back(THConcreteZ0_[ii]*m);
	  THConcreteDxdz.push_back(THConcreteDxdz_[ii]*m);
	  THConcreteDydz.push_back(THConcreteDydz_[ii]*m);
	  THConcreteLength.push_back(THConcreteLength_[ii]*m);
	  THConcreteHdx.push_back(THConcreteHdx_[ii]*m);
	  THConcreteHdy.push_back(THConcreteHdy_[ii]*m);
	  THConcreteGeantMaterial.push_back(THConcreteGeantMaterial_[ii]);
	  THConcreteName.push_back(THConcreteName_[ii]);
  }
  
  //==========================================================================

  //======================================================================= 
  // Target Hall shielding (18 blocks, numbered 0-17)
  
  if(jCompare){
    THBlockNblock = 24;
  }
  else{
    THBlockNblock=18;
  }
  // reminder: all length dimensions for the target shielding blocks are in meters.
  //These are the Duratek blocks mentioned in the Numi Technical Design Handbook.

  //Blocks 15-17 are not actually in the Numi Technical Design Handbook--they are used
  //to provide a covering over the top

  G4double THBlockX0_[]    = { 0.0,      //Block 0
							   1.3554,   //Block 1
							   -1.3554,  //Block 2
							   1.7158,    //Block 3
							   .6904,     //Block 4
							   -.6904,    //Block 5
							   -1.7158,   //Block 6
							   1.0254,      //Block 7 
							   -1.0254,     //Block 8
							   1.7158,    //Block 9
							   -1.7158,   //Block 10
							   1.0254,      //Block 11
							   -1.0254,     //Block 12
							   1.7158,    //Block 13
							   -1.7158,    //Block 14
							   0.6904,        //Block15 (top block1--one of the slightly enlarged blocks)
							   -0.6904,        //Block16 (top block2--one of the slightly enlarged blocks)
							   0.0,        //Block17 (top block3)
			       0.0,        // Block 18 (bottom gnumi block)
			       0.6373,  // Block 19 ( side 1 gnumi block)
			       -0.6373, // Block 20 (Side 2 gnumi block)
			       0.0, // Block 21 (top gnumi block bf Horn2)
			       0.0,//
			       0.0//

  };

  G4double THBlockY0_[]    = { -2.03895,      //Block 0
							   -2.03895,      //Block 1
							   -2.03895,      //Block 2
							   -1.03895,      //Block 3
							   -1.36895,      //Block 4
							   -1.36895,      //Block 5
							   -1.03895,      //Block 6
							   -0.36895,     //Block 7
							   -0.36895,     //Block 8
							   0.29105,     //Block 9
							   0.29105,     //Block 10
							   0.96105,      //Block 11
							   0.96105,      //Block 12
							   1.62105,     //Block 13
							   1.62105,      //Block 14
							   1.98395,        //Block15 (top block1--one of the slightly enlarged blocks)
							   1.98395,        //Block16 (top block2--one of the slightly enlarged blocks)
							   1.29355,        //Block17 (top block3)
			       -0.993215, //Block 18 (bottom gnumi block)
                               -0.0377, //Block 19 (side 1 gnumi block)
                               -0.0377, //Block 20 (side 2 gnumi block)
                               0.657075,//Block 21 (top gnumi block)
			       0.657075, // Block 22 (top gnumi block after h2)
                               0.672075 //Block 23 above Horn2

  };

  
  
  // these Z0 values are set with respect to the ROCK volume, but NumiTargetHall.cc adjusts them to the TGAR volume

  G4double THBlockZ0_[]= {19.499};
  G4double THBlockDxdz_[] = {0.0};
  G4double THBlockDydz_[] = {0.0};
	  
  /*	  
  // these Z0 values are set with respect to the ROCK volume, but NumiTargetHall.cc adjusts them to the TGAR volume 
  G4double THBlockZ0_[]    = { 20.64,        //Block 0
  20.64,        //Block 1
							   20.64,        //Block 2
							   20.64,        //Block 3
							   20.64,        //Block 4
							   20.64,        //Block 5
							   20.64,        //Block 6
							   20.64,        //Block 7
							   20.64,        //Block 8
							   20.64,        //Block 9
							   20.64,        //Block 10
							   20.64,        //Block 11
							   20.64,        //Block 12
							   20.64,        //Block 13
							   20.64,         //Block 14
							   20.64,        //Block15 (top block1--one of the slightly enlarged blocks)
							   20.64,        //Block16 (top block2--one of the slightly enlarged blocks)
							   20.64,        //Block17 (top block3)
							   19.499,//Block 18 bottom
							   19.499,//Block 19 side
							   19.499,//Block 20 side	
							   19.499,//Block 21 top
							   19.499,//Block 22 top
							   19.499,//Block 23 top
  };

	  
  G4double THBlockDxdz_[]  = { 0.0,        //Block 0
							   0.0,        //Block 1
							   0.0,        //Block 2
							   0.0,        //Block 3
							   0.0,        //Block 4
							   0.0,        //Block 5
							   0.0,        //Block 6
							   0.0,        //Block 7
							   0.0,        //Block 8
							   0.0,        //Block 9
							   0.0,        //Block 10
							   0.0,        //Block 11
							   0.0,        //Block 12
							   0.0,        //Block 13
							   0.0,         //Block 14
							   0.0,        //Block15 (top block1--one of the slightly enlarged blocks)
							   0.0,        //Block16 (top block2--one of the slightly enlarged blocks)
							   0.0,        //Block17 (top block3)
                             };

  G4double THBlockDydz_[]  = { 0.0,        //Block 0
			       0.0,        //Block 1
			       0.0,        //Block 2
			       0.0,        //Block 3
			       0.0,        //Block 4
			       0.0,        //Block 5
			       0.0,        //Block 6
			       0.0,        //Block 7
			       0.0,        //Block 8
			       0.0,        //Block 9
			       0.0,        //Block 10
			       0.0,        //Block 11
			       0.0,        //Block 12
			       0.0,        //Block 13
			       0.0,         //Block 14
			       0.0,        //Block15 (top block1--one of the slightly enlarged blocks)
			       0.0,        //Block16 (top block2--one of the slightly enlarged blocks)
			       0.0,        //Block17 (top block3)
                             };
  */
	  
  // all normal blocks have the same Length, Hdx, and Hdy values.  These values for the slightly larger blocks are set
  // in NumiTargetHall.cc, line 103
  G4double THBlockLength_[]= {52.397};

  G4double THBlockHdx_[]   = {1.33/2.0};

  G4double THBlockHdy_[]   = {0.67/2.0};

  G4int THBlockGeantMaterial_[] = {10};

  G4String THBlockName_[]  = {"BLK0",
			      "BLK1",
			      "BLK2",
			      "BLK3",
			      "BLK4",
			      "BLK5",
			      "BLK6",
			      "BLK7",
			      "BLK8",
			      "BLK9",
			      "BLK10",
			      "BLK11",
			      "BLK12",
			      "BLK13",
			      "BLK14",
			      "BLK15",
			      "BLK16",
			      "BLK17",
			      "BLK18",
			      "BLK19",
			      "BLK20",
			      "BLK21",
			      "BLK22",
			      "BLK23"

                              };

  //This next block puts all the block coordinates into vectors and assigns the unit "meters" to the values.

  for (G4int ii=0;ii<THBlockNblock;ii++){
    THBlockX0.push_back(THBlockX0_[ii]*m);
    THBlockY0.push_back(THBlockY0_[ii]*m);
    if(ii==21){
      THBlockZ0.push_back(1.15*m);
    }
    else if(ii==22) THBlockZ0.push_back(30.34*m);
    else if(ii==23) THBlockZ0.push_back(12*m);
    else    THBlockZ0.push_back(THBlockZ0_[0]*m);
    THBlockDxdz.push_back(THBlockDxdz_[0]*m);
    THBlockDydz.push_back(THBlockDydz_[0]*m);
    THBlockLength.push_back(THBlockLength_[0]*m);
    THBlockHdx.push_back(THBlockHdx_[0]*m);
    THBlockHdy.push_back(THBlockHdy_[0]*m);
    THBlockGeantMaterial.push_back(THBlockGeantMaterial_[0]);
    THBlockName.push_back(THBlockName_[ii]);
  }

  //======================================================================= 
  // Hadron Box Dimensions
  // 55.75' is the surveyed distance between the
  // downstream wall of Muon Alcove 1 and the
  // nominal center of the Decay Pipe.
  HadrBox_width = 324*.0254*m;
  HadrBox_height = 6.6294*m;
  HadrBox_length = 55.75*12*in-(4*12*in+2.24*in+9*12*in+9*in);

  
  HornCurrent=182100.*ampere; 
  
  NPHorn2EndN=3;
  G4double PHorn2EndZ0_[] ={118.11, 119.86, 122.048};
  //  G4double PHorn2EndZ0_[]     ={135.861        ,137.611     ,139.486};
  G4double PHorn2EndLength_[] ={1.75           ,2.188       ,.625};
  G4double PHorn2EndRin_[]    ={12.719         ,12.532      ,11.};
  G4double PHorn2EndRout_[]   ={14.405         ,14.469      ,12.532};
  G4int PHorn2EndGeantMat_[]  ={31             ,  9         ,  9 };  
  G4String PHorn2EndVolName_[]={"PHorn2InsRing","PHorn2CPB1","PHorn2CPB2"};

  for (G4int ii=0;ii<NPHorn2EndN;ii++){
    PHorn2EndZ0.push_back(PHorn2EndZ0_[ii]*in);
    PHorn2EndLength.push_back(PHorn2EndLength_[ii]*in);
    PHorn2EndRin.push_back(PHorn2EndRin_[ii]*in);
    PHorn2EndRout.push_back(PHorn2EndRout_[ii]*in);
    if(airhrn){
      PHorn2EndGeantMat.push_back(15);
    }
    else{
    PHorn2EndGeantMat.push_back(PHorn2EndGeantMat_[ii]);
    }
    PHorn2EndVolName.push_back(PHorn2EndVolName_[ii]);
  }

  NPHorn1EndN=3;
  G4double PHorn1EndZ0_[] ={118.11        , 119.86 , 122.048};
  //  G4double PHorn1EndZ0_[]     ={126.092        ,127.842     ,129.718};
  G4double PHorn1EndLength_[] ={1.75           ,2.188       ,.624};
  G4double PHorn1EndRin_[]    ={6.00           ,5.815       ,4.25};
  G4double PHorn1EndRout_[]   ={7.687          ,7.75        ,5.815};
  G4int PHorn1EndGeantMat_[]  ={31             ,  9         ,  9   };
  G4String PHorn1EndVolName_[]={"PHorn1InsRing","PHorn1CPB1","PHorn1CPB2"};

  for (G4int ii=0;ii<NPHorn1EndN;ii++){
    PHorn1EndZ0.push_back(PHorn1EndZ0_[ii]*in);
    PHorn1EndLength.push_back(PHorn1EndLength_[ii]*in);
    PHorn1EndRin.push_back(PHorn1EndRin_[ii]*in);
    PHorn1EndRout.push_back(PHorn1EndRout_[ii]*in);
    if(airhrn){ 
      PHorn1EndGeantMat.push_back(15);
    }
    else{
      PHorn1EndGeantMat.push_back(PHorn1EndGeantMat_[ii]);
    }
    PHorn1EndVolName.push_back(PHorn1EndVolName_[ii]);
  }

  //Spider supports for Horn1
  NHorn1SpiderSupportPlanesN=3;
  NHorn1SpidersPerPlaneN=3;
  Horn1SpiderSupportZ0.push_back(19.261*in);
  Horn1SpiderSupportZ0.push_back(42.862*in);
  Horn1SpiderSupportZ0.push_back(82.444*in);

  NumiHornSpiderSupport dummy=NumiHornSpiderSupport();
  dummy.stripW=.5*in;
  dummy.stripH=3.685*in;
  dummy.stripL=.076*in;
  dummy.topW=1.437*in;
  dummy.topH=.108*in;
  dummy.topL=.4*in;
  dummy.bottomW=1.437*in;
  dummy.bottomH=.542*in;
  dummy.bottomL=.4*in;
  dummy.bottomThickMid=.3*in;  //Thickness of bottom part in the middle
  dummy.bottomR=1.188*in;
  dummy.ceramicRodR=.491*in;
  
  Horn1SS.push_back(dummy);
  
  dummy.stripW=.5*in;
  dummy.stripH=3.123*in;
  dummy.stripL=.063*in;
  dummy.topW=1.437*in;
  dummy.topH=.108*in;
  dummy.topL=.4*in;
  dummy.bottomW=1.437*in;
  dummy.bottomH=.454*in;
  dummy.bottomL=.4*in;
  dummy.bottomThickMid=.3*in;  //Thickness of bottom part in the middle
  dummy.bottomR=1.75*in;
  dummy.ceramicRodR=.491*in;
  
  Horn1SS.push_back(dummy);
 
  dummy.stripW=.5*in;
  dummy.stripH=1.56*in;
  dummy.stripL=.031*in;
  dummy.topW=1.437*in;
  dummy.topH=.108*in;
  dummy.topL=.4*in;
  dummy.bottomW=1.437*in;
  dummy.bottomH=.379*in;
  dummy.bottomL=.4*in;
  dummy.bottomThickMid=.3*in;  //Thickness of bottom part in the middle
  dummy.bottomR=3.313*in;
  dummy.ceramicRodR=.491*in;
  
  Horn1SS.push_back(dummy);

  //Spider supports for Horn2
  NHorn2SpiderSupportPlanesN=1;
  NHorn2SpidersPerPlaneN=3;
  Horn2SpiderSupportZ0.push_back(59.809*in);

  dummy.stripW=.5*in;
  dummy.stripH=7.033*in;
  dummy.stripL=.125*in;
  dummy.topW=1.437*in;
  dummy.topH=.108*in;
  dummy.topL=.4*in;
  dummy.bottomW=1.437*in;
  dummy.bottomH=.345*in;
  dummy.bottomL=.4*in;
  dummy.bottomThickMid=.3*in;  //Thickness of bottom part in the middle
  dummy.bottomR=5.812*in;
  dummy.ceramicRodR=.491*in;
 
  Horn2SS.push_back(dummy);
  //---------------------------------------------------------
  
  //Near & Far Detector location
  nNear=11;//was 9 without the different energy for the ND positions.
  nFar=2;
  G4double xdetNear[]    = {0     , 0.     , 7.     , 11.    , 14.    , 14.    , 14.   , 0.  , 25.84  , 4.8/2.       , -4.8/2.       };
  G4double ydetNear[]    = {0     , -3.    , -5.    , -5.    , -6.    , -3.    , 0.    , 71. , 78.42  , 3.8/2.       , -3.8/2.       };
  G4double zdetNear[]    = {1040  , 1010.  , 975.   , 958.   , 940.   , 840.   , 740.  , 940., 745.25 , 1040+16.6/2. , 1040-16.6/2.  };
  G4String detNameNear[] = {"Near","Nova1a","Nova1b","Nova1c","Nova2a","Nova2b","Nova3","MSB","MiniBooNE","Near +x +y +z","Near -x -y -z"};
  G4double xdetFar[]     = {0     , 28.81258   };
  G4double ydetFar[]     = {0     , 81.39258   };
  G4double zdetFar[]     = {735000, 811400     };
  G4String detNameFar[]  = {"Far" , "Ash River"};

  for(G4int ii=0;ii<nNear;ii++){
    xdet_near.push_back(xdetNear[ii]*m);
    ydet_near.push_back(ydetNear[ii]*m);
    zdet_near.push_back(zdetNear[ii]*m);
  }
  for(G4int ii=0;ii<nFar;ii++){
    xdet_far.push_back(xdetFar[ii]*m);
    ydet_far.push_back(ydetFar[ii]*m);
    zdet_far.push_back(zdetFar[ii]*m);
  }
}

NumiDataInput::~NumiDataInput()
{
}

void NumiDataInput::ApplyStepLimits(G4LogicalVolume *vol) {
  if (StepLimit == 0.0) return;
  vol->SetUserLimits(new G4UserLimits(StepLimit));
}
