#ifndef BLENDERS_H
#define BLENDERS_H

#include <map>
#include "Skeleton.h"
#include "Animation.h"

/*This header file is for defining blending functions/Classes for two or more animations(animators)
* Define 1dim blending, 2 dim blending,difference blending, ect...
* this will help blend animations for different parts of a body.
* for example, running and strafing while looking around with a gun
* this requires blending 4 animations or more depending on the context 
( two for looking and two for the running and strafing)
*/


//-------------------------------------------------
//One dimensional Lerp blending
//This is the generalization of the SyncAnime class
class onedim_Blend
{
private:
	//Animations with pescribed locations on the real line
	std::map<float,Animator> anims;

	//current position on the real line
	float blend_ratio;

	//Current subinterval we are in.  We are partition the interval [first animation time, last animation time] in the anims map
	float blend_partfirst;
	float blend_partlast;

	//Last time
	float last=0;

	//first time
	float first=0;

	//Current Speed of the animation (this corresponds to the fact that as we blend between animations, we have to change the speed.
	//Since they are running on different intervals.
	float speed=1.0;
public:
	//Constructors
	onedim_Blend() {};

	//Add animations
	//pos is the pos on the real line.
	void insert(Animator &anim, float pos);

	//Animation functions
	SkeletonPose Animate(float &deltatime);	//Animate all the animations, takes as input the delta change of the animation time

	void Start_Animation();								

	//Change the Blend ratio, which afffects how two neighboring animations weights
	void Change_Blend_Ratio(float time);				

private:
	//New pose given based on two neighboring animations
	SkeletonPose New_Pose();					
};



//-------------------------------------------------------------------
//2 dim blend
class twodim_Blend
{

};


#endif
