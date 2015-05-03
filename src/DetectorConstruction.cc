#include "DetectorConstruction.hh"

#include "G4SystemOfUnits.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4Box.hh"
#include "G4Hype.hh"

#include "LNSensitiveDetector.hh"
#include "YBCOSensitiveDetector.hh"
#include "BackSensitiveDetector.hh"

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
    new G4Box("World",                          //its name
    world_sizeXY, world_sizeXY, world_sizeZ);   //its size
    
  G4LogicalVolume* logicWorld =                         
  new G4LogicalVolume(solidWorld,     //its solid
            world_mat,                //its material
            "World");                 //its name

  G4VPhysicalVolume* physWorld = 
  new G4PVPlacement(0,                    //no rotation
            G4ThreeVector(),              //at (0,0,0)
            logicWorld,                   //its logical volume
            "World",                      //its name
            0,                            //its mother  volume
            false,                        //no boolean operation
            0,                            //copy number
            checkOverlaps);               //overlaps checking

  // make STS304 material
  G4Material* STS304_mat = MakeMaterialSTS304();

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

  //
  // front STS304
  //
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

  G4Material* liquidNitrogen_mat = nist->FindOrBuildMaterial("G4_lN2");

  G4double LN_xy = 10 / 2. * cm;
  G4double LN_z  = 20 / 2. * mm;
  G4VSolid* nitrogen_box = new G4Box("LN_box", LN_xy, LN_xy, LN_z);
  G4LogicalVolume* liquidNitrogen_LV =
      new G4LogicalVolume(nitrogen_box,         //its solid
                          liquidNitrogen_mat,   //its material
                          "LN_LV");             //its name

  G4ThreeVector LN_pos(0, 0, STS304_z * 2 + LN_z);
  new G4PVPlacement(0,               ///no rotation
          LN_pos,                    //at position
          liquidNitrogen_LV,         //its logical volume
          "LN_PV",                   //its name
          logicWorld,                ///its mother  volume
          false,                     ///no boolean operation
          0,                         ///copy number
          checkOverlaps);            ///overlaps checking

  //
  // super conductor: YBCO
  //

  G4double ybco_wide   = 4 / 2 * mm;
  G4double ybco_height = 10  / 2 * mm;
  G4double ybco_z      = 0.1 / 2 * mm;
  G4VSolid* ybco_box = new G4Box("YBCO_box", ybco_wide, ybco_height, ybco_z);

  G4LogicalVolume* ybco_LV = new G4LogicalVolume(ybco_box,
                                                  MakeMaterialYBCO(),
                                                  "YBCO_LV");

  // mother logical volume is standard position.
  G4ThreeVector ybco_pos(0, 0, 0);
  new G4PVPlacement(0,
                    ybco_pos,
                    ybco_LV,
                    "YBCO_PV",
                    liquidNitrogen_LV,
                    false,
                    0,
                    checkOverlaps
                    );

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
  // Back Detector
  //

  G4double back_detector_wide = LN_xy;
  G4double back_detector_height = LN_xy;
  G4double back_detector_z = 10 * mm;

  G4Box* back_detector_box = new G4Box("Back_Detector_box", back_detector_wide, back_detector_height, back_detector_z);

  G4LogicalVolume* back_detector_lv = new G4LogicalVolume(back_detector_box,
                                                          nist->FindOrBuildMaterial("G4_Fe"),
                                                          "Back_Detector_LV");

  new G4PVPlacement(0,
                    G4ThreeVector(0, 0, STS304_z * 2 + LN_z * 2 + STS304_z + back_detector_z),
                    back_detector_lv,
                    "Back_Detector_PV",
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
  LNSensitiveDetector* LN_sd = new LNSensitiveDetector("LN_SD", "LN_SDHitsCollection");
  SetSensitiveDetector("LN_LV", LN_sd);

  YBCOSensitiveDetector* YBCO_sd = new YBCOSensitiveDetector("YBCO_SD", "YBCO_SDHitsCollection");
  SetSensitiveDetector("YBCO_LV", YBCO_sd);

  BackSensitiveDetector* back_sd = new BackSensitiveDetector("Back_Detector_LV", "Back_SDHitsCollection");
  SetSensitiveDetector("Back_Detector_LV", back_sd);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* DetectorConstruction::MakeMaterialSTS304()
{
  G4double z, a, fractionmass, density;
  G4String name, symbol;
  G4int ncomponents;

  // STS304 properties -> http://www.susmetal.co.kr/sts304.htm
  // gram per mole     -> http://www.convertunits.com/from/mole/to/gram

  // Carbon 0.08%
  a = 12.0107 * g/mole;
  G4Element* elC = new G4Element(name="Carbon", symbol="C" , z = 6., a);

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

  return STS304_mat;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* DetectorConstruction::MakeMaterialYBCO()
{
  G4double z, a, fractionmass, density;
  G4String name, symbol;
  G4int ncomponents;

  // Yttrium
  a = 88.90585 * g/mole;
  G4Element* eY = new G4Element(name="Yttrium", symbol="Y" , z = 39., a);

  // Barium
  a = 137.327 * g/mole;
  G4Element* eBa = new G4Element(name="Barium", symbol="Ba" , z = 56., a);

  // Copper
  a = 63.546 * g/mole;
  G4Element* eCu = new G4Element(name="Copper", symbol="Cu" , z = 29., a);

  // Oxygen
  a = 15.9994 * g/mole;
  G4Element* eO = new G4Element(name="Oxygen", symbol="O" , z = 8., a);

  // YBCO
  // density 4.4 ~ 5.3 g/cm3
  density = 5.0 * g/cm3;
  G4Material* YBCO_mat = new G4Material(name="YBCO", density, ncomponents = 4);

  //
  // Y   : Ba  : Cu  : O
  // 1.2 : 0.8 : 1.0 : 4
  //
  // Y  = 17.14286 %
  // Ba = 11.42857 %
  // Cu = 14.28571 %
  // O  = 57.14286 %

  YBCO_mat->AddElement(eY,  fractionmass = 17.14286 * perCent);
  YBCO_mat->AddElement(eBa, fractionmass = 11.42857 * perCent);
  YBCO_mat->AddElement(eCu, fractionmass = 14.28571 * perCent);
  YBCO_mat->AddElement(eO,  fractionmass = 57.14286 * perCent);

  return YBCO_mat;
}
