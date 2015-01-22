#include "TH1File.hh"

TH1File* TH1File::instance;

void TH1File::SetFileName(const std::string& fileName)
{
	this->fileName = fileName;
}


void TH1File::CreateHisto(const std::string& name, const std::string& title, Int_t nbinsx, Double_t xlow, Double_t xup)
{
	// if TH1D doesn't exist, TH1D is created.
	if (histograms.find(name) == histograms.end())
		histograms.insert(std::pair<std::string, TH1D>(name, TH1D(name.c_str(), title.c_str(), nbinsx, xlow, xup)));
}

bool TH1File::IsExist(const std::string& name)
{
	if (histograms.find(name) != histograms.end())
		return true;
	else
		return false;
}

void TH1File::Fill(const std::string& name, Double_t value)
{
	if (histograms.find(name) != histograms.end())
		histograms[name].Fill(value);
}

void TH1File::Close()
{
	if (fileName.size() == 0)
		throw (std::string("set file name"));

	TFile file((fileName + ".root").c_str(), "recreate");

	for (std::map<std::string, TH1D>::iterator it = histograms.begin(); it != histograms.end(); ++it)
	{
		((*it).second).SetOption("cp");
		((*it).second).Write();
	}
	file.Close();

	std::map<std::string, TH1D>().swap(histograms);
}