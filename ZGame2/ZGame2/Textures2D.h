#ifndef TEXTURES2D_H
#define TEXTURES2D_H


#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <string>
#include "stb_image.h" 


unsigned int TextureFromFile(const char* file, const std::string &directory, bool gamma = false);



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
	void Generate(unsigned int width, unsigned int height, unsigned char* data, int nrComponents);

	// Bind the texture as the current active GL_TEXTURE_2D texture object
	void Bind() const;
};
































#endif
