#include "DetectorConstruction.hh"

#include "G4SystemOfUnits.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4Box.hh"

#include "SensitiveDetector.hh"

#include "G4VisAttributes.hh"

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
    new G4Box("World",                //its name
    world_sizeXY, world_sizeXY, world_sizeZ);   //its size
    
  G4LogicalVolume* logicWorld =                         
  new G4LogicalVolume(solidWorld,       //its solid
            world_mat,        //its material
            "World");       //its name

  G4VPhysicalVolume* physWorld = 
  new G4PVPlacement(0,                    //no rotation
            G4ThreeVector(),      //at (0,0,0)
            logicWorld,           //its logical volume
            "World",              //its name
            0,                    //its mother  volume
            false,                //no boolean operation
            0,                    //copy number
            checkOverlaps);       //overlaps checking

  //
  // front STS304
  //
  G4double z, a, fractionmass, density;
  G4String name, symbol;
  G4int ncomponents;

  // STS304 properties -> http://www.susmetal.co.kr/sts304.htm
  // gram per mole     -> http://www.convertunits.com/from/mole/to/gram

  // Carbon 0.08%
  a = 12.0107 * g/mole;
  G4Element* elC = new G4Element(name="Carbon", symbol="C" , z = 7., a);

  // Silicon 1.00%
  a = 81.0228 * g/mole;
  G4Element* elSi = new G4Element(name="Silicon", symbol="Si" , z = 14., a);

  // Manganese 2.00%
  a = 54.938049 * g/mole;
  G4Element* elMn = new G4Element(name="Manganese", symbol="Mn" , z = 25., a);

  // Phosphorus 0.040%
  a = 30.973761 * g/mole;
  G4Element* elP = new G4Element(name="Phosphorus", symbol="P" , z = 15., a);

  // Sulfur 0.030%
  a = 32.065 * g/mole;
  G4Element* elS = new G4Element(name="Sulfur", symbol="S" , z = 16., a);

  // Chromium 19.00% (18.00 ~ 20.00)
  a = 51.9961 * g/mole;
  G4Element* elCr = new G4Element(name="Chromium", symbol="Cr" , z = 24., a);

  // Nickel 9.25% (8.00 ~ 10.50)
  a = 58.6934 * g/mole;
  G4Element* elNi = new G4Element(name="Nickel", symbol="Ni" , z = 28., a);

  // Iron
  a = 55.845 * g/mole;
  G4Element* elFe = new G4Element(name="Iron", symbol="Fe" , z = 26., a);

  // STS 304
  density = 7.93 * g/m3;
  G4Material* STS304_mat = new G4Material(name="STS304_mat", density, ncomponents = 8);

  G4double ironRatio = 100.0 * perCent;
  STS304_mat->AddElement(elC,  fractionmass = 0.08 * perCent); ironRatio -= fractionmass;
  STS304_mat->AddElement(elSi, fractionmass = 1.00 * perCent); ironRatio -= fractionmass;
  STS304_mat->AddElement(elMn, fractionmass = 2.00 * perCent); ironRatio -= fractionmass;
  STS304_mat->AddElement(elP,  fractionmass = 0.04 * perCent); ironRatio -= fractionmass;
  STS304_mat->AddElement(elS,  fractionmass = 0.03 * perCent); ironRatio -= fractionmass;
  STS304_mat->AddElement(elCr, fractionmass = 19.0 * perCent); ironRatio -= fractionmass;
  STS304_mat->AddElement(elNi, fractionmass = 9.25 * perCent); ironRatio -= fractionmass;
  STS304_mat->AddElement(elFe, fractionmass = ironRatio);

  G4double STS304_xy = 10  / 2. * cm;
  G4double STS304_z  = 0.1 / 2. * mm;
  G4VSolid* STS304_box = new G4Box("STS304_box", STS304_xy, STS304_xy, STS304_z);
  G4LogicalVolume* STS304_LV =
                        new G4LogicalVolume(STS304_box,
                                            STS304_mat,
                                            "STS304_LV");

  G4VisAttributes* att = new G4VisAttributes();
  att->SetColor(0,0,255);
  STS304_LV->SetVisAttributes(att);

  G4ThreeVector STS304_pos(0, 0, STS304_z);
  new G4PVPlacement(0,
                    STS304_pos,
                    STS304_LV,
                    "STS304_PV_0",
                    logicWorld,
                    false,
                    0,
                    checkOverlaps);

  //
  // Liquid Nitrogen
  //

  /*
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
  */

  G4Material* liquidNitrogen_mat = nist->FindOrBuildMaterial("G4_lN2");

  G4double LN_xy = 10   / 2. * cm;
  G4double LN_z  = 14.5 / 2. * mm;
  G4VSolid* nitrogen_box = new G4Box("LN_box", LN_xy, LN_xy, LN_z);
  G4LogicalVolume* liquidNitrogen_LV =
      new G4LogicalVolume(nitrogen_box,         //its solid
                          liquidNitrogen_mat,   //its material
                          "LN_LV");             //its name

  G4ThreeVector LN_pos = G4ThreeVector(0, 0, STS304_z * 2 + LN_z);
  new G4PVPlacement(0,               ///no rotation
          LN_pos,                    //at position
          liquidNitrogen_LV,         //its logical volume
          "LN_PV",                   //its name
          logicWorld,                ///its mother  volume
          false,                     ///no boolean operation
          0,                         ///copy number
          checkOverlaps);            ///overlaps checking

  //
  // back STS304
  //
  new G4PVPlacement(0,
                    G4ThreeVector(0, 0, STS304_z * 2 + LN_z * 2 + STS304_z),
                    STS304_LV,
                    "STS304_PV_1",
                    logicWorld,
                    false,
                    0,
                    checkOverlaps);

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