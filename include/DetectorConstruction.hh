#ifndef	__DETECTOR_CONSTRUCTION_HH__
#define __DETECTOR_CONSTRUCTION_HH__


#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4Material.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

    G4Material* MakeMaterialSTS304();
    G4Material* MakeMaterialYBCO();
};


#endif