#ifndef __SENSITIVE_DETECTOR_HH__
#define __SENSITIVE_DETECTOR_HH__

#include "G4VSensitiveDetector.hh"
#include "G4DynamicParticle.hh"

class SensitiveDetector : public G4VSensitiveDetector
{
public:
	SensitiveDetector(const G4String& name, 
	                  const G4String& hitsCollectionName);
	virtual ~SensitiveDetector();

	// methods from base class
	virtual void   Initialize(G4HCofThisEvent* hitCollection);
	virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
	virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);\

	void CollectEnergyDeposit(const G4Step* step);
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif // __SENSITIVE_DETECTOR_HH__
