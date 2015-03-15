#include <sys/stat.h>
#include <unistd.h>

void save()
{
	using namespace std;

	// Create Sub directory
	mkdir("pic", 0700);
	chdir("pic");

	// Open root file
	TFile* file = new TFile("../LN.root", "r");
	TIter it(file->GetListOfKeys());
	TKey* key = NULL;

	// save current working directory ./data
	char buf[255];
	getcwd(buf, 255);

	// iterate root file
	while ((key = (TKey*)it()) != NULL)
	{
		chdir(buf);

		TDirectoryFile* subDir = (TDirectoryFile*) key->ReadObj();
		mkdir(subDir->GetName(), 0700);
		chdir(subDir->GetName());

		saveAsJpg(subDir);
	}


	file->Close();
	delete file;
	delete key;
}

void saveAsJpg(TDirectoryFile* file)
{
	using namespace std;

	TIter it(file->GetListOfKeys());
	TKey* key = NULL;

	while ((key = (TKey*)it()) != NULL)
	{
		TCanvas* canvas = new TCanvas();
		string type(key->GetClassName());

		TH1D* h1 = NULL;
		TH2D* h2 = NULL;

		if (type == "TH1D")
			h1 = (TH1D*) key->ReadObj();
		else if (type == "TH2D")
			h2 = (TH2D*) key->ReadObj();

		if (h1 != NULL)
		{
			canvas->SetName(h1->GetName());
			h1->Draw();
		}
		else if (h2 != NULL)
		{
			canvas->SetName(h2->GetName());
			h2->Draw();
		}

		if (canvas != NULL)
		{
			canvas->SaveAs((string(canvas->GetName()) + ".jpg").c_str());
		}

		delete canvas;
	}

	delete key;
}
