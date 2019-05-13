#ifndef MESH_H
#define MESH_H

#include "Shader.h"
#include "Skeleton.h"
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <iostream>
#include <vector>
#include <string>

//constants 
const static float FBX_ROT = 19.0f;


//Organize the vertices with these attributes
struct Vertex
{
	glm::vec3 Position;					//Position
	glm::vec3 Normal;					//Normal vector for this vertex
	glm::vec2 TexCoords;				//coordinates of the textures for this vertex
	unsigned int JointID[4];				//Which joints affect this vertex, when we skin this mesh to the skeleton
	float weights[4];					//Weights of how the corresponding joint afffect this vertex
	int Number_Joints;					//Number of joints affecting this vertex

};


//Organize the Textures
struct Texture
{
	unsigned int ID;
	std::string type;			//a diffuse texture or a specular texture (need to add this in the lighting)
	aiString path;			//path of the file
};


//the collection of vertices and their connections to make a mesh
class Mesh
{
public:
	//Mesh Data
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	Skeleton skeleton;

	//Mesh transformation to its parent
	glm::mat4 mesh_transform;

	//methods
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indieces, std::vector<Texture> textures);
	void Draw(glm::mat4 projection, glm::mat4 view, glm::vec3 pos, Shader &shader,std::string &file_type);
	void Draw(glm::mat4 projection, glm::mat4 view, glm::vec3 pos, float &angle, glm::vec3 &rot_vec, Shader &shader,std::string &file_type);

private:
	
	//Render Data
	unsigned int VAO, VBO, EBO;

	
	
	//methods
	void SetupMesh();

};



#endif
