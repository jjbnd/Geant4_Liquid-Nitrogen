#ifndef __TH1_FILE_HH__
#define __TH1_FILE_HH__

#include <string>
#include <map>

#include "TFile.h"
#include "TH1D.h"

class TH1File
{
private:
	static TH1File* instance;

	std::string fileName;
	std::map<std::string, TH1D> histograms;
	TH1File() {}
	TH1File(TH1File const&) {}

public:
	static TH1File* Instance()
	{
		if (!TH1File::instance)
			TH1File::instance = new TH1File();

		return TH1File::instance;
	}

	void SetFileName(const std::string& fileName);

	void CreateHisto(const std::string& name, const std::string& title, Int_t nbinsx, Double_t xlow, Double_t xup);
	bool IsExist(const std::string& name);
	void Fill(const std::string& name, Double_t value);
	void Close();
};



#endif // __TH1_FILE_HH__