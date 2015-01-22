#include "SensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

#include "G4ParticleDefinition.hh"
#include "G4DynamicParticle.hh"
#include "G4TrackVector.hh"

#include "G4SystemOfUnits.hh"

// #include "g4root.hh"
#include "TH1File.hh"

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

void SensitiveDetector::Initialize(G4HCofThisEvent* hce)
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
	{
		stepLength = step->GetStepLength();
	}

	if ( edep==0. && stepLength == 0. ) return false;


	const G4TrackVector* secondaries = step->GetSecondary();
	if (secondaries->size() > 0)
	{
		// G4cout << "---------------------- Hit! ----------------------" << G4endl;
		for (G4TrackVector::const_iterator it = secondaries->begin(); it != secondaries->end(); it++)
		{
		// 	G4cout << "********* TrackID: " << std::setw(2) << (*it)->GetTrackID() << " *********" << G4endl;

			const G4DynamicParticle* dynamic_particle = (*it)->GetDynamicParticle();
			const G4ParticleDefinition* particle = (dynamic_particle != NULL ?
														dynamic_particle->GetParticleDefinition() :   // true
														(*it)->GetParticleDefinition());              // false

		// 	G4cout << "Particle Name: " << particle->GetParticleName() << G4endl;
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
				StoreData(dynamic_particle);
		}
		// G4cout << "--------------------------------------------------" << G4endl;
	}

	return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SensitiveDetector::StoreData(const G4DynamicParticle* dynamic_particle)
{
	// G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	// const G4String particle_name = dynamic_particle->GetParticleDefinition()->GetParticleName();
	// G4int histo_id = analysisManager->GetH1Id(particle_name);

	// if (histo_id < 0)
	// 	histo_id = analysisManager->CreateH1(particle_name, "Kinetic Energy", 10000, 0.0, 10*MeV);

	// analysisManager->FillH1(histo_id, dynamic_particle->GetKineticEnergy());

	const G4String particle_name = dynamic_particle->GetParticleDefinition()->GetParticleName();
	TH1File* file = TH1File::Instance();
	if (!file->IsExist(particle_name))
		file->CreateHisto(particle_name, "Kinetic Energy", 1000, 0.0, 10 * MeV);
	file->Fill(particle_name, dynamic_particle->GetKineticEnergy());
}