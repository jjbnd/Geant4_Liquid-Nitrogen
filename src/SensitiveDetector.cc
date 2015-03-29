#include "SensitiveDetector.hh"
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

#include <limits>
#include <cmath>
#include <cstdlib>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SensitiveDetector::SensitiveDetector(
							const G4String& name, 
							const G4String& hitsCollectionName)
 : G4VSensitiveDetector(name)
{
	collectionName.insert(hitsCollectionName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SensitiveDetector::~SensitiveDetector() 
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SensitiveDetector::Initialize(G4HCofThisEvent*)
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool SensitiveDetector::ProcessHits(G4Step* step, 
									 G4TouchableHistory*)
{
	// energy deposit
	G4double edep = step->GetTotalEnergyDeposit();

	// step length
	G4double stepLength = 0.;
	if ( step->GetTrack()->GetDefinition()->GetPDGCharge() != 0. )
		stepLength = step->GetStepLength();

	if ( abs(edep)		 < std::numeric_limits<G4double>::epsilon() &&
		 abs(stepLength) < std::numeric_limits<G4double>::epsilon() )
		return false;

	const G4TrackVector* secondaries = step->GetSecondary();
	if (secondaries->size() > 0)
	{
		for (G4TrackVector::const_iterator it = secondaries->begin(); it != secondaries->end(); it++)
		{
			const G4DynamicParticle* dynamic_particle = (*it)->GetDynamicParticle();
			// const G4ParticleDefinition* particle = (*it)->GetParticleDefinition();

			// if particle is dynamic_particle, store energy
			if (dynamic_particle)
			{
				
			}
		}
	}

	CollectEnergyDeposit(step);

	return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SensitiveDetector::CollectEnergyDeposit(const G4Step* step)
{
	THFile* file = THFile::Instance();
	G4ThreeVector pos = step->GetTrack()->GetPosition();
	file->EnergyDeposit(pos.x(), pos.y(), pos.z(), step->GetTotalEnergyDeposit() / MeV);
}