#ifndef ANIMATION_H
#define ANIMATION_H

#include <iostream>
#include "Skeleton.h"
#include "Quaternion.h"

//Class that holds the key frames for the animation
struct Animation
{
	const char* Name;  //Name of the animation

	Skeleton* pskeleton;  // Pointer to skeleton this animation is connected to , later we will use Unique IDS in the Entity_Manager

	std::vector<std::pair<float,SkeletonPose>> KeyFrames; // key frames ordered <timeStamp, pose> 

	//Make a Constructor
	//Default Constructor
	Animation();
	Animation(const char* name, Skeleton &skeleton, std::vector<std::pair<float,SkeletonPose>> keyframes);
};

//----------------------------------------------------
//Animation Methods
Animation::Animation()
{

}

Animation::Animation(const char* name, Skeleton &skeleton, std::vector<std::pair<float, SkeletonPose>> keyframes)
{
	Name = name;
	pskeleton = &skeleton;
	KeyFrames = keyframes;
}


//Animator, has the information about the animation, calculates the current pose through the interpolation of the keyframes

class Animator
{
public:
	Skeleton* pskeleton; //Pointer to skeleton this is associated to

	Animation Cur_Anim;  // Current animation

	float Current_Anim_Time;	// current animation time
	float Start_Time;          //Global Start Time;
	float End_Time;				//Local End_Time

	bool Started = false;     // Is this animating?
	
	//Make a Constructor
	Animator(Skeleton &skeleton, Animation &animation);

	//Methods
	//This takes in the time, and figures out what the new global joint transforms have to be.
	//Start the Animation
private:
	void Start_Animation(float &time);
	void End_Animation();

public:
	//New Pose while the animation is going
	std::vector<glm::mat4> New_Pose(float time);

	//End the Animation ?
	
};


//Methods for Animator

Animator::Animator(Skeleton &skeleton, Animation &animation) :Current_Anim_Time(0.0)
{
	this->pskeleton = &skeleton;
	this->Cur_Anim = animation;
	this->End_Time = Cur_Anim.KeyFrames.back().first - 0.0;
}

void Animator::Start_Animation(float &time)
{
	this->Start_Time = time;
	this->Started = true;
}

void Animator::End_Animation()
{
	this->Started = false;
	Current_Anim_Time = 0.0;
	
}

std::vector<glm::mat4> Animator::New_Pose(float time)
{
	if (this->Started == false)
	{
		Start_Animation(time);
	}

	this->Current_Anim_Time = time - this->Start_Time;

	if (this->Current_Anim_Time > this->End_Time)
	{
		End_Animation();
	}

	std::vector<JointPose> Temp_Pose;					//this will be the Interpolated Quaternions/Pos  to make the Global Transforms
	Temp_Pose.reserve(this->pskeleton->JointCount);

	std::vector<glm::mat4> New_Inter_Global_Tran;		//This will be the resulting Joint Transforms
	New_Inter_Global_Tran.reserve(this->pskeleton->JointCount);

	//First, we need to figure out where it is  between the key frames
	int k = 1;
	while (Cur_Anim.KeyFrames[k].first < Current_Anim_Time)
	{
		k += 1;
	}

	float beta = (Current_Anim_Time - Cur_Anim.KeyFrames[k - 1].first) / (Cur_Anim.KeyFrames[k].first - Cur_Anim.KeyFrames[k - 1].first);

	//Lerp Method
	/*for (int j = 0; j < this->pskeleton->JointCount; ++j)
	{
		//Temporary quarternions/Pos
		Quarternion temp_quat;
		glm::vec3 temp_pos;

		//look at (1-beta) PoseJoint[k-1]+beta(PoseJoint[k]
		temp_quat = this->Cur_Anim.KeyFrames[k].second.Poses_Joints[j].Rot_Quat*(beta)+this->Cur_Anim.KeyFrames[k - 1].second.Poses_Joints[j].Rot_Quat*(1 - beta);

		temp_pos = this->Cur_Anim.KeyFrames[k].second.Poses_Joints[j].Pos_in_Parent*(beta)+this->Cur_Anim.KeyFrames[k - 1].second.Poses_Joints[j].Pos_in_Parent*(1 - beta);

		JointPose temp (temp_quat, temp_pos, this->pskeleton->Vector_Joints[j]);
		temp.Compile_Transform();
		Temp_Pose.push_back(temp);

		
	}*/
	
	//Slerp Method, using Geodesics of the sphere
	for (int j = 0; j < this->pskeleton->JointCount; ++j)
	{
		//Temporary quarternions/Pos
		Quarternion temp_quat;
		glm::vec3 temp_pos;
		float angle = acos(glm::dot(this->Cur_Anim.KeyFrames[k].second.Poses_Joints[j].Rot_Quat.Vector,this->Cur_Anim.KeyFrames[k - 1].second.Poses_Joints[j].Rot_Quat.Vector));
		//look at (1-beta) PoseJoint[k-1]+beta(PoseJoint[k]
		temp_quat = this->Cur_Anim.KeyFrames[k].second.Poses_Joints[j].Rot_Quat*(sin(glm::radians(angle)*beta)/sin(glm::radians(angle)))+this->Cur_Anim.KeyFrames[k - 1].second.Poses_Joints[j].Rot_Quat*(sin(glm::radians(angle)*(1-beta)) / sin(glm::radians(angle)));

		temp_pos = this->Cur_Anim.KeyFrames[k].second.Poses_Joints[j].Pos_in_Parent*(beta)+this->Cur_Anim.KeyFrames[k - 1].second.Poses_Joints[j].Pos_in_Parent*(1 - beta);

		JointPose temp(temp_quat, temp_pos, this->pskeleton->Vector_Joints[j]);
		temp.Compile_Transform();
		Temp_Pose.push_back(temp);


	}

	for (int i = 0; i <Temp_Pose.size(); ++i)
	{
		glm::mat4 new_matrix;
		Joint_to_Root_Transform(Temp_Pose, Temp_Pose[i], new_matrix);

		New_Inter_Global_Tran.push_back(new_matrix);

	}

	
	return New_Inter_Global_Tran;
}





#endif
