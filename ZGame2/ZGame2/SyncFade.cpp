
#include "SyncFade.h"


//Make sure to document this code, since there is no information here

SyncAnim::SyncAnim()
{

}

void SyncAnim::Insert(Animation anim)
{
	this->List_Anim.push_back(Animator(*anim.pskeleton, anim));
	
	
}


std::vector<glm::mat4> SyncAnim::Animate(float &deltatime)
{
	
	this->List_Anim[0].Current_Anim_Time += this->Speed*deltatime;
	
	
	//keep the animations looping and make sure they are synced correctly
	if (this->List_Anim[0].Current_Anim_Time >= this->List_Anim[0].End_Time)
	{
		this->List_Anim[0].Start_Animation();
	}


	for (unsigned int k = 1; k < this->List_Anim.size(); ++k)	//Syn these animations together so they are on the same track
	{
		this->List_Anim[k].Current_Anim_Time = this->List_Anim[0].Current_Anim_Time*this->List_Anim[k].End_Time / this->List_Anim[0].End_Time;
		//std::cout << this->List_Anim[0].Current_Anim_Time << std::endl;
	}

	for (unsigned int i = 0; i < List_Anim.size(); ++i)
	{
		//std::cout << "ANIMATION"<<i<<List_Anim[i].Current_Anim_Time << std::endl;
		//std::cout << "ANIMATION TIME" << List_Anim[0].Current_Anim_Time << std::endl;
		//std::cout << "END TIME" << List_Anim[i].End_Time << std::endl;
	}
	
	return JointPoses_To_JointTransforms(this->New_Pose());

}


SkeletonPose SyncAnim::Animate_Pose(float &deltatime)
{
	this->List_Anim[0].Current_Anim_Time += this->Speed*deltatime;


	//keep the animations looping and make sure they are synced correctly
	if (this->List_Anim[0].Current_Anim_Time >= this->List_Anim[0].End_Time)
	{
		this->List_Anim[0].Start_Animation();
	}


	for (unsigned int k = 1; k < this->List_Anim.size(); ++k)	//Syn these animations together so they are on the same track
	{
		this->List_Anim[k].Current_Anim_Time = this->List_Anim[0].Current_Anim_Time*this->List_Anim[k].End_Time / this->List_Anim[0].End_Time;
		//std::cout << this->List_Anim[0].Current_Anim_Time << std::endl;
	}

	for (unsigned int i = 0; i < List_Anim.size(); ++i)
	{
		//std::cout << "ANIMATION"<<i<<List_Anim[i].Current_Anim_Time << std::endl;
		//std::cout << "ANIMATION TIME" << List_Anim[0].Current_Anim_Time << std::endl;
		//std::cout << "END TIME" << List_Anim[i].End_Time << std::endl;
	}

	return this->New_Pose();
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
	//returned the blended Skeletonpose
	//the beta facter in the blending needs to be between 0 and 1.  So we take the blend_ratio-(k-1). to get it between 0 and 1
	if (this->List_Anim.size() > 1)
	{
		return this->List_Anim[this->Blend_Partition - 1].Blend_Pose(this->Blend_Ratio - this->Blend_Partition + 1, this->List_Anim[this->Blend_Partition]);
	}
	else
	{
		return List_Anim[0].New_Pose();
	}
}


void SyncAnim::Change_Blend_Ratio(float time)
{
	
	this->Blend_Ratio += time;

	if (this->Blend_Ratio> this->List_Anim.size() - 1)
	{
		//Set the Blend_Ratio as the last animation
		this->Blend_Ratio = this->List_Anim.size()-1;
		this->Blend_Partition = this->List_Anim.size()-1;
	}
	else if(this->Blend_Ratio<0)
	{
		this->Blend_Ratio = 0.0;
		this->Blend_Partition = 1;
	}
	else
	{
		
		//First, we need to find the subinterval blend_ratio is in the interval [0,n-1)
		int k = 1;
		while (this->Blend_Ratio > k)
		{
			k += 1;
		}
		//This tells us that blend_ratio\in [k-1,k)

		//Set the Blend_partiion for when we use the New_Pose Function
		this->Blend_Partition = k;

		//Next, we need to figure out what the speed is for our animation
		//We have that the playback the kth animation is related to the k-1th animation by the equation
		// F_k=(List_Anim[k].EndTime)/(List_Anim[k-1].EndTime + beta(List_Anim[k].EndTime - List_Anim[k-1].EndTime)
		//This is comes from the interpolation between the playback speed for the kth animation and the k-1th animation
		// with the relationship F_{k-1}=(List_Anim[k-1].ENdTime/List_Anim[k].EndTime) F_{k}
		if (0<k<this->List_Anim.size())
		{
			float beta;	//Get the blend factor between 0 and 1
			if (k > 1)
			{
				beta = this->Blend_Ratio - k + 1;
			}
			else
			{
				beta = this->Blend_Ratio;
			}

			float Omega= (List_Anim[k].End_Time) / (List_Anim[k - 1].End_Time + beta * (List_Anim[k].End_Time - List_Anim[k - 1].End_Time));

			this->Speed = Omega * (this->List_Anim[0].End_Time / this->List_Anim[k].End_Time);
			
		}	
		
		
	}
}
