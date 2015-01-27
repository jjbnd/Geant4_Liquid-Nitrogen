#ifndef __TH_FILE_HH__
#define __TH_FILE_HH__

#include <string>
#include <set>
#include <map>

#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"

class THFile
{
private:
	static THFile* instance;

	TFile* file;
	std::string fileName;
	std::string option;
	std::set<std::string> dirs;
	std::map<std::string, void*> multi_histo[2];

	THFile()
		: file(NULL)
	{ }
	THFile(THFile const&) {}


public:
	static THFile* Instance()
	{
		if (!THFile::instance)
			THFile::instance = new THFile();

		return THFile::instance;
	}

	static void RemoveInstance()
	{
		if (THFile::instance)
		{
			delete THFile::instance;
			THFile::instance = NULL;
		}
	}

	~THFile();

	void SetFileName(const std::string& fileName, const std::string& option = "recreate");

	void mkdir(const std::string& dir);
	void cd(const std::string& dir);

	void CreateHisto(const std::string& dir, const std::string& name, const std::string& title, Int_t nbinsx, Double_t xlow, Double_t xup);
	void CreateHisto(const std::string& dir, const std::string& name, const std::string& title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup);
	bool IsExist(const int dim, const std::string& dir, const std::string& name);
	void Fill(const std::string& dir, const std::string& name, Double_t x);
	void Fill(const std::string& dir, const std::string& name, Double_t x, Double_t y);
	void Close();
};



#endif // __TH_FILE_HH__
