#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include "stb_image.h" 
#include "Shader.h"
#include "Textures2D.h"
#include <map>

// A statics singleton ResourceManager class that hosts several
// functions to load textures and shaders.  Each loaded texture 
// and/or shader is also stored for future reference by string 
// handles.  All functions and resources are static and no
// public constructor is defined

class ResourceManager
{
public:
	//Resource Storage
	static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Texture2D> Textures;

	//Loads (and generates) a shader program from file loading vertex and fragment shader's source code.  
	static Shader LoadShader(const char* vShaderFile, const char* fShaderFile, std::string name);

	//Retrieves a stored Shader
	static Shader GetShader(std::string name);

	//Load in a texture with name
	static Texture2D LoadTexture(const char* file, bool alpha, std::string name);

	//Retreive a stored Texture
	static Texture2D GetTexture(std::string name);

	//properly de-Allocate all loaded resources
	static void Clear();

private:
	// Private constructor, that is we do not want any actual resource manager objects.  Its members and functions should be publicly available
	ResourceManager() {};
	// Loads and generates a shader from file
	static Shader LoadShaderFromFile(const char* vShaderFile, const char* fShaderFile);
	//Loads a single texture from file
	static Texture2D LoadTextureFromFile(const char* file, bool alpha);

};

// Instantiate Static Variables
std::map<std::string, Texture2D> ResourceManager::Textures;
std::map<std::string, Shader> ResourceManager::Shaders;

//Now lets actually define the methods of the Resource Manager class
Shader ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, std::string name)
{
	Shaders[name] = LoadShaderFromFile(vShaderFile, fShaderFile);
	return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name)
{

	return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const char* file, bool alpha, std::string name)
{
	Textures[name] = LoadTextureFromFile(file, alpha);
	return Textures[name];
}

Texture2D ResourceManager::GetTexture(std::string name)
{
	return Textures[name];
}

void ResourceManager::Clear()
{
	//Properly delete all shaders
	for (auto iter : Shaders)
		glDeleteProgram(iter.second.ID);
	//Properly Deletes all Textures
	for (auto iter : Textures)
		glDeleteTextures(1, &iter.second.ID);
}


Shader ResourceManager::LoadShaderFromFile(const char* vShaderFile, const char* fShaderFile)
{
	Shader shader;
	shader.CompileShader(vShaderFile, fShaderFile);
	return shader;
}

Texture2D ResourceManager::LoadTextureFromFile(const char* file, bool alpha)
{
	//create Texture2D object
	Texture2D texture;

	// Checks for alpha true
	if (alpha)
	{
		texture.Image_Format = GL_RGBA;
		texture.Internal_Format = GL_RGBA;
	}

	// use stb_load to load in the jpg image and check if the loading of the image was successful
	int width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
	if (data)
	{
		texture.Generate(width, height, data);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	return texture;
}





#endif