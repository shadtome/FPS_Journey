

#include "AnimationSystem.h"


void Update_Animation(Entity_Manager &world, unsigned int entity_id, SkeletonPose &pose)
{
	world.Update_Animation(entity_id, pose);
}

void Update_Animation(Entity_Manager &world, IEntity* &entity_ptr, SkeletonPose &pose)
{
	world.Update_Animation(entity_ptr->ID, pose);
}
