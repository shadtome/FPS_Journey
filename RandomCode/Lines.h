#ifndef LINES_H
#define LINES_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>

float vertices_lines[]
{
	0.0f, -100.0f, 0.0f,
	0.0f, 100.0f, 0.0f

};


class Lines
{
public:

	// the unique integers associted to the VAO and the VBO
	unsigned int VAO;
	unsigned int VBO;

	// Shader it points to
	Shader* shaderpointer;





	//constructor for this class
	Lines(Shader &shader)
	{
		shaderpointer = &shader;


		//generate the unsigned ints for this buffer objects


		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);


		//bind the VertexArray and Vertex object array
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind the newly created buffer to the GL_ARRAY_BUFFER target with this function
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_lines), vertices_lines, GL_STATIC_DRAW);//  bytes for this vertex.  This copies the vertex data in to the buffers memory





		//Configure Vertex attribute(s)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // since we changed the vertex data above, we have to change how the vertex pointer moves now

		glEnableVertexAttribArray(0);

		//unbind the vertex array, 
		glBindVertexArray(0);
	}

	// Bind the VAO to the current vertex array to use this for drawing
	void bindVAO()
	{

		glBindVertexArray(VAO);
	}

	void unbindVAO()
	{

		glBindVertexArray(0);

	}

	void draw(glm::mat4 view, glm::mat4 projection, glm::mat4 model,glm::vec3 color)
	{
		bindVAO();
		shaderpointer->use();
		shaderpointer->setMat4("view", view);
		shaderpointer->setMat4("projection", projection);
		shaderpointer->setMat4("model", model);
		shaderpointer->setVec3("aColor", color.x,color.y,color.z);
		
		glDrawArrays(GL_LINES, 0, 2);
		unbindVAO();

	}


};












#endif
