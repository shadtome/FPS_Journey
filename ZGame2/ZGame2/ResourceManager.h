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







#endif