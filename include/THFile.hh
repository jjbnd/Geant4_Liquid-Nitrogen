#ifndef __TH_FILE_HH__
#define __TH_FILE_HH__

#include <string>
#include <set>
#include <map>
#include <vector>

#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "G4ios.hh"

class THFile
{
private:
	static THFile* instance;
	static int countEvent;

	TFile* file;
	std::string fileName;
	std::string option;
	std::set<std::string> dirs;

	std::vector<TH2D*> energyDeposit;

	TH2D* hProjToFront;
	TH2D* hProjToSide;

	THFile()
		: file(NULL)
	{
		energyDeposit.resize(200);

		hProjToFront = NULL;
		hProjToSide = NULL;
	}
	
	THFile(THFile const&) {}


public:
	static THFile* Instance()
	{
		if (!THFile::instance)
		{
			THFile::instance = new THFile();
			THFile::countEvent = 0;
		}

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

	static void CountStartEvent()
	{
		THFile::countEvent++;
	}

	~THFile();

	void SetFileName(const std::string& fileName, const std::string& option = "recreate");

	void mkdir(const std::string& dir);
	void cd(const std::string& dir);

	void Close();

	void EnergyDeposit(Double_t x, Double_t y, Double_t z, Double_t energy);
};



#endif // __TH_FILE_HH__
