// modified from example B1

#ifndef DETECTORCONSTRUCTION_HH
#define DETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class DetectorMessenger;
class G4Box;

/// Detector construction class to define materials and geometry.

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  DetectorConstruction();
  virtual ~DetectorConstruction();
  
  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();
  
  G4double GetWorldSizeZ() {return _worldSizeZ;};
  void SetSiThickness(G4double thickness);
  
  private:
  G4double _worldSizeZ;
  DetectorMessenger* _messenger;
  G4double _siThick;
  G4Box* _solidSiDet;
};



#endif
