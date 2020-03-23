#include "dupeRunner.h"

dupeRunner::dupeRunner(std::string baseFolder, std::string copyTo, std::string filter, bool recursive)
{
	
	basefolder = baseFolder;
	copyto = copyTo;
	directories = 0;
	duplicates = 0;
	skipped = 0;
	nameCollisions = 0;
	copies = 0;


	recurse = recursive;	

	filters = Split(filter, ';');
}

void dupeRunner::Parse()
{
	Parse(basefolder);
}

void dupeRunner::Parse(std::string dir)
{
	md5gen md5;
	++directories;
	//std::filesystem::path copyTo(copyToDir);

	//auto path = std::filesystem::path("C:\\temp"); // dir);
	for (const auto& entry : std::filesystem::directory_iterator(dir, std::filesystem::directory_options::skip_permission_denied)) // path))
	{
		if (entry.is_directory() && recurse)
		{
			Parse(entry.path().generic_string());
		}
		else
		{
			if (entry.is_regular_file())
			{
				char drt[1024];
				char fname[1024];
				char drive[5];
				char ext[1024];

				_splitpath_s(entry.path().string().c_str(), drive, 5, drt, 1024, fname, 1024, ext, 1024);

				//file we are looking for?
				bool bSkip = true;
				for (auto f : filters)
				{
					if (f == ext)
					{
						bSkip = false;
						break;
					}
				}

				if (bSkip)
				{
					++skipped;
#ifdef _DEBUG
					std::cout << "skipping " << entry.path().string() << std::endl;
#endif
					continue; //skip this
				}

				auto md5 = md5File(entry.path().string());

				//duplicate?
				if (md5sign.find(md5) != md5sign.end())
				{
					++duplicates;
					std::cout << "#!Duplicate detected:" << entry.path().string() << ":" << md5File(entry.path().string()) << std::endl;
					continue;
				}								

				std::string toFile;
				toFile.assign(copyto);
				toFile.append(fname);
				toFile.append(ext);

				md5sign.insert(md5);
				std::cout << entry.path().string() << ":" << md5File(entry.path().string()) << std::endl;

				if (FileExists(toFile))
				{
					++nameCollisions;
					toFile = newFileName(copyto, fname, ext);
					std::cout << "#Name collision. file " << entry.path().string() << " renamed on copy " << toFile << std::endl;
				}
				++copies;
				std::filesystem::copy_file(entry, toFile, std::filesystem::copy_options::overwrite_existing);

			}
		}
	}
}

std::vector<std::string> dupeRunner::Split(std::string sval, char separator)
{
	std::vector<std::string> ret;
	std::istringstream att(sval);
	std::string s;
	while (std::getline(att, s, separator))
	{
		ret.push_back(s);
	}

	return ret;
}


std::string dupeRunner::md5File(std::string path)
{
	char* content;

	std::ifstream fcontent(path.c_str(), std::ios::in | std::ios::binary | std::ios::ate); //ate = at the end

	if (fcontent.is_open())
	{
		std::streampos size = fcontent.tellg();
		
		content = new char[size];
		fcontent.seekg(0, std::ios::beg);
		fcontent.read(content, size);
		fcontent.close();

		md5gen md5;
		md5.update(content, size);
		delete[] content;

		return md5.finalize().hexdigest();

	}
}

bool dupeRunner::FileExists(const std::string filepath)
{
	struct stat buf;	
	return stat(filepath.c_str(), &buf) == 0;	
}

std::string dupeRunner::newFileName(const std::string base, const std::string fname, const std::string ext)
{
	int copy = 1;
	std::string newName;
	do
	{
		newName.assign(base);
		newName.append(fname);
		newName.append("_Copy");
		newName.append(std::to_string(copy));
		newName.append(ext);
		++copy;
	} while (FileExists(newName));

	return newName;
}