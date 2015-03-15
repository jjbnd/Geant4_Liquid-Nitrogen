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


void THFile::CreateHisto(const std::string& dir, const std::string& name, const std::string& title, Int_t nbinsx, Double_t xlow, Double_t xup)
{
	std::map<std::string, std::map<std::string, TH1D*>*>* mh = (std::map<std::string, std::map<std::string, TH1D*>*>*) &multi_histo[0];
	if (mh->find(dir) == mh->end())
	{
		std::map<std::string, TH1D*>* histo = new std::map<std::string, TH1D*>();
		(*histo)[name] = new TH1D(name.c_str(), title.c_str(), nbinsx, xlow, xup);
		(*mh)[dir] = histo;
	}
	else
	{
		std::map<std::string, TH1D*>* histo = (*mh)[dir];
		if (histo->find(name) == histo->end())
		{
			TH1D* h = new TH1D(name.c_str(), title.c_str(), nbinsx, xlow, xup);
			(*histo)[name] = h;
		}
	}
}

void THFile::CreateHisto(const std::string& dir, const std::string& name, const std::string& title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup)
{
	std::map<std::string, std::map<std::string, TH2D*>*>* mh = (std::map<std::string, std::map<std::string, TH2D*>*>*) &multi_histo[1];
	if (mh->find(dir) == mh->end())
	{
		std::map<std::string, TH2D*>* histo = new std::map<std::string, TH2D*>();
		(*histo)[name] = new TH2D(name.c_str(), title.c_str(), nbinsx, xlow, xup, nbinsy, ylow, yup);
		(*mh)[dir] = histo;
	}
	else
	{
		std::map<std::string, TH2D*>* histo = (*mh)[dir];
		if (histo->find(name) == histo->end())
		{
			TH2D* h = new TH2D(name.c_str(), title.c_str(), nbinsx, xlow, xup, nbinsy, ylow, yup);
			(*histo)[name] = h;
		}
	}
}


bool THFile::IsExist(const int dim, const std::string& dir, const std::string& name)
{
	int d = dim - 1;
	switch (d)
	{
	case 1:
		{
			std::map<std::string, std::map<std::string, TH1D*>*>* mh = (std::map<std::string, std::map<std::string, TH1D*>*>*) &multi_histo[0];
			if (mh->find(dir) != mh->end())
			{
				std::map<std::string, TH1D*>* h = (*mh)[dir];
				if (h->find(name) != h->end())
					return true;
			}
		}
		break;

	case 2:
		{
			std::map<std::string, std::map<std::string, TH2D*>*>* mh = (std::map<std::string, std::map<std::string, TH2D*>*>*) &multi_histo[1];
			if (mh->find(dir) != mh->end())
			{
				std::map<std::string, TH2D*>* h = (*mh)[dir];
				if (h->find(name) != h->end())
					return true;
			}
		}
		break;
	}
	
	return false;
}

void THFile::Fill(const std::string& dir, const std::string& name, Double_t x)
{
	std::map<std::string, std::map<std::string, TH1D*>*>* mh = (std::map<std::string, std::map<std::string, TH1D*>*>*) &multi_histo[0];
	(*(*mh)[dir])[name]->Fill(x);
}

void THFile::Fill(const std::string& dir, const std::string& name, Double_t x, Double_t y)
{
	std::map<std::string, std::map<std::string, TH2D*>*>* mh = (std::map<std::string, std::map<std::string, TH2D*>*>*) &multi_histo[1];
	(*(*mh)[dir])[name]->Fill(x, y);
}

void THFile::Close()
{
	if (!file)
	{
		std::fstream isotopes("isotopes.txt", std::ios_base::out | std::ios_base::trunc);

		int num_of_h = 0;
		file = new TFile((fileName + ".root").c_str(), option.c_str());
		for (int i = 0; i < 2; i++)
		{
			if (i == 0)
			{
				std::map<std::string, std::map<std::string, TH1D*>*>* mh = (std::map<std::string, std::map<std::string, TH1D*>*>*) &multi_histo[0];
				num_of_h = (*mh)["Secondaries"]->size();
				file->mkdir("total");
				TH1D* total = new TH1D("total_number", "total number of radiations", num_of_h, 0, num_of_h);

				for (std::map<std::string, std::map<std::string, TH1D*>*>::iterator it = mh->begin(); it != mh->end(); it++)
				{
					std::string dir_name = (*it).first;
					file->mkdir(dir_name.c_str());
					file->cd(dir_name.c_str());
					for (std::map<std::string, TH1D*>::iterator n_it = (*it).second->begin(); n_it != (*it).second->end(); n_it++)
					{
						(*n_it).second->SetOption("cd");
						(*n_it).second->Write();

						total->Fill((*n_it).first.c_str(), (*n_it).second->GetEntries());
						isotopes << (*n_it).first << " : " << (*n_it).second->GetEntries() << '\n';

						delete (*n_it).second;
					}
					delete (*it).second;
					isotopes.close();
				}

				file->cd("total");
				TCanvas* c = new TCanvas("Total number of radiations");
				TPad* pad1 = new TPad("log scale", "log scale", 0, 0, 0.5, 1);
				TPad* pad2 = new TPad("original", "original", 0.5, 0, 1, 1);
				pad1->Draw(); pad2->Draw();

				pad1->cd();
				pad1->SetLogy();
				total->Draw("text");
				total->Draw("same");

				pad2->cd();
				total->Draw();

				c->Write();
				
				delete pad1;
				delete pad2;

				delete c;
				delete total;		
			}
			else if (i == 1)
			{
				std::map<std::string, std::map<std::string, TH2D*>*>* mh = (std::map<std::string, std::map<std::string, TH2D*>*>*) &multi_histo[1];
				for (std::map<std::string, std::map<std::string, TH2D*>*>::iterator it = mh->begin(); it != mh->end(); it++)
				{
					std::string dir_name = (*it).first;
					file->mkdir(dir_name.c_str());
					file->cd(dir_name.c_str());
					for (std::map<std::string, TH2D*>::iterator n_it = (*it).second->begin(); n_it != (*it).second->end(); n_it++)
					{
						(*n_it).second->SetOption("colz");
						(*n_it).second->Write();
						delete (*n_it).second;
					}
					delete (*it).second;
				}
				
			}
		}

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

		fTotalDeposit << "Total : " << totalDepositEnergy << '\n';
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

	energyDeposit[sliceNum]->Fill(x, y, energy);

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