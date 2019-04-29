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

//Forward decelerations
class Square_twodim_Blend;

//-------------------------------------------------
//One dimensional Lerp blending
//This is the generalization of the SyncAnime class
class onedim_Blend
{
	friend Square_twodim_Blend;
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
	void Change_Blend_Ratio(float delta);
	//Set the blend ratio to a certain number.
	void Set_Blend_Ratio(float delta);

	float Cur_Blend_Ratio();

private:
	//New pose given based on two neighboring animations
	SkeletonPose New_Pose();					
};



//-------------------------------------------------------------------
//2 dim blend

/* We are blending 4 different animations (good for looking around animation ect..)
 we can imagine these animations as corners of a square 

		A ----------AB----------B
		-			-		    -
		-			-			-
		-			ABCD		-
		-			-			-
		-			-			-
		C------------CD---------D
		where AB is the 1-dim blend between A and B, same for CD. ABCD is the 1-dim blend between AB and CD.

	We will blend from top to bottom i.e. (beta*bottom +(1-beta) * top)
	Only works right now with animations that are exactly the same on the real line
*/

class Square_twodim_Blend
{

	//Just need the blend in the y direction, as the x direction blends are embedded in the onedim blends
	float blend_y;

	/* Two one-dim blends, both have the same number of elements
	and both have their animations at the same times(i don't think I need this)
	*/
	onedim_Blend Top_b;
	onedim_Blend Bot_b;

	//Speed of the blended animations
	//This is a result of blending between the top speed and the bottom speed.
	float speed;

	//Current animation time
	float Cur_time;

public:
	//constructor 
	Square_twodim_Blend() {};

	//Insert animation to the top of the square (in order A B) from left to right.
	void Top_insert(Animator &anim, float pos);
	//Insert animation to the bottom of the square from left to right.
	void Bot_insert(Animator &anim, float pos);

	//Animation functions
	SkeletonPose Animate(float &deltatime);	//Animate all the animations, takes as input the delta change of the animation time

	void Start_Animation();

	//Change the Blend ratio, which afffects how two neighboring animations weights
	void Change_Blend_Ratio(float delta_x,float delta_y);
	//This sets the blend ratio to which ever parameter. (between 0 and 1)
	void Set_Blend_Ratio(float delta_x, float delta_y);

private:
	//New pose given based on two neighboring animations
	SkeletonPose New_Pose(SkeletonPose Top,SkeletonPose Bot);

};



//-------------------------------------
//Triangular two-dimensional Lerp Blending

/* This is used to blend a arbitrary number of animations at arbitrary locations in the plane.  
* The follow class is used to blend three animations based on barycentric coordinates for a simplex.
* This used Lerp instead of Slerp to do the interpolation, since Lerp can be generalized to any simplex.
* IMPLEMENT THIS LaTER
*/

class Triangle_twodim_Blend
{
	//Two blend factors
	float blend_a;
	float blend_b;


};



//----------------------------------------
//Additive Blending

/* This is used to add effects to already stable animations.  For example, a running animation, we can "add" a hurt animation using
* a additive blending.

* One must input a Source animation, and a reference animation to get the difference pose,
Then we can "act" this on another animation to give it this difference effect.
* Right now, the differencepose is in SkeletonPose format with SQT, but might need it to be in matrix format.
* 
*/

class Difference_Animator
{
	//NOTE: Both source and reference have to have the same duration, exactly!
	//Source Clip
	Animator source;

	//Reference Clip
	Animator reference;

	//Blend factor
	float beta;

public:
	//Constructor
	//NOTE: Both the source and reference animators must have the same duration!!
	Difference_Animator(Animator &source, Animator &reference);

	//Add difference pose to other pose, with blend factor beta
	//NOTE: the target animation does not have to have the same duration, or even related to this difference clip.

	SkeletonPose Add_Anim(Animator &other,float &deltatime);
	SkeletonPose Add_Pose(SkeletonPose &target, float &deltatime);

	//Change blend factor
	void Change_blend_factor(float &deltabeta);

	//Set blend factor
	void Set_blend_factor(float &Beta);
	//Start adding animation with blend factor beta
	void start(float Beta);

	//Turn off adding animation
	void stop();



private:
	SkeletonPose Setup_Pose(float &deltatime);


};






#endif
