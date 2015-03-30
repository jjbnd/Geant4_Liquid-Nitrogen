#ifndef __YBCO_SENSITIVE_DETECTOR_HH__
#define __YBCO_SENSITIVE_DETECTOR_HH__

#include "G4VSensitiveDetector.hh"
#include "G4DynamicParticle.hh"

class YBCOSensitiveDetector : public G4VSensitiveDetector
{
public:
	YBCOSensitiveDetector(const G4String& name, 
	                  const G4String& hitsCollectionName);
	virtual ~YBCOSensitiveDetector();

	// methods from base class
	virtual void   Initialize(G4HCofThisEvent* hitCollection);
	virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
	virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);\
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif // __YBCO_SENSITIVE_DETECTOR_HH__
