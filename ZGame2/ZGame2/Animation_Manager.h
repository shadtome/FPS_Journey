#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#include <vector>
#include <map>
#include "Animation.h"

//this is where we will be able to have a manager of the animations all in one place, my goal is to have each entity have a animation component
//that holds the information there, and it will be able to access the other components

//This holds the animators for the animations
class Animation_Manager
{
	std::map<const char*, Animator> List_Animators;			//Holds them by their name

	//Methods

	Animator Grab_Animator(const char* name);					//Search and grab animator from the list
	void Delete(const char* name);							// Delete animator from list
	void Add(Animation anim);							// Add animation to list
	void Start_Animation(const char* name);							// start the animation for the specifed animation
	void End_Animation(const char* name);						//End the specific animation
	std::vector<glm::mat4> Animate(const char* name, float &time);							//Animate the specific animation
	std::vector<glm::mat4> Blend(float &time,const char* name1, const char* name2);				//blend the two animations together

};





#endif
