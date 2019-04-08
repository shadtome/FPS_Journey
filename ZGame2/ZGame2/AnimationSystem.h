#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include "Components.h"
#include "Entity_Manager.h"

//This is the Animation system
/*
* All the methods to animations, like updateing, blending with the current animation, adding to it, ect...
*/

//Replace the current SkeletonPose with this new skeletonPose
void Update_Animation(Entity_Manager &world, unsigned int entity_id, SkeletonPose &pose);
void Update_Animation(Entity_Manager &world, IEntity* &entity_ptr, SkeletonPose &pose);

//Blend new animation to the current skeletonpose saved in Model.
void Blend_Animation(Model &model, SkeletonPose &pose);


#endif
