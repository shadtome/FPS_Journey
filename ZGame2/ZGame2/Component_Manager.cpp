
#include "Component_Manager.h"



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
	E_Col.Data[entity.Col_ID].Entity_ID = entity.ID;
	E_Col.Data[entity.Col_ID].Model_ID = entity.Model_ID;


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