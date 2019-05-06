
#include "Entity_Manager.h"


//Source code for the functions of Entity_Manager

IEntity* Entity_Manager::Create_Entity()
{
	IEntity temp;
	temp.ID = LastID;
	LastID += 1;
	World[temp.ID] = temp;
	return &World[temp.ID];
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

IEntity* Entity_Manager::Include_Entity(IEntity &entity)
{
	if (!Check_Exist(entity))
	{
		LastID += 1;
		entity.ID = LastID;

		World[entity.ID] = entity;
	}
	else
	{
		//std::cout << "The entity" << entity.ID << "(" << components.E_Name.Data[entity.Name_ID].name << ")" << " " << "already exists"<< std::endl;
	}
	return &World[entity.ID];
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
	return World[entity.ID].attributes.Is_Opt_On(component);
}

bool Entity_Manager::Is_Opt_On(IEntity &entity, Options opt)
{
	return World[entity.ID].attributes.Is_Opt_On(opt);
}

bool Entity_Manager::Is_Opt_On(unsigned int &ID, Options opt)
{
	return World[ID].attributes.Is_Opt_On(opt);
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

void Entity_Manager::Input_Col(IEntity &entity, Collision col)
{

	components.Input_Col(entity, col);

}

//-----------------------------------------------------------
//Access Data

//Model
Model* Entity_Manager::access_model(IEntity* entity_ptr)
{
	
	return &components.E_Model.Data[entity_ptr->Model_ID];
}
Model* Entity_Manager::access_model(unsigned int ID)
{
	return &components.E_Model.Data[World[ID].Model_ID];
}






//-----------------------------------------
//Update Functions

//Update Animation
void Entity_Manager::Update_Animation(unsigned int &ID, SkeletonPose pose)
{
	components.E_Model.Data[World[ID].Model_ID].Cur_Pose = pose;
}
void Entity_Manager::Update_Animation(IEntity* &entity_ptr, SkeletonPose pose)
{
	components.E_Model.Data[entity_ptr->Model_ID].Cur_Pose = pose;
}

//Update Position
void Entity_Manager::Set_Position(unsigned int &ID, glm::vec3 pos)
{
	components.E_Model.Data[World[ID].Model_ID].pos = pos;
}
void Entity_Manager::Set_Position(IEntity* &entity_ptr, glm::vec3 pos)
{
	components.E_Model.Data[entity_ptr->Model_ID].pos = pos;
}

void Entity_Manager::Update_Position(unsigned int& ID, glm::vec3 pos)
{
	components.E_Model.Data[World[ID].Model_ID].pos +=pos;
}
void Entity_Manager::Update_Position(IEntity* &entity_ptr, glm::vec3 pos)
{
	components.E_Model.Data[entity_ptr->Model_ID].pos +=pos;
}


//-----------------------------------------------------------
//Animation Functions
//Blend Animation
void Entity_Manager::Blend_Animation(unsigned int entity_id, float beta, SkeletonPose &pose)
{
	unsigned int model_id = World[entity_id].Model_ID;
	this->Update_Animation(entity_id, Blend_Poses(components.E_Model.Data[model_id].Cur_Pose, pose, beta));
}
void Entity_Manager::Blend_Animation(IEntity* &entity_ptr, float beta, SkeletonPose &pose)
{
	this->Update_Animation(entity_ptr->ID, Blend_Poses(components.E_Model.Data[entity_ptr->Model_ID].Cur_Pose, pose, beta));
}


/*void Entity_Manager::Setup_Grid()
{
	grid.Grid.reserve(pow(2, components.E_Col.Data.size()));
	for (int k = 0; k < components.E_Col.Data.size(); ++k)
	{
		if (components.E_Col.Data[k].State == STATIC)
		{
			grid.Input_Into_Grid(components.E_Col.Data[k]);
		}
	}
}*/

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
	for (unsigned int k = 0; k < world.components.E_Vel.Data.size(); ++k)
	{
		Change_Position(world.components.E_Pos.Data[k], world.components.E_Vel.Data[k]);
	}
}