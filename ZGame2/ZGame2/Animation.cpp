#include "Animation.h"
#include "LinearInterpolation.h"
//----------------------------------------------------
//Animation Methods
Animation::Animation()
{

}

Animation::Animation(const char* name, Skeleton &skeleton, std::vector<std::pair<float, SkeletonPose>> keyframes, Type_of_Animation type)
{
	Name = name;
	pskeleton = &skeleton;
	KeyFrames = keyframes;
	this->Type = type;
}


//Methods for Animator

Animator::Animator()
{

}

Animator::Animator(Skeleton &skeleton, Animation &animation) :Current_Anim_Time(0.0)
{
	this->pskeleton = &skeleton;
	this->Cur_Anim = animation;
	this->End_Time = Cur_Anim.KeyFrames.back().first - 0.0;
}

void Animator::Start_Animation()
{
	this->Current_Anim_Time = 0.0;
	this->Animating = true;
}

void Animator::End_Animation()
{
	this->Animating = false;
}

std::vector<JointPose> Animator::New_Pose()
{


	std::vector<JointPose> Temp_Pose;					//this will be the Interpolated Quaternions/Pos  to make the Global Transforms
	Temp_Pose.reserve(this->pskeleton->JointCount);


	//First, we need to figure out where it is  between the key frames
	int k = 1;
	while (Cur_Anim.KeyFrames[k].first <Current_Anim_Time && k<Cur_Anim.KeyFrames.size()-1)
	{
		k += 1;
	}

	//Set beta, which will be the blend parameter
	float beta = (Current_Anim_Time - Cur_Anim.KeyFrames[k - 1].first) / (Cur_Anim.KeyFrames[k].first - Cur_Anim.KeyFrames[k - 1].first);

	//Lerp Method
	/*for (int j = 0; j < this->pskeleton->JointCount; ++j)
	{
		JointPose temp=Lerp(beta, this->Cur_Anim.KeyFrames[k-1].second.Poses_Joints[j], this->Cur_Anim.KeyFrames[k].second.Poses_Joints[j]);
		Temp_Pose.push_back(temp);


	}*/

	//Slerp Method, using Geodesics of the sphere
	for (int j = 0; j < this->pskeleton->JointCount; ++j)
	{
		
		JointPose temp = Slerp(beta, this->Cur_Anim.KeyFrames[k-1].second.Poses_Joints[j], this->Cur_Anim.KeyFrames[k].second.Poses_Joints[j]);
		Temp_Pose.push_back(temp);


	}


	return Temp_Pose;
}


std::vector<glm::mat4> Animator::Animate(float &deltatime)
{
	if (this->Animating)										//Is this animating?
	{
		this->Current_Anim_Time += deltatime;					//iterate the current animation time

		if (this->Cur_Anim.Type == LOOP)						//If loop animation, check if current animation time is above end time or not and deal with it according
		{
			if (this->Current_Anim_Time < this->End_Time)
			{
				return JointPoses_To_JointTransforms( New_Pose());
			}
			else
			{
				Start_Animation();
				return JointPoses_To_JointTransforms(New_Pose());
			}
		}

		if (this->Cur_Anim.Type == ONE_OFF)								//If the animation is oen_off, then end animation if current animation time is greater then end time
		{
			if (this->Current_Anim_Time < this->End_Time)
			{
				return JointPoses_To_JointTransforms(New_Pose());
			}
			else
			{
				End_Animation();
			}
		}
	}
	//This does not return a value if it exits (so be warned, this it not finished yet) (NOTE: to my self)
}


void Animator::Start_Blending(float &deltatime, Animator &other)
{
	this->Blending = true;
	this->Blending_Start = this->Current_Anim_Time;
	this->Blending_Parameter = 0.0;
	other.Start_Animation();
	other.Current_Anim_Time = this->Current_Anim_Time* other.End_Time / this->End_Time;
	
}	

void Animator::End_Blending()
{
	this->Blending = false;
	this->End_Animation();
}

std::vector<JointPose> Animator::Blend_Pose(float deltatime, Animator &other)
{
	
	std::vector<JointPose> Temp_Pose;					//this will be the Interpolated Quaternions/Pos  to make the Global Transforms
	Temp_Pose.reserve(this->pskeleton->JointCount);


	//First we need to figure out which key frames the first animation is in between
	int k = 1;
	while (Cur_Anim.KeyFrames[k].first < Current_Anim_Time && k<Cur_Anim.KeyFrames.size()-1)
	{
		k += 1;
	}

	//time parameter between 0 and 1
	float beta = deltatime;

	//Import the animation jointPoses for animation one and animation 2
	std::vector<JointPose> original = this->New_Pose();
	std::vector<JointPose> other_JP = other.New_Pose();

	//std::cout << beta << std::endl;
	//Slerp Method, using Geodesics of the sphere
	for (int j = 0; j < this->pskeleton->JointCount; ++j)
	{
		
		//JointPose temp = Lerp(beta, original[j], other_JP[j]);
		JointPose temp = Slerp(beta, original[j], other_JP[j]);
		Temp_Pose.push_back(temp);


	}
	
	

	return Temp_Pose;
}

/*
std::vector<glm::mat4>Animator::Blend_Animate(float &deltatime,Animator other)
{
	this->Current_Anim_Time += deltatime;
	Blending_Parameter += deltatime;
	other.Current_Anim_Time += deltatime;

	std::cout << "Deltatime" << deltatime << std::endl;
	std::cout << "other.CurrentTime" << other.Current_Anim_Time << std::endl;
	std::cout << "this->Current TIme" << this->Current_Anim_Time << std::endl;

	if (this->Blending && Blending_Parameter<=1.0)			
	{
		
		

		if (this->Cur_Anim.Type == LOOP)
		{
			if (this->Current_Anim_Time < this->End_Time)
			{
				return JointPoses_To_JointTransforms(Blend_Pose( other));
			}
			else
			{
				this->Current_Anim_Time = 0.0;
				return JointPoses_To_JointTransforms(Blend_Pose( other));
			}
		}
		else
		{
			if (this->Current_Anim_Time < this->End_Time)
			{
				return JointPoses_To_JointTransforms(Blend_Pose( other));
			}
			else
			{
				this->Current_Anim_Time = this->End_Time;
				return JointPoses_To_JointTransforms(Blend_Pose( other));
			}
		}
	}
	else
	{
		this->End_Blending();
	}
}*/


std::vector<glm::mat4> JointPoses_To_JointTransforms(std::vector<JointPose> jointposes)
{
	std::vector<glm::mat4> New_Inter_Global_Tran;

	for (unsigned int k = 0; k < jointposes.size(); ++k)			//Make sure to compile each Jointpose's Matrix transform.
	{
		jointposes[k].Compile_Transform();
	}

	//Make these Quarternions in to Matrix transforms
	for (int i = 0; i < jointposes.size(); ++i)
	{
		glm::mat4 new_matrix;
		Joint_to_Root_Transform(jointposes, jointposes[i], new_matrix);

		New_Inter_Global_Tran.push_back(new_matrix);

	}
	return New_Inter_Global_Tran;
}