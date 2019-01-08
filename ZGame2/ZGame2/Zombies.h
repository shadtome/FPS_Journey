#ifndef ZOMBIES_H
#define ZOMBIES_H

#include "ResourceManager.h"
#include "Entity_Manager.h"
#include "Graphics.h"
#include "CollisionSystem.h"

float vertices[] = {
-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
-0.5f,  0.5f, -0.5f,  0.0f, 1.0f

};

//Texture2D Contexture=ResourceManager::LoadTexture("/Users/Cody Tipton/Desktop/ZGame/ZGame1/container.jpg", false, "Test");

//Zombie Spawner
void Zombie_Create(Entity_Manager &world, Texture2D &texture,glm::vec3 pos)
{
	// Set the Model Data
	Model model_data;
	model_data.Graphics_Options.Turn_Opt_On(TRIANGLES | TEXTURES);
	model_data.Vertices = vertices;
	model_data.Number_Vertices = 36;
	model_data.Number_Vertex_Attributes = 2;
	model_data.Number_Tuples = { 3,2 };
	model_data.pos = pos;
	model_data.texture = &texture;
	Set_VAO(model_data);
	for (int k = 0; k < 3; ++k)
	{
		model_data.scale[k] = 1;
	}

	//Set the Entity information
	IEntity entity;
	Attributes temp;
	temp.Turn_Opt_On(NAME | MODEL | VELOCITY | COLLISION);
	world.Include_Entity(entity, temp);


	// Set the Model in
	world.Input_Model(entity, model_data);

	//Input Data
	world.Input_Data(entity, "/Users/Cody Tipton/Desktop/ZGame/Position_Test.txt");

	//Set the Collision Information
	Collision col;
	col.State = DYNAMIC;
	Setup_Collision(model_data, col);
	world.components.Input_Col(entity,col);

};


//Wall information
// Vertices/ Texture coords/ normals
float Wall_vertices[] =
{
	-1.0, -1.0, 0.0,	0.0, 0.0,	0.0, 0.0, 1.0,
	 1.0, -1.0, 0.0,	1.0, 0.0,	0.0, 0.0, 1.0,
	 1.0,  1.0, 0.0,	1.0, 1.0,	0.0, 0.0, 1.0,
	 1.0,  1.0, 0.0,	1.0, 1.0,	0.0, 0.0, 1.0,
	-1.0,  1.0, 0.0,	0.0, 1.0,	0.0, 0.0, 1.0,
	-1.0, -1.0, 0.0,	0.0, 0.0,	0.0, 0.0, 1.0
};

void Wall_Spawner(Entity_Manager &world, Texture2D &tex, glm::vec3 pos)
{
	// Set the Model Data
	Model model_data;
	model_data.Graphics_Options.Turn_Opt_On(TRIANGLES | TEXTURES);
	model_data.Vertices =Wall_vertices;
	model_data.Number_Vertices = 6;
	model_data.Number_Vertex_Attributes = 3;
	model_data.Number_Tuples = { 3,2,3 };
	model_data.pos = pos;
	model_data.texture = &tex;
	Set_VAO(model_data);
	for (int k = 0; k < 3; ++k)
	{
		model_data.scale[k] = 1;
	}
	model_data.angle = glm::radians(90.0);

	//Set the Entity information
	IEntity entity;
	Attributes temp;
	temp.Turn_Opt_On(NAME | MODEL | VELOCITY | COLLISION);
	world.Include_Entity(entity, temp);


	// Set the Model in
	world.Input_Model(entity, model_data);

	//Input Data
	world.Input_Data(entity, "/Users/Cody Tipton/Desktop/ZGame/Position_Test.txt");

	//Set the Collision Information
	Collision col;
	col.State = WALL;
	Setup_Collision(model_data, col);
	world.components.Input_Col(entity, col);
}

#endif
