#ifndef ANIMATIONSTATE_H
#define ANIMATIONSTATE_H

#include "Animation.h"

//this class is a Double-Linked List, where the root node is the basic animation, and it continous through the list in order of animations that must blend with each other to get to the next animation
// for example: the moving Animation Tree is Root: Idle, the next to the right is Walking, and the node connected to walking is Running
//This is a way to have structure between animations and the transitions between them
//There is no natural ordering for the animations, I would have to order it my self when putting things in, unless I had GUI for this




class AnimationTree
{
public:
	std::vector<Animator> Ordered_List;
	unsigned int State = 0;
	unsigned int New_State=0;

	//Constructor
	AnimationTree() { };
	//Methods 
	void Insert_End(Animator animator);			//Insert the next animation in the list
	void Insert_Beg(Animator animator);
	void Insert_Pos(Animator animator);
	void Delete_End(Animator animator);			//Delete the animation from the list
	void Delete_First(Animator animator);
	void Delete_Pos(Animator animator);
	unsigned int Search(const char*);

	//Animation with this state
	void Change_State(float deltatime, const char* name);
	std::vector<glm::mat4> Animate_State(float deltatime);


};






#endif