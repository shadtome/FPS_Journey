#include "Animation_Manager.h"


//source file for Animation Manager

Animator Animation_Manager::Grab_Animator(const char* name)
{
	return List_Animators[name];
}

void Animation_Manager::Delete(const char* name)
{
	this->List_Animators.erase(name);
}

void Animation_Manager::Add(Animation anim)
{
	Animator new_animator(*anim.pskeleton, anim);
	this->List_Animators[anim.Name] = new_animator;
}

void Animation_Manager::Start_Animation(const char* name)
{
	this->List_Animators[name].Start_Animation();
}

void Animation_Manager::End_Animation(const char* name)
{
	this->List_Animators[name].End_Animation();
}

std::vector<glm::mat4> Animation_Manager::Animate(const char* name, float &time)
{
	return this->List_Animators[name].Animate(time);
}

/*
std::vector<glm::mat4> Animation_Manager::Blend(float &time, const char* name1, const char* name2)
{
	return this->Grab_Animator(name1).Blend_Animate(time,this->Grab_Animator(name2));
}
*/