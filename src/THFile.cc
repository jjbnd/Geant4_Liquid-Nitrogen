#include "THFile.hh"

#include "TPad.h"
#include "TCanvas.h"
#include "G4SystemOfUnits.hh"

#include <fstream>
#include <cmath>
#include <sstream>

THFile* THFile::instance;
int THFile::countEvent;

THFile::~THFile()
{

}

void THFile::SetFileName(const std::string& fileName, const std::string& option)
{
	this->fileName = fileName;
	this->option = option;
}

void THFile::mkdir(const std::string& dirName)
{
	dirs.insert(dirName);
}

void THFile::cd(const std::string& dirName)
{
	file->cd(dirName.c_str());
}

void THFile::Close()
{
	if (!file)
	{
		file = new TFile((fileName + ".root").c_str(), option.c_str());

		//
		// energy deposit distribution
		//

		std::string dir_energyDeposit = "Energy Deposit Distribution";
		file->mkdir(dir_energyDeposit.c_str());
		file->cd(dir_energyDeposit.c_str());

		Double_t LN_totalDepositEnergy = 0;
		for (int i = 0; i < (int) energyDeposit.size(); i++)
		{
			if (energyDeposit[i] == NULL)
				continue;

			energyDeposit[i]->SetOption("colz");
			energyDeposit[i]->Write();

			Double_t depositEnergy = energyDeposit[i]->GetSumOfWeights();
			LN_totalDepositEnergy += depositEnergy;
		}

		file->mkdir("projection");
		file->cd("projection");
		hProjToFront->SetOption("colz");
		hProjToFront->Write();
		hProjToSide->SetOption("colz");
		hProjToSide->Write();

		//
		// total deposit energy
		//

		std::fstream fTotalDeposit("totalDepositEnergy.txt", std::ios_base::out | std::ios_base::trunc);


		fTotalDeposit << "-=Liquid Nitrogen=-" << std::endl;
		fTotalDeposit << "Total Energy (MeV): " << LN_totalDepositEnergy << std::endl;
		fTotalDeposit << "Total Watt(1uA) :  " << ConvertMevToWatt(LN_totalDepositEnergy, 1. * pow(10, -6))  << std::endl;
		fTotalDeposit << "Total Watt(2uA) : " << ConvertMevToWatt(LN_totalDepositEnergy, 2. * pow(10, -6)) << std::endl << std::endl;

		fTotalDeposit << "-=YBCO=-" << std::endl;
		fTotalDeposit << "Total Energy (MeV): " << YBCO_totalEnergyDeposit << std::endl;
		fTotalDeposit << "Total Watt(1uA) :  " << ConvertMevToWatt(YBCO_totalEnergyDeposit, 1. * pow(10, -6))  << std::endl;
		fTotalDeposit << "Total Watt(2uA) : " << ConvertMevToWatt(YBCO_totalEnergyDeposit, 2. * pow(10, -6)) << std::endl << std::endl;

		fTotalDeposit << "-=Back=-" << std::endl;
		fTotalDeposit << "Total Energy (MeV): " << Back_totalEnergyDeposit << std::endl;
		fTotalDeposit << "Total Watt(1uA) :  " << ConvertMevToWatt(Back_totalEnergyDeposit, 1. * pow(10, -6))  << std::endl;
		fTotalDeposit << "Total Watt(2uA) : " << ConvertMevToWatt(Back_totalEnergyDeposit, 2. * pow(10, -6)) << std::endl << std::endl;

		fTotalDeposit.close();

		file->Close();
		delete file;
		file = NULL;
	}
}

void THFile::EnergyDeposit(Double_t x, Double_t y, Double_t z, Double_t energy)
{
	int sliceNum = (int) (floor(z * 10));
	if (sliceNum >= 200)
		sliceNum = 199;

	if (energyDeposit[sliceNum] == NULL)
	{
		std::ostringstream oss;
		oss << "Energy Deposit " << sliceNum;
		energyDeposit[sliceNum] = new TH2D(oss.str().c_str(), oss.str().c_str(), 100, -5, 5, 100, -5, 5);
		
		energyDeposit[sliceNum]->SetXTitle("X (cm)");
		energyDeposit[sliceNum]->SetYTitle("Y (cm)");
		energyDeposit[sliceNum]->SetZTitle("MeV");
	}

	energyDeposit[sliceNum]->Fill(x / 10, y / 10, energy);

	if (hProjToFront == NULL)
	{
		hProjToFront = new TH2D("front", "front", 1000, -50, 50, 1000, -50, 50);
		hProjToFront->SetXTitle("X (mm)");
		hProjToFront->SetYTitle("Y (mm)");
		hProjToFront->SetZTitle("Mev");
	}

	if (hProjToSide == NULL)
	{
		hProjToSide = new TH2D("side", "side", 1000, -50, 50, 200, 0, 20);
		hProjToSide->SetXTitle("Y (mm)");
		hProjToSide->SetYTitle("Z (mm)");
		hProjToSide->SetZTitle("MeV");
	}

	hProjToFront->Fill(x, y, energy);
	hProjToSide->Fill(y, z, energy);
}

void THFile::Add_YBCO_EnergyDeposit(Double_t energy)
{
	YBCO_totalEnergyDeposit += energy;
}

void THFile::Add_Back_EnergyDeposit(Double_t energy)
{
	Back_totalEnergyDeposit += energy;
}

Double_t THFile::ConvertMevToWatt(Double_t energy, Double_t ampare)
{
	Double_t numOfTotalParticles = THFile::countEvent;

	/*
		2uA를 e로 나누어 2uA의 초당 전자 개수 계산
		빔으로 쏜 입자 수를 위 수로 나누면, 2uA에 해당하는 입자를 몇 초간 조사하였는지 나옴
		따라서, 타깃에 축적된 에너지를 이 시간으로 나누면 초당 타깃에 누적되는 에너지가 나옴
		이 초당 에너지가 eV/s단위 이므로 e를 곱하여 Watt단위로 환산
	*/
	Double_t secondAtAmpare = numOfTotalParticles / (ampare / CLHEP::e_SI);

	// G4cout << CLHEP::e_SI << G4endl;		// 1.60218e-19		
	// G4cout << MeV << G4endl;				// 1
	// G4cout << eV << G4endl;				// 1e-06

	// "MeV is one" so, energy must be multiplied 10^6.
	Double_t totalEnergyPerSec = energy * (pow(10, 6)) / secondAtAmpare;
	Double_t totalWatt = totalEnergyPerSec * CLHEP::e_SI;

	// W = J / s
	return totalWatt;
}