// modified from example B1

#include "DetectorConstruction.hh"
#include "SiDetSD.hh"
#include "DetectorMessenger.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4StateManager.hh"
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
#include "G4SDManager.hh"

#include "G4UserLimits.hh"

DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction()
{

  _messenger = new DetectorMessenger(this);
  _siThick = 160*um;
  
  return;
}

DetectorConstruction::~DetectorConstruction()
{
  return;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{  
  // Clean old geometry, if any. from AnaEx01
  //
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_size = 30*cm; // the word will be a cube with this side
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  //G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");

  _worldSizeZ = world_size;
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_size, 0.5*world_size, 0.5*world_size);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking

  //
  // Silicon detector
  //
  G4double siDet_size = 1*cm;
  // thickness initialized in constructor
  G4Material* siDet_mat = nist->FindOrBuildMaterial("G4_Si");

  _solidSiDet =
    new G4Box(
	      "siDet", // name
	      0.5*siDet_size, 0.5*siDet_size, 0.5*_siThick); // size

  G4LogicalVolume* logicSiDet =
    new G4LogicalVolume(_solidSiDet, // solid
			siDet_mat,  // material
			"sidet_LV");   // name

  // limit step length in the detector
  G4UserLimits* userLimits = new G4UserLimits(1*um);
  logicSiDet->SetUserLimits(userLimits);
  
  //  G4VPhysicalVolume* physSiDet =
    new G4PVPlacement(0,                                      // rotation
		      G4ThreeVector(0,0,0),                   // position
		      logicSiDet,                             // logical volume
		      "sidet_PV",                                // name
		      logicWorld,                             // mother volume
		      false,                                  // boolean operation
		      0,                                      // copy number
		      checkOverlaps);                         // overlap checking
    
  //
  //always return the physical World
  //
  return physWorld;
}

void DetectorConstruction::ConstructSDandField()
{
  // Sensitive detectors

  // delete old sensitive detectors if they exist, to avoid warnings
  G4VSensitiveDetector* oldDet = G4SDManager::GetSDMpointer()->FindSensitiveDetector("siSensDet", false);
  if(oldDet)
    delete oldDet;

  SiDetSD* sensDet = new SiDetSD("siSensDet", "SiDetHitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector(sensDet);
  // Setting a sensDet to all logical volumes with the same name 
  // of "sidet_LV".
  SetSensitiveDetector("sidet_LV", sensDet);

  return;
}

void DetectorConstruction::SetSiThickness(G4double thickness)
{
  if(thickness >= _worldSizeZ){
    G4cout << "Requested Si thikness exceeds the world size (" << _worldSizeZ << " mm) and will not be set" << G4endl;
    return;
  }
  
  if(_siThick != thickness){
    _siThick = thickness;
    G4cout << "Set Si thickness to " << _siThick << " mm" << G4endl;

    if( G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit ){ // from AnaEx01
      G4RunManager::GetRunManager()->ReinitializeGeometry();
    }
  }
  
  return;
}
