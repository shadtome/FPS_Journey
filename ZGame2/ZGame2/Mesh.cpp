
#include "Mesh.h"
#include <glm/gtc/matrix_transform.hpp>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	SetupMesh();
	
}


void Mesh::SetupMesh()
{
	//Note to self (struct's memory layout is sequential )
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),&indices[0],GL_STATIC_DRAW);
	
	//Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	//Vertex Normals
	glEnableVertexAttribArray(1);
	//Offsetof(s,m) is a preprocessor directive that takes as its first argument a struct, and its second argumenta variable name of the struct
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,Normal));	

	//Vertex Texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	//Joint Name
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_UNSIGNED_INT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, JointID));

	//Joint Weights
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, weights));

	glBindVertexArray(0);


}


void Mesh::Draw(glm::mat4 projection, glm::mat4 view, glm::vec3 pos, Shader shader)
{
	
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for (unsigned int i = 0; i < textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);		//Activate proper texture unit before binding
		//retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = textures[i].type;
		if (name == "diffuse")
		{
			number = std::to_string(diffuseNr++);
		}
		else if (name == "specular")
		{
			number = std::to_string(specularNr++);
		}

		shader.setInt(("texture_" + name+number).c_str(), i);
		
		glBindTexture(GL_TEXTURE_2D, textures[i].ID);
	
	}	
	glActiveTexture(GL_TEXTURE0);


	//Set uniforms for the Animation
	//-------------------------------------------------
	//STILL NEED TO DO~~~~~!!!!

	//-------------------------------------------------

	//Set uniforms for matrices
	glm::mat4 model_matrix;
	model_matrix = glm::translate(model_matrix, pos);
	
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
	shader.setMat4("model", model_matrix);
	model_matrix = glm::transpose(glm::inverse(model_matrix));
	shader.setMat4("NormalMatrix", model_matrix);

	

	//Draw Mesh
	
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	
}