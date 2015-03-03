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
		// G4cout << "---------------------- Hit! ----------------------" << G4endl;
		for (G4TrackVector::const_iterator it = secondaries->begin(); it != secondaries->end(); it++)
		{
			// G4cout << "********* TrackID: " << std::setw(2) << (*it)->GetTrackID() << " *********" << G4endl;

			const G4DynamicParticle* dynamic_particle = (*it)->GetDynamicParticle();
			// const G4ParticleDefinition* particle = (*it)->GetParticleDefinition();

			// G4cout << "Particle Name: " << particle->GetParticleName() << G4endl;
		// 	G4cout << "         Type: " << particle->GetParticleType() << G4endl;
		// 	G4cout << "         Encoding: " << particle->GetPDGEncoding() << G4endl;
		// 	if (dynamic_particle)
		// 	{
		// 		G4cout << "         TotalEnergy: " << dynamic_particle->GetTotalEnergy() / MeV << " MeV" << G4endl;
		// 		G4cout << "         KineticEnergy: " << dynamic_particle->GetKineticEnergy() / MeV << " Mev" << G4endl;

		// 		const G4ThreeVector momentum = dynamic_particle->GetMomentumDirection();
		// 		G4cout << "         Momentum: " << "(" << momentum.x() << ", " << momentum.y() << ", " << momentum.z() << ")" << G4endl;
		// 		G4cout << "         Charge: " << dynamic_particle->GetCharge() << G4endl;
		// 	}

		// 	G4cout << "************************************" << G4endl << G4endl;

		// 	// if particle is dynamic_particle, store energy
			if (dynamic_particle)
			{
				StoreData(dynamic_particle);
				StoreData2(dynamic_particle);
			}
		}
		// G4cout << "--------------------------------------------------" << G4endl;
	}
	// else
	// {
	// 	G4Track* track = step->GetTrack();
	// 	G4int trackID = track->GetTrackID();
	// 	if (trackID > 0)
	// 	{
	// 		const G4DynamicParticle* dynamic_particle = track->GetDynamicParticle();
	// 		const G4ParticleDefinition* particle = track->GetParticleDefinition();

	// 		G4String tracking = "Tracking";
	// 		G4String particle_name = particle->GetParticleName();
	// 		if (!file->IsExist(tracking, particle_name))
	// 			file->CreateHisto(tracking, particle_name, "Deposit Energy");
	// 	}
	// }

	return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SensitiveDetector::StoreData(const G4DynamicParticle* dynamic_particle)
{

	const G4String particle_name = dynamic_particle->GetParticleDefinition()->GetParticleName();
	THFile* file = THFile::Instance();
	G4String secondaries = "Secondaries";
	if (!file->IsExist(1, secondaries, particle_name))
		file->CreateHisto(secondaries ,particle_name, "Kinetic Energy", 1000, 0.0, 10 * MeV);
	file->Fill(secondaries, particle_name, dynamic_particle->GetKineticEnergy());
}

void SensitiveDetector::StoreData2(const G4DynamicParticle* dynamic_particle)
{
	const G4ParticleDefinition* particle = dynamic_particle->GetParticleDefinition();
	G4String particle_name = particle->GetParticleName();
	THFile* file = THFile::Instance();
	G4String secondaries = "proton-neutron";

	int mass = 0;
	int idx = 0;
	for (int i = 0; i < (int) particle_name.length(); i++)
	{
		if (particle_name[i] >= '0' && particle_name[i] <= '9')
		{
			idx = i;
			break;
		}
	}

	mass = atoi(particle_name.substr(idx).c_str());

	if (!file->IsExist(2, secondaries, secondaries))
		file->CreateHisto(secondaries, secondaries, "proton-neutron", 20, 0, 20, 20, 0, 20);

	G4int p = particle->GetAtomicNumber(); 	       // number of proton , atomic number
	G4int n = mass - p;   // number of neutrons

	file->Fill(secondaries, secondaries, p, n);
}
