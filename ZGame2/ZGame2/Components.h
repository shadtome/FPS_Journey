#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <glm/glm.hpp> //the GLM math header files (all the awesome math goodness!!)
#include <glm/gtc/matrix_transform.hpp>  //More math stuff from glm
#include <glm/gtc/type_ptr.hpp>  //this is to change the way matrices look so that is can be implemted in to OpenGL
#include <iostream>
#include <map>
#include <stdio.h>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <cmath>
#include "Attributes.h"
#include "Collision.h"
#include "Animation_Manager.h"
#include "Model.h"
#include "Animation.h"
#include "IEntity.h"

//Forward Deceleration
class Texture2D;
class AABB;
class ColSphere;



// Components
//bitmask for the best memory and to put on the true or false in a simple 32 bit (4 bytes int data)
// Define bit literals in C++14 (Need to do it differently in C++11 or earlier)
// This is for 32 bit first ( we can always do more if more options are needed)
//const unsigned char position = 0b00000010; // Option 2
//const unsigned char velocity = 0b00000100; // Option 3

typedef unsigned int Comp_Mask;
enum component_ID
{
	NAME = 0b00000001,
	POSITION = 0b00000010,
	VELOCITY = 0b00000100,
	MODEL = 0b00001000,
	COLLISION = 0b00010000,
	AI = 0b00100000,
	PLAYER = 0b01000000,
	DOESDAMAGE = 0b10000000,
	HEALTH = 0b0000000100000000,
	ANIMATION = 0b0000001000000000
};


class Name
{
public:
	//Reference back to the Entity it is associated with
	//IEntity *entity_pointer; //Why does this not work? It says that I am missing ; before *, what???
	unsigned int Entity_ID = 0;
	// The name of the Entity
	std::string name;
};

class Position // This is the position for the Bounding box or spheres (the position of the model is in the model class)
{
public:
	//Reference back to the Entity it is associated with
	unsigned int Entity_ID = 0;

	float x; 
	float y;
	float z;
};

class Velocity
{
public:
	//Reference back to the Entity it is associated with
	unsigned int Entity_ID = 0;

	// Velocity of the entity
	float x;
	float y;
	float z;
};

//----------------------------------------------
//Graphics
// Options for if textures are used or colors, ect...
enum Graphics_Options
{
	TEXTURES=0b00000001,
	COLORS	=0b00000010,
	TRIANGLES = 0b00000100,
	LINES = 0b00001000
};

/*class Model
{
public:
	//Pointer back to the original Entity
	//IEntity *entity; // Would be better to be able to point back to the IEntity, but this wont let me do it for somereason
	//Reference Back tot he ID for the entity
	unsigned int Entity_ID = 0;
	//Reference back to the Velocity data it is associated with
	unsigned int Entity_Vel_ID = 0;
	// Reference back to Collision Data if it has it
	unsigned int Entity_Col_ID = 0;


	// Unsigned char for determining on and off for options
	Attributes Graphics_Options;

	// how many datapoints are in the vertex array
	int Size_of_Vertices;
	// Hold the vertex data
	float* Vertices;
	// Number of vertex points (there can be repeats)
	int Number_Vertices;

	// Unique ID for the VAO
	unsigned int VAO;
	// Unique ID for the VBO
	unsigned int VBO;

	//Number of vertex Attributes
	int Number_Vertex_Attributes;

	//Number of tuples for each data, i.e. for position it would be 3, and for texture coordinates it would be 2, ect.
	std::vector<int> Number_Tuples; // Order matters and depends on the order that is put in to the vertices
	
	//Texture for the model
	Texture2D* texture;

	//Colors
	float r;
	float b;
	float g;

	//scale of the model
	float scale[3] = { 1,1,1 };

	//Position of the model (This is the position that the origin in the models local coordinates it mapped to)
	glm::vec3 pos;

	//Rotation information
	//counter clockwise roation about the vector
	float angle = 0;

	glm::vec3 Vector_Rot = glm::vec3(0.0, 1.0, 0.0);




};*/

class Model
{
	typedef std::string Name;
public:
	//References back to important components
	//Reference Back tot he ID for the entity
	unsigned int Entity_ID = 0;
	//Reference back to the Velocity data it is associated with
	unsigned int Entity_Vel_ID = 0;
	// Reference back to Collision Data if it has it
	unsigned int Entity_Col_ID = 0;

private:
	//Pointer to IEntity
	IEntity* ptr;
public:

	Full_Model* model;			//The imported model data/animation data/skeleton data/ textures data

	std::map<Name,Animator> animators;		//Animators

	//Current Collection of Joint Transforms for animations
	SkeletonPose Cur_Pose;

	//Information to change the model with respect to translation, rotation and scale
	glm::vec3 pos;				// Model Position in the world coordinates
	//scale of the model
	float scale[3] = { 1,1,1 };

	//Rotation information
	//counter clockwise roation about the vector
	float angle = 0;

	glm::vec3 Vector_Rot = glm::vec3(0.0, 1.0, 0.0);

	//Constructor
	Model(Full_Model* &Model, std::map<Name, Animator> &Animators, glm::vec3 Pos)
	{
		this->model = Model;
		this->animators = Animators;
		this->pos = Pos;
	}
	//Overloaded Constructor
	Model(Full_Model* &Model, glm::vec3 Pos)
	{
		this->model = Model;
		this->pos = Pos;
	}

};



//-------------------------------------------------
//Collision

enum Collision_Options
{
	STATIC=0,
	DYNAMIC=1,
	WALL = 2
};

typedef unsigned int Collision_State;


class Collision
{
public:
	//Reference back to the IEntity
	unsigned int Entity_ID = 0;
	//Reference back to the Model Data for this entity
	unsigned int Model_ID = 0;
	
	// Is the collision object statics or dynamic
	Collision_Options State = STATIC;

	//List of the vertices that are hashed in to the hash table
	std::vector<glm::vec3> List_Vertices;

	//AABB for this model
	AABB box;
	
	// Vertices that represent the AABB
	std::vector<float> points;

	//Original Center of the AABB box in model Coordinates
	glm::vec3 Original_Pos;

	// Bounding Sphere for this model
	ColSphere sphere;

	//Plane collision

};

// End of Collision
//-------------------------------------------------

class Health
{
public:
	unsigned int health;
};

class AI
{
	// AI for the enemies// Ben can do this, I can give him some tips from my expereince from my other game (Keeping enties from over lapping, center of mass for a collection of entites, addition of directional vectors)
};

class Player
{
	//information about player controls
	// camera control,ect...
};


//Input Data from a file for the components
Name File_Name(const char* inputpath); // Input name from file
Position File_Pos(const char* inputpath); // Input Position from file
Velocity File_Vel(const char* inputpath); // Input Velocity from file
Model File_Model(const char* inputpath);  // Input Model From file


#endif
