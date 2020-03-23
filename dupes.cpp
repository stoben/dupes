// dupes.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "dupes.h"
#include "dupeRunner.h"

//stats
std::time_t starttime;

std::list<std::tuple<std::string, std::string>> container;


int main(int argc, char* argv[])
{
	//very simple arg-parse
	if (argv[1] == NULL || argv[2] == NULL || argv[3] == NULL)
	{
		std::cout << "please give base folder, filter (semicolon-divided) and dump-folder as arguments " << std::endl;
		std::cout << "dupes.exe D:/pictures/ .png;.gif;.jpg   C:/copies/" << std::endl;
		return 0;
	}

	std::string basepath(argv[1]);		
	std::string filter = argv[2]; 
	std::string copyto(argv[3]);
	
	if (copyto._Starts_with(basepath))
	{//avoid loop
		std::cout << "Copy to path can not be part of basepath ";
		return 0;
	}

	std::cout << "Using base folder " << basepath << " filter " << argv[2] << " copy to " << copyto << std::endl;

	dupeRunner rn(basepath, copyto, filter, true);

	starttime = std::time(nullptr);
	rn.Parse();
	
	std::cout << std::endl << std::endl;
	std::cout << "###################### SUMUP #######################################  " << std::endl;
	std::cout << "Time used " << std::difftime(std::time(nullptr), starttime) << " s " << std::endl;
	std::cout << "Directories parsed " << rn.DirectoriesParsed()  << std::endl;
	std::cout << "Files copied: " << rn.filesCopied() << std::endl;
	std::cout << "Duplicate files detected " << rn.Duplicates() << std::endl;
	std::cout << "Files renamed on copy " << rn.NameCollisions() << std::endl;
	std::cout << "Total files handled " << (rn.filesCopied() + rn.Duplicates() + rn.SkippedFiles()) << std::endl;
	std::cout << "Total files skipped (filtered) " << rn.SkippedFiles() << std::endl;
	std::cout << "#############################################################  " << std::endl;
}
