#ifndef ANIMATION_H
#define ANIMATION_H

#include <iostream>
#include "Skeleton.h"
#include "Quaternion.h"

enum Type_of_Animation											//Types of animation, like a looping animation or not
{
	LOOP,
	ONE_OFF
};

//Class that holds the keyframes for a animation.
struct Animation
{
	const char* Name;  //Name of the animation

	Skeleton* pskeleton;  // Pointer to skeleton this animation is connected to , later we will use Unique IDS in the Entity_Manager

	std::vector<std::pair<float,SkeletonPose>> KeyFrames; // key frames ordered <timeStamp, pose> 

	//Type of animation loop?, a one off, ect...
	Type_of_Animation Type;

	//Make a Constructor
	//Default Constructor
	Animation();
	Animation(const char* name, Skeleton &skeleton, std::vector<std::pair<float,SkeletonPose>> keyframes,Type_of_Animation type);
};


//Class that takes a Animation and sets it up for animating, and has functions that can blend with other animaters.
//Also blends between key frames


class Animator
{
public:
	Skeleton* pskeleton; //Pointer to skeleton this is associated to

	Animation Cur_Anim;  // Current animation

	float Current_Anim_Time;	// current animation time
	float End_Time;				//Local End_Time

	bool Animating=false;     // Is this animating?
	bool Blending = false;		//Is this blending?

	float Blending_Start;
	float Blending_Parameter = 0;

	//Constructors
	Animator(Skeleton &skeleton, Animation &animation);
	Animator();

	//Methods
public:
	void Start_Animation();													//Start animation
	void End_Animation();													//End animation
	std::vector<glm::mat4> Animate(float &deltatime);						//go through the process iterating the current animation time and figure out what to do with it (find new pose or stop)
	void Start_Blending(float &deltatime, Animator &other);					// Start blending with another animator
	void End_Blending();													//End blending effect
	std::vector<glm::mat4> Blend_Animate(float &deltatime, Animator other);	

	//New Blend Pose
	std::vector<JointPose> Blend_Pose(float deltatime, Animator &other);	//Find the new interpolated pose between this animator and the other


private:
	//New Pose while the animation is going
	std::vector<JointPose> New_Pose();										// New pose based on the two key frames that the current animation time is between
	

	
};


//Change Vector of JointPoses to vector of matrix transforms
std::vector<glm::mat4> JointPoses_To_JointTransforms(std::vector<JointPose> jointposes);


#endif
