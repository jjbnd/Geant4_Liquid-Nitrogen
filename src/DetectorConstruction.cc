#include "DetectorConstruction.hh"

#include "G4SystemOfUnits.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4Box.hh"

#include "SensitiveDetector.hh"

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction()
{ }

DetectorConstruction::~DetectorConstruction()
{ }

G4VPhysicalVolume* DetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXY = 50*cm;
  G4double world_sizeZ  = 50*cm;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");
  
  G4VSolid* solidWorld =
		new G4Box("World",								//its name
		world_sizeXY, world_sizeXY, world_sizeZ);		//its size
	  
  G4LogicalVolume* logicWorld =                         
	new G4LogicalVolume(solidWorld,				//its solid
						world_mat,				//its material
						"World");				//its name

  G4VPhysicalVolume* physWorld = 
	new G4PVPlacement(0,              			//no rotation
					  G4ThreeVector(),			//at (0,0,0)
					  logicWorld,     			//its logical volume
					  "World",        			//its name
					  0,              			//its mother  volume
					  false,          			//no boolean operation
					  0,              			//copy number
					  checkOverlaps); 			//overlaps checking
 
  //     
  // Liquid Nitrogen
  // http://www.ge.infn.it/geant4/training/ptb_2009/day2/solutions_day2_partII.html
  //  
  G4String name("LiquidNitrogen");
  G4double z = 7;
  G4double density = 0.808*g/cm3;            // http://en.wikipedia.org/wiki/Nitrogen
  G4double a = 14.007*g/mole;                // http://en.wikipedia.org/wiki/Nitrogen
  G4double temperature = 77*kelvin;          // http://en.wikipedia.org/wiki/Liquid_nitrogen
											 // 63K ~ 77K


  G4Material* liquidNitrogen = new G4Material(name, z, a, density, kStateLiquid, temperature);
  G4ThreeVector pos = G4ThreeVector(0, 0, 0);

  G4VSolid* nitrogen_box = new G4Box("LN_box", 10*cm, 10*cm, 10*cm);

  G4LogicalVolume* logicShape =
	new G4LogicalVolume(nitrogen_box,     //its solid
						liquidNitrogen,   //its material
						"LN_LV");         //its name

  new G4PVPlacement(0,                    ///no rotation
					pos,                  //at position
					logicShape,           //its logical volume
					"LN_PV",              //its name
					logicWorld,           ///its mother  volume
					false,                ///no boolean operation
					0,                    ///copy number
					checkOverlaps);       ///overlaps checking
				
  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void DetectorConstruction::ConstructSDandField()
{
  SensitiveDetector* sd = new SensitiveDetector("SD", "SDHitsCollection");

  SetSensitiveDetector("LN_LV", sd);
}