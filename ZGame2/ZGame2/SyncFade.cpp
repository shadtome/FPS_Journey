
#include "SyncFade.h"


//Make sure to document this code, since there is no information here

SyncAnim::SyncAnim()
{

}

void SyncAnim::Insert(Animation anim)
{
	this->List_Anim.push_back(Animator(*anim.pskeleton, anim));
	Blend_Param.push_back(0.0);
	if (this->List_Anim.size() > 1)
	{
		this->Playback_Speed.push_back(1 - this->List_Anim.back().End_Time / this->List_Anim[this->List_Anim.size() - 2].End_Time);
		std::cout << "Animation Time" << anim.KeyFrames.front().first <<"::"<< anim.KeyFrames.back().first << std::endl;
		std::cout << "CALCULATION" << 1 - this->List_Anim.back().End_Time / this->List_Anim[this->List_Anim.size() - 2].End_Time << std::endl;
	}
	else
	{
		this->Playback_Speed.push_back(1.0);
	}
}


std::vector<glm::mat4> SyncAnim::Animate(float &deltatime)
{
	this->List_Anim[0].Current_Anim_Time += this->Speed *deltatime;		//Set the first animation and change its current animation time

	if (this->List_Anim[0].Current_Anim_Time > this->List_Anim[0].End_Time)
	{
		this->List_Anim[0].Start_Animation();
	}


	for (unsigned int k = 1; k < this->List_Anim.size(); ++k)							//Syn these animations together so they are on the same track
	{
		this->List_Anim[k].Current_Anim_Time = this->List_Anim[k - 1].Current_Anim_Time*this->List_Anim[k].End_Time / this->List_Anim[k - 1].End_Time;
	}

	return JointPoses_To_JointTransforms(this->New_Pose());

}

void SyncAnim::Start_Animation()
{
	for (unsigned int k = 0; k < this->List_Anim.size(); ++k)
	{
		this->List_Anim[k].Start_Animation();
	}
}

SkeletonPose SyncAnim::New_Pose()
{
	int k = 1;
	while (this->Blend_Ratio >k)
	{
		k += 1;
	}

	return this->List_Anim[k - 1].Blend_Pose(this->Blend_Ratio-k+1 , this->List_Anim[k]);

}


void SyncAnim::Change_Blend_Ratio(float time)
{



	if (this->Blend_Ratio+time > this->List_Anim.size()-1)
	{
		this->Blend_Ratio = this->List_Anim.size() - 1;
	}
	else if(this->Blend_Ratio+time<0)
	{
		this->Blend_Ratio = 0.0;
	}
	else
	{
		int k = 1;
		while (this->Blend_Ratio > k)
		{
			k += 1;
		}
		this->Blend_Partition = k;

		if (k > 1)
		{
			this->Speed = 1 + time / (k - 1) * (this->Playback_Speed[k]);
		}
		

		this->Blend_Ratio += time;
	}
}
