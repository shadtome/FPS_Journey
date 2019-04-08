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
#include "Model.h"
#include <map>

// A statics singleton ResourceManager class that hosts several
// functions to load textures and shaders.  Each loaded texture 
// and/or shader is also stored for future reference by string 
// handles.  All functions and resources are static and no
// public constructor is defined

class ResourceManager
{
	typedef std::string Name;
	typedef std::string File_Path;
public:
	//Resource Storage
	static std::map<Name, Shader> Shaders;
	static std::map<Name, Texture2D> Textures;
	static std::map<Name, Full_Model> Models;

	//Loads (and generates) a shader program from file loading vertex and fragment shader's source code.  
	static Shader LoadShader(const char* vShaderFile, const char* fShaderFile, std::string name);

	//Retrieves a stored Shader
	static Shader GetShader(std::string name);

	//Load in a texture with name
	static Texture2D LoadTexture(const char* file, bool alpha, std::string name);

	//Retreive a stored Texture
	static Texture2D GetTexture(std::string name);

	//Load a Model from file
	static Full_Model LoadModel(std::string file_path,std::string name, bool hasskeleton, bool hastexture);

	//Retrieve a stored model
	static Full_Model GetModel(std::string name);

	//Import animation to certain Model
	static void LoadAnimation(std::string file, Type_of_Animation type, std::string name_animation,std::string name_model);


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