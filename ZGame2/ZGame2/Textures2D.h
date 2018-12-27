#ifndef TEXTURES2D_H
#define TEXTURES2D_H


#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <string>
#include "stb_image.h" 

class Texture2D
{
public:
	// Holds the ID for the texture object, used for all operations to reference to this particular texture
	unsigned int ID;
	//Texture image dimensions
	unsigned int Width, Height; //Width and height of Loaded image in pixels
	//Texture Format
	unsigned int Internal_Format; // Format of texture object
	unsigned int Image_Format; // format of loaded image
	//Texture Configuration
	unsigned int Wrap_S; //wrapping mode on S axis
	unsigned int Wrap_T; // wrapping mode on T Axis
	unsigned int Filter_Min; // Filtering mode if Texture pixels < screen Pixels
	unsigned int Filter_Max; // Filtering mode if texture pixels > screen pixels

	// constructor (sets default texture modes)
	Texture2D();
	
	//Generate textures from image data
	void Generate(unsigned int width, unsigned int height, unsigned char* data);

	// Bind the texture as the current active GL_TEXTURE_2D texture object
	void Bind() const;
};

// Now lets define the functions
//Constructor

Texture2D::Texture2D():Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_T(GL_REPEAT), Wrap_S(GL_REPEAT), Filter_Min(GL_LINEAR),Filter_Max(GL_LINEAR)
{
	glGenTextures(1, &ID);
}


// functions to generate the textures
void Texture2D::Generate(unsigned int width, unsigned int height, unsigned char* data)
{
	this->Width = width;
	this->Height = height;

	//Bind our texture to GL_TEXTURE_2D
	glBindTexture(GL_TEXTURE_2D, this->ID);

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
	glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	
	glBindTexture(GL_TEXTURE_2D,0);
}

// Bind the Texture to the current GL_TEXTURE_2D
void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->ID);
}































#endif
