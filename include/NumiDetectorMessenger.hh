#ifndef NumiDetectorMessenger_H
#define NumiDetectorMessenger_H 1

#include "globals.hh"
#include "G4UImessenger.hh"

class NumiDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;

class NumiDetectorMessenger: public G4UImessenger {

public:
   NumiDetectorMessenger(NumiDetectorConstruction* );
   ~NumiDetectorMessenger();
   
   void SetNewValue(G4UIcommand*, G4String);
   
private:
   NumiDetectorConstruction* NumiDetector;
   
   G4UIdirectory*              NumiDir;
   G4UIdirectory*              detDir;
   G4UIcmdWithAString*         TargetGasCmd;
   G4UIcmdWithADoubleAndUnit*  TargetZ0Cmd;
   G4UIcmdWithADoubleAndUnit*  HornCurrentCmd;
   G4UIcmdWithABool*           ConstructTarget;
   G4UIcmdWithABool*           ConstructSolidMuMons;
   G4UIcmdWithAString*         AbsorberConfig;
   G4UIcmdWithAString*         Mon0AbsMatCmd;
   G4UIcmdWithAString*         Mon1AbsMatCmd;
   G4UIcmdWithAString*         Mon2AbsMatCmd;
   G4UIcmdWithADoubleAndUnit*  Mon0AbsThickCmd;
   G4UIcmdWithADoubleAndUnit*  Mon1AbsThickCmd;
   G4UIcmdWithADoubleAndUnit*  Mon2AbsThickCmd;
   G4UIcmdWithADoubleAndUnit*  Mon0AbsDistCmd;
   G4UIcmdWithADoubleAndUnit*  Mon1AbsDistCmd;
   G4UIcmdWithADoubleAndUnit*  Mon2AbsDistCmd;
   
   //G4UIcmdWithAnInteger*     NbLayersCmd;    
   G4UIcmdWithoutParameter*    UpdateCmd;
};

#endif
