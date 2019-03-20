#ifndef SYNCFADE_H
#define SYNCFADE_H

#include "Animation.h"
#include "Skeleton.h"

//given n animations that are similar to each other, sync them together and blend them at the same time as they play through their animations

class SyncAnim
{
private:
	std::vector<Animator>	List_Anim;			//List of animations

	float Blend_Ratio;							//Blend ratio to blend between which two neighboring animations

	float Blend_Partition=0;						//Which blend partition it is in, where we partitioned the interval [0,n-1]

	float Speed = 1.0;							//How fast the root animation goes, which changes the children animations

	std::vector<float> Blend_Param;				//Blend parameters between neigbhoring animations

	std::vector<float> Playback_Speed;			//Playback speed for each animation.  These are based on the relationship between neighboring animations

public:
	//Constructors
	SyncAnim();
	

	//DataStructure Methods
	void Insert(Animation anim);				//Order Matters, meaning 0th animation is the first, and so on

	//methods
	std::vector<glm::mat4> Animate(float &deltatime);	//Animate all the animations, takes as input the delta change of the animation time

	void Start_Animation();								//Start the animation

	void Change_Blend_Ratio(float time);				//Change the Blend ratio, which afffects how two neighboring animations weights

private:
	SkeletonPose New_Pose();					//New pose given based on two neighboring animations

};




#endif
