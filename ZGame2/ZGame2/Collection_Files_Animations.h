#ifndef COLLECTION_FILES_ANIMATION
#define COLLECTION_FILES_ANIMATION

#include <vector>
#include <map>
#include "Animation.h"

//This Class is used put the file locations and animations together in a concise way

class Collection_Files_Animations
{
	typedef std::string Name;
	typedef std::string File_Path;
	typedef std::map<Name, std::pair<File_Path, Type_of_Animation>>::iterator iterator;
	//files
	std::map<Name,std::pair<File_Path,Type_of_Animation>> files;

public:
	//Constructor
	Collection_Files_Animations(Name name, File_Path &file_path, Type_of_Animation &type)
	{
		files[name] = std::pair<File_Path, Type_of_Animation>(file_path, type);
	}

	//Iterators
	iterator begin()
	{
		return files.begin();
	}
	iterator end()
	{
		return files.end();
	}
};



#endif
