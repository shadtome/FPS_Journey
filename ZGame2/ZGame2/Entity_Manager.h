#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <iostream>
#include <map>
#include <stdio.h>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <cmath>
#include "Components.h"
#include "Attributes.h"


// Need to add the specific functions for Model and Collision

// Entities stuff
//This is the Unique ID for a Entity
class IEntity
{
public:
	unsigned int ID = 0; // This has 32 bits for use in bitmasking
	//Name ID
	unsigned int Name_ID = 0;
	// Position ID
	unsigned int Pos_ID = 0; // For these It might be best to try to use a good has function (use a prime number)
	//Velocity ID
	unsigned int Vel_ID = 0;
	// Model ID
	unsigned int Model_ID = 0;
	// Collision ID
	unsigned int Col_ID = 0;
};




// Component managment

template<class T>
class Components
{
public:
	// Holds the data for this Component
	std::vector<T> Data; // Lets see if we can get out of using the map and use a vector(more linear look up)(I will create another version)

};




// Component Managment
class Component_Manager
{
public:
	// Have this hold all the maps for each component.  This class will hold them all and able to access them.
	// Name data
	Components<Name> E_Name;
	// Position data for all entities
	Components<Position> E_Pos;   

	// Velocity Data for all entities
	Components<Velocity> E_Vel; 

	// Model Data for entities
	Components<Model> E_Model;

	// Collision Data for Entities
	Components<Collision> E_Col;



	//Use a Vector of components,

	//Big method so everything is simple
	void Delete_Data(IEntity &entity, Comp_Mask comp);

	// Input Data for the Components
	//-----------------------------
	// Position
	void Input_Pos(IEntity &entity, Position pos_data);
	// Velocity
	void Input_Vel(IEntity &entity, Velocity vel_data);
	//NAME
	void Input_Name(IEntity &entity, Name name_data);
	// Model
	void Input_Model(IEntity &entity, Model model_data); 
	// Collision
	void Input_Col(IEntity &entity, Collision col_data); // NEED TO FINISH THIS FUNCTION

	//Template version ?? This might work better instead of repeating Code, if I can some how compactify the data for the COmponents
	template<class T>
	void Input_Component(IEntity &entity, T T_Data);

	//Input Data for the Components
	void Input_Comp_Data(IEntity &entity, Comp_Mask comp, const char* inputpath);

};

// Methods for Component Manager
void Component_Manager::Delete_Data(IEntity &entity, Comp_Mask comp)
{
	if (comp == NAME)
		entity.Name_ID = 0;; //Delete the Data associted with this entity
	if (comp == POSITION)
		entity.Pos_ID = 0; //Delete the Data associted with this Entity
	if (comp == VELOCITY)
		entity.Vel_ID = 0; //Delete the Data associted with this Entity
	if (comp == MODEL)
		entity.Model_ID = 0; // Delete the ID location for the model
}

void Component_Manager::Input_Pos(IEntity &entity, Position pos_data)
{
	entity.Pos_ID = E_Pos.Data.size();
	E_Pos.Data.push_back(pos_data);
	E_Pos.Data[entity.Pos_ID].Entity_ID = entity.ID;
};

void Component_Manager::Input_Vel(IEntity &entity, Velocity vel_data)
{
	entity.Vel_ID = E_Vel.Data.size();
	E_Vel.Data.push_back(vel_data);
	E_Vel.Data[entity.Vel_ID].Entity_ID = entity.ID;
}

void Component_Manager::Input_Name(IEntity &entity, Name name_data)
{
	entity.Name_ID = E_Name.Data.size();
	E_Name.Data.push_back(name_data);
	E_Name.Data[entity.Name_ID].Entity_ID = entity.ID;
}

void Component_Manager::Input_Model(IEntity &entity, Model model_data)
{
	entity.Model_ID = E_Model.Data.size();
	E_Model.Data.push_back(model_data);
	E_Model.Data[entity.Model_ID].Entity_Vel_ID = entity.Vel_ID;
	E_Model.Data[entity.Model_ID].Entity_ID = entity.ID;
}

void Component_Manager::Input_Col(IEntity &entity, Collision col_data)
{
	entity.Col_ID = E_Col.Data.size();
	E_Col.Data.push_back(col_data);
	E_Model.Data[entity.Model_ID].Entity_Col_ID = entity.Col_ID;
}

void Component_Manager::Input_Comp_Data(IEntity &entity, Comp_Mask comp, const char* inputpath)
{
	if (comp == NAME)
	{
		Input_Name(entity, File_Name(inputpath));
	}
	if (comp == POSITION)
	{
		Input_Pos(entity, File_Pos(inputpath));
	}
	if (comp == VELOCITY)
	{
		Input_Vel(entity, File_Vel(inputpath));  
	}
	// Later ADD the one for MODEL AND COLLISION
}








// Entity managment
 // Makes the entities and gives them a unique ID
class
	Entity_Manager
{
public:
	//Collection of all the Entities
	std::map<unsigned int, Attributes> World;

	// Class that holds all the data for the components for each Entity
	Component_Manager components;


	//Ids for the entities
	unsigned int LastID = 0;

	//Constructor

	// Create Entities and puts the unsigned int in to the world vector
	void Create_Entity();
	//Destry Entity
	void Destroy_Entity(IEntity &entity); // Need a good way to destroy entities in a efficient Manner
	//INclude Entity
	void Include_Entity(IEntity &entity, Attributes &attributes);

	//Check if the Entity Already exists
	bool Check_Exist(IEntity &entity);
	//----------------------------------
	// Get options for this Entity
	bool Get_Options(IEntity &entity, Comp_Mask component);
	//---------------------------------
	// Input Data for the Entities
	void Input_Data(IEntity &entity, const char* inputpath); // Need a way to import information from a file to make things easier
	// This is to import the information about the model (since the File importer for this is not finished yet)
	void Input_Model(IEntity &entity, Model model);

};

//Methods for the Entity_Manager Class
void Entity_Manager::Create_Entity()
{
	IEntity temp;
	Attributes temp_A;
	temp.ID = LastID;
	LastID += 1;
	World[temp.ID] = temp_A;

};

void Entity_Manager::Destroy_Entity(IEntity &entity)
{
	std::string name = components.E_Name.Data[entity.Name_ID].name;
	unsigned int id = entity.ID;
	Comp_Mask bit = 1;
	while (bit != 0)
	{
		if (Get_Options(entity, bit))
		{
			components.Delete_Data(entity, bit); // Destroy the data for the components associated with this entity.
		}
		bit = bit << 1;
	}

	//Destory the Attributes in the map
	World.erase(entity.ID);
	//Lets check to see if the Entity still exists to make sure it was successfully deleted
	if (Check_Exist(entity))
	{
		//std::cout << entity.ID << "(" << components.E_Name.Data[entity.Name_ID].name << ")" << " did not delete"<< std::endl;
	}
	//else
	  //std::cout << id << "(" << name << ")" << " was successfully deleted" << std::endl;

}

void Entity_Manager::Include_Entity(IEntity &entity, Attributes &attributes)
{
	if (!Check_Exist(entity))
	{
		LastID += 1;
		entity.ID = LastID;

		World[entity.ID] = attributes;
	}
	else
	{
		//std::cout << "The entity" << entity.ID << "(" << components.E_Name.Data[entity.Name_ID].name << ")" << " " << "already exists"<< std::endl;
	}
};

bool Entity_Manager::Check_Exist(IEntity &entity)
{
	bool exists = false;
	for (auto key = World.cbegin(); key != World.cend(); ++key)
	{
		if (key->first == entity.ID)
			exists = true;
	}
	return exists;
}

bool Entity_Manager::Get_Options(IEntity &entity, Comp_Mask component)
{
	return World[entity.ID].Is_Opt_On(component);
}


void Entity_Manager::Input_Data(IEntity &entity, const char* inputpath)
{
	Comp_Mask bit = 1;
	while (bit != 0)
	{
		if (Get_Options(entity, bit))
		{
			components.Input_Comp_Data(entity, bit, inputpath);
		}
		else
		{
			//std::cout << "This Entity"<< " "<< entity.ID<< "("<< components.E_Name.Data[entity.Name_ID].name<< ")" << " " << "does not have component" << " "<< log2(bit)+1 << " "<< "active" << std::endl;
		}
		bit = bit << 1;
	}
}

void Entity_Manager::Input_Model(IEntity &entity, Model model)
{
	components.Input_Model(entity, model);
}

// Lets try to make a systems Managment
// Systems

//Change Position System
void Change_Position(Position &pos, Velocity &vel)
{
	pos.x += vel.x;
	pos.y += vel.y;
	pos.z += vel.z;

}
// Change Something using compeonts and the functions with those COmponents
void Change_Pos_World(Entity_Manager &world)
{
	for (int k = 0; k < world.components.E_Vel.Data.size(); ++k)
	{
		Change_Position(world.components.E_Pos.Data[k], world.components.E_Vel.Data[k]);
	}
}




#endif
