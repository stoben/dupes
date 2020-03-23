#pragma once

#include "dupes.h"

class dupeRunner
{
private:
	std::string basefolder;
	std::string copyto;

	std::vector<std::string> filters;

	std::set<std::string> md5sign;

	bool recurse;

	long directories;
	long duplicates;
	long skipped;
	long nameCollisions;
	long copies;

	void Parse(std::string dir);
	std::vector<std::string> Split(std::string sval, char separator);
	std::string newFileName(const std::string base, const std::string fname, const std::string ext);

public:
	dupeRunner(std::string baseFolder, std::string copyTo, std::string filter, bool recursive);

	void Parse();	
	std::string md5File(std::string path);
	bool FileExists(const std::string filepath);

	//statistics
	long DirectoriesParsed() { return directories; }
	long SkippedFiles() { return skipped; }
	long Duplicates() { return duplicates; }
	long NameCollisions() { return nameCollisions; }
	long filesCopied() { return copies; }
	

};

