#include "Textures2D.h"
#include "ResourceManager.h"


//Figure out a different way to implement this in to this code, since I already have other code that does all of this already
unsigned int TextureFromFile(const char* file, const std::string &directory, bool gamma)
{
	std::string filename = std::string(file);
	filename = directory + '/' + filename;
	
	unsigned int textureID;
	glGenTextures(1, &textureID);	

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path:" << file << std::endl;
		stbi_image_free(data);
	}
	
	return textureID;
}







// Now lets define the functions
//Constructor

Texture2D::Texture2D() :Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_T(GL_REPEAT), Wrap_S(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR)
{
	glGenTextures(1, &ID);
}


// functions to generate the textures
void Texture2D::Generate(unsigned int width, unsigned int height, unsigned char* data, int nrComponents)
{
	this->Width = width;
	this->Height = height;

	//Bind our texture to GL_TEXTURE_2D
	glBindTexture(GL_TEXTURE_2D, this->ID);

	GLenum format;
	if (nrComponents == 1)
	{
		format = GL_RED;
	}
	else if (nrComponents == 3)
	{
		format = GL_RGB;
	}
	else if (nrComponents == 4)
	{
		format = GL_RGBA;
	}

	//Set our Parameters
	//Texture Parameters
	//-----------------------------
	// Texture Wrappings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);

	//texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);

	//Import the data for the texture and create it
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);


	glBindTexture(GL_TEXTURE_2D, 0);
}

// Bind the Texture to the current GL_TEXTURE_2D
void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->ID);
}





