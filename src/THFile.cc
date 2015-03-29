#include "THFile.hh"

#include "TPad.h"
#include "TCanvas.h"



#include <fstream>
#include <cmath>
#include <sstream>

THFile* THFile::instance;

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
		for (int i = 0; i < (int) energyDeposit.size(); i++)
		{
			if (energyDeposit[i] == NULL)
				continue;

			energyDeposit[i]->SetOption("colz");
			energyDeposit[i]->Write();
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
		Double_t totalDepositEnergy = 0;

		for (int i = 0; i < (int) energyDeposit.size(); i++)
		{
			if (energyDeposit[i] == NULL)
				continue;

			Double_t depositEnergy = energyDeposit[i]->GetSumOfWeights();
			fTotalDeposit << i << " : " << depositEnergy << '\n';
			totalDepositEnergy += depositEnergy;
		}

		fTotalDeposit << "Total : " << totalDepositEnergy << std::endl;
		fTotalDeposit.close();

		file->Close();
		delete file;
		file = NULL;
	}
}

void THFile::EnergyDeposit(Double_t x, Double_t y, Double_t z, Double_t energy)
{
	int sliceNum = (int) (floor(z * 10));
	if (sliceNum >= 145)
		sliceNum = 144;

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
		hProjToSide = new TH2D("side", "side", 1000, -50, 50, 150, 0, 14);
		hProjToSide->SetXTitle("Y (mm)");
		hProjToSide->SetYTitle("Z (mm)");
		hProjToSide->SetZTitle("MeV");
	}

	hProjToFront->Fill(x, y, energy);
	hProjToSide->Fill(y, z, energy);
}