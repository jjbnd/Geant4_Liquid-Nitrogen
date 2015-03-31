#include "COPPERSensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

#include "G4ParticleDefinition.hh"
#include "G4DynamicParticle.hh"
#include "G4TrackVector.hh"

#include "THFile.hh"

#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

COPPERSensitiveDetector::COPPERSensitiveDetector(
							const G4String& name, 
							const G4String& hitsCollectionName)
 : G4VSensitiveDetector(name)
{
	collectionName.insert(hitsCollectionName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

COPPERSensitiveDetector::~COPPERSensitiveDetector() 
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void COPPERSensitiveDetector::Initialize(G4HCofThisEvent*)
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool COPPERSensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*)
{
	THFile* file = THFile::Instance();
	file->Add_COPPER_EnergyDeposit(step->GetTotalEnergyDeposit() / MeV);
	
	return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void COPPERSensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......