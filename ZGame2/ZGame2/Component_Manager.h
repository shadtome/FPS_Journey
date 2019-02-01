#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H

#include "Components.h"
#include "IEntity.h"

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
	//template<class T>
	//void Input_Component(IEntity &entity, T T_Data);

	//Input Data for the Components
	void Input_Comp_Data(IEntity &entity, Comp_Mask comp, const char* inputpath);

};




#endif
