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
#include "Component_Manager.h"
#include "Attributes.h"
#include "Grid_Col.h"


//CHANGE THIS WITH ITERATORS FOR EACH OF THE COMPONENTS TO MAKE IT EASIER!!!!!!! note to my self

//This is the entity manager, that keeps track of all entities in the game, that is represented by a unique ID.  
//For example, a zombie, is represetned as a unique ID

//Add method for Entity_Manager to do everything (draw entities, check collision, animate entities, ect..)



//Forward Decelerations


// Entity managment
 // Makes the entities and gives them a unique ID
class
	Entity_Manager
{
public:
	//Collection of all the Entities
	std::map<unsigned int, IEntity> World;

	// Class that holds all the data for the components for each Entity
	Component_Manager components;

	//Grid that partittions the the space in to uniform grid (static objects dont change their grid location, while the dynamic ones do)
	//Grid grid;


	//Ids for the entities
	unsigned int LastID = 0;

	//Constructor

	// Create Entities and puts the unsigned int in to the world vector
	IEntity* Create_Entity();
	//Destry Entity
	void Destroy_Entity(IEntity &entity); // Need a good way to destroy entities in a efficient Manner
	//INclude Entity
	IEntity* Include_Entity(IEntity &entity);

	//Check if the Entity Already exists
	bool Check_Exist(IEntity &entity);
	//----------------------------------
	// Get options for this Entity
	bool Get_Options(IEntity &entity, Comp_Mask component);
	//Check if option is turned on for this Entity
	bool Is_Opt_On(IEntity &entity,Options opt);
	bool Is_Opt_On(unsigned int &ID, Options opt);
	//---------------------------------
	// Input Data for the Entities
	void Input_Data(IEntity &entity, const char* inputpath); // Need a way to import information from a file to make things easier
	// This is to import the information about the model (since the File importer for this is not finished yet)
	void Input_Model(IEntity &entity, Model model);
	//This is to import the information about the Collision data
	void Input_Col(IEntity &entity, Collision col);

	//----------------------------------------
	//Update Functions
	void Update_Animation(unsigned int &ID, SkeletonPose &pose);


	//Activate the world (draw, collision, animate ect...)
	void Activate();



	//Set Grid Up with all the static Entities (Make sure you do this last after all the Initalizations
	//void Setup_Grid();
};





#endif
