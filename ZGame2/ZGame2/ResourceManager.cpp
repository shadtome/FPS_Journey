
#include "ResourceManager.h"



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
	if (alpha)							//Might not need this
	{
		texture.Image_Format = GL_RGBA;
		texture.Internal_Format = GL_RGBA;
	}

	// use stb_load to load in the jpg image and check if the loading of the image was successful
	int width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
	if (data)
	{
		texture.Generate(width, height, data, nrChannels);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	return texture;
}