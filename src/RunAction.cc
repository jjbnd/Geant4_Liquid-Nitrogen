#include "RunAction.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"

#include "G4SystemOfUnits.hh"

// #include "g4root.hh"
#include "TH1File.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
: G4UserRunAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* run)
{ 
	//inform the runManager to save random number seed
	G4RunManager::GetRunManager()->SetRandomNumberStore(false);


	// // for root output
	// G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	// analysisManager->OpenFile("LN");
	// analysisManager->SetHistoDirectoryName("LN_Gamma");
	
	// analysisManager->CreateH1("gamma", "Kinetic Energy", 10000, 0.0, 10*MeV);


	TH1File* file = TH1File::Instance();
	file->SetFileName("LN");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* run)
{
	// G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	// analysisManager->Write();
	// analysisManager->CloseFile();

	TH1File* file = TH1File::Instance();
	file->Close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
