// from B2a

#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction* Det)
 : G4UImessenger(),
   fDetectorConstruction(Det)
{
  fMuOnSiDirectory = new G4UIdirectory("/muOnSi/");
  fMuOnSiDirectory->SetGuidance("UI commands specific to this program.");

  fDetDirectory = new G4UIdirectory("/muOnSi/det/");
  fDetDirectory->SetGuidance("Detector construction control");

  fSiThickCmd = new G4UIcmdWithADoubleAndUnit("/muOnSi/det/siThickness",this);
  fSiThickCmd->SetGuidance("Define the thickness of Si for the simulation");
  fSiThickCmd->SetParameterName("siThickness",false);
  fSiThickCmd->SetUnitCategory("Length");
  fSiThickCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
  delete fSiThickCmd;
  delete fMuOnSiDirectory;
  delete fDetDirectory;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{

  if( command == fSiThickCmd ) {
    fDetectorConstruction->SetSiThickness(fSiThickCmd->GetNewDoubleValue(newValue));
  }   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
