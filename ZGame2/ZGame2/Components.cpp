
#include "Components.h"


//Name from File
Name File_Name(const char* inputpath)
{
	std::string line;
	std::string word;
	// Arbitrary Name to output
	Name Name_File;
	//Holds data from the file
	std::ifstream datastream;
	//open the file
	datastream.open(inputpath);
	if (!datastream)
	{
		std::cout << "There is a error in opening the file" << std::endl;
	}
	//else std::cout << "The file was successfully opened" << std::endl;

	while (getline(datastream, line))
	{
		std::istringstream record(line);
		record >> word;
		if (word == "Name=")
		{
			record >> Name_File.name;
		}
	}
	datastream.close();
	return Name_File;
};




// Position from file
Position File_Pos(const char* inputpath)
{
	std::string line;
	std::string word;
	//arbitrary Position to output
	Position Pos_File;
	//Holds the data from the file
	std::ifstream datastream;
	//open the file
	datastream.open(inputpath);
	if (!datastream)
	{
		std::cout << "There is a error in opening the file" << std::endl;
	}
	//else std::cout << "the file was successfully opened" << std::endl;

	while (getline(datastream, line))
	{
		std::istringstream record(line);
		record >> word;
		if (word == "Position=")
		{
			record >> word;
			Pos_File.x = ::atof(word.c_str());

			record >> word;
			Pos_File.y = ::atof(word.c_str());

			record >> word;
			Pos_File.z = ::atof(word.c_str());

		}

	}
	datastream.close();
	return Pos_File;
};

// Velocity from File
Velocity File_Vel(const char* inputpath)
{
	std::string line;
	std::string word;
	//arbitrary Position to output
	Velocity Vel_File;
	//Holds the data from the file
	std::ifstream datastream;
	//open the file
	datastream.open(inputpath);
	if (!datastream)
	{
		std::cout << "There is a error in opening the file" << std::endl;
	}
	//else std::cout << "the file was successfully opened" << std::endl;

	while (getline(datastream, line))
	{
		std::istringstream record(line);
		record >> word;
		if (word == "Velocity=")
		{
			record >> word;
			Vel_File.x = ::atof(word.c_str());

			record >> word;
			Vel_File.y = ::atof(word.c_str());

			record >> word;
			Vel_File.z = ::atof(word.c_str());

		}
	}
	datastream.close();
	return Vel_File;
};


//Input Model Data from File;
Model File_Model(const char* inputpath)
{
	std::string line;
	std::string word;
	//arbitrary Position to output
	Model Model_File;
	//Holds the data from the file
	std::ifstream datastream;
	//open the file
	datastream.open(inputpath);
	if (!datastream)
	{
		std::cout << "There is a error in opening the file" << std::endl;
	}
	//else std::cout << "the file was successfully opened" << std::endl;

	while (getline(datastream, line))
	{
		std::istringstream record(line);
		record >> word;
		if (word == "Number_Vertex_Attributes=")
		{
			record >> word;
			Model_File.Number_Vertex_Attributes = ::atof(word.c_str());
			record >> word;
		}
		if (word == "Number_Vertices=")
		{
			record >> word;
			Model_File.Number_Vertices = ::atof(word.c_str());
			record >> word;
		}
		if (word == "Number_Tuples=")
		{
			for (int k = 0; k < Model_File.Number_Vertex_Attributes; ++k)
			{
				record >> word;
				Model_File.Number_Tuples.push_back(::atof(word.c_str()));
			}
		}
		if (word == "Vertices=")
		{

			int skip = 0;
			for (int k = 0; k < Model_File.Number_Vertex_Attributes; ++k)
			{
				skip += Model_File.Number_Tuples[k];
			}
			float vertices[500];
			for (int k = 0; k < skip*Model_File.Number_Vertices; ++k) // This does not work as intended, there must be a different way to import this in, so for now I can just define information in a class for a entities (like a template)
			{
				record >> word;
				vertices[k] = ::atof(word.c_str());

			}
			Model_File.Vertices = vertices;
		}
	}
	datastream.close();
	return Model_File;
};