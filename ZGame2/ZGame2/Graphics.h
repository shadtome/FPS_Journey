#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Entity_Manager.h"
#include "Components.h"
#include "Attributes.h"
#include "Shader.h"
#include <array>


//Change the Position of the model
void ChangeModel_Pos(Model &model, Velocity &vel)
{
	model.pos.x += vel.x;
	model.pos.y += vel.y;
	model.pos.z += vel.z;
}

void ChangeModel_Pos_World(Entity_Manager &world)
{
	for (int k = 0; k < world.components.E_Model.Data.size(); ++k)
	{
		unsigned int vel_loc = world.components.E_Model.Data[k].Entity_Vel_ID;
		(world.components.E_Model.Data[k], world.components.E_Vel.Data[vel_loc]);  
	}
}





//Set VAO for the model
void Set_VAO(Model &model)
{
	// Generate a unique ID for the VAO
	glGenVertexArrays(1, &(model.VAO));

	// Generate a unqiue ID for the VBO
	glGenBuffers(1, &(model.VBO));

	// Set the skip and gap for the attribute pointers
	int skip = 0;
	int gap = 0;
	for (int k = 0; k < model.Number_Vertex_Attributes; ++k)
	{
		skip += model.Number_Tuples[k];
	}

	//bind the VertexArray and Vertex object array
	glBindVertexArray(model.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, model.VBO); // bind the newly created buffer to the GL_ARRAY_BUFFER target with this function
	glBufferData(GL_ARRAY_BUFFER, model.Number_Vertices*skip*sizeof(model.Vertices), model.Vertices, GL_STATIC_DRAW);//  bytes for this vertex.  This copies the vertex data in to the buffers memory
	


	
	for (int k = 0; k < model.Number_Vertex_Attributes; ++k)
	{	
		
		glVertexAttribPointer(k, model.Number_Tuples[k], GL_FLOAT, GL_FALSE, skip * sizeof(float), (void*)(gap * sizeof(float)));
		gap += model.Number_Tuples[k];
		glEnableVertexAttribArray(k);
	}
	glBindVertexArray(0);
	
}









//Draw entity model
void Draw_Entity(Model &model,glm::mat4 &projection, glm::mat4 &view,glm::mat4 &model_matrix,Shader &shader)
{
	
	//If this model has textures on, then use textures
	if (model.Graphics_Options.Is_Opt_On(TEXTURES))
	{
		// Activate the Textures
		glActiveTexture(GL_TEXTURE0);
		model.texture->Bind();
	}
	glBindVertexArray(model.VAO);
	if (model.Graphics_Options.Is_Opt_On(COLORS))
	{
		// Maybe I will need this? probobly not, I can used textures out the but to make some nice looks
	}
	//Shader points and sets the matrix information about the projection, model, and view matrices
	shader.use();
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.setMat4("model", model_matrix);

	
	
	
	// gldraw with traingles option
	if (model.Graphics_Options.Is_Opt_On(TRIANGLES))
	{
		glDrawArrays(GL_TRIANGLES, 0, model.Number_Vertices);
	}
	//Unbind the current VAO
	glBindVertexArray(0);
}



//Set up all the VAO and graphics Data for a Entity_Manager world
void Set_VAO_World(Entity_Manager &world)
{
	for (int k = 0; k < world.components.E_Model.Data.size(); ++k)
	{
		Set_VAO(world.components.E_Model.Data[k]);
	}
}

//Draw all the assoctated enties in the E_Model component 
void Draw_World(Entity_Manager &world,glm::mat4 &projection, glm::mat4 &view, Shader &shader)
{
	
	

	for (int k = 0; k < world.components.E_Model.Data.size(); ++k)
	{	
		glm::mat4 model_matrix;
		
		model_matrix=glm::translate(model_matrix, world.components.E_Model.Data[k].pos);
		model_matrix = glm::scale(model_matrix, glm::vec3(world.components.E_Model.Data[k].scale[0], world.components.E_Model.Data[k].scale[1], world.components.E_Model.Data[k].scale[2]));
		
		Draw_Entity(world.components.E_Model.Data[k],projection,view,model_matrix,shader);

		

		//Set the New Center of the AABB and/ or Bounding Sphere
		if (world.World[world.components.E_Model.Data[k].Entity_ID].Is_Opt_On(COLLISION))
		{
			//Reference to Collision data
			int col_loc = world.components.E_Model.Data[k].Entity_Col_ID;
			// Update the Collision Data
			model_matrix = glm::mat4();
			model_matrix = glm::translate(model_matrix, world.components.E_Model.Data[k].pos);

			glm::vec3 original_pos = world.components.E_Col.Data[col_loc].Original_Pos; // Find the Original Position for the AABB

			glm::vec3 cnewPos = glm::vec3(model_matrix*glm::vec4(original_pos, 1)); // Multiply the original position by the translation matrix from the translation of the model

			world.components.E_Col.Data[col_loc].box.UpdateAABB(cnewPos); // Set new location for the center of the AABB
		}
	}
}


#endif
