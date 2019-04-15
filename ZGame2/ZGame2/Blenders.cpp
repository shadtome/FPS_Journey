#include "Blenders.h"

//---------------------------------------------------
//onedim_blend functions

//--------------------------------------------------
//Insert
void onedim_Blend::insert(Animator &anim, float pos)
{

	this->anims[pos] = anim;
	//Check if it over passes the last or first
	if (this->anims.empty())
	{
		first = pos;
		last = pos;
	}
	else
	{
		if (pos < first)
		{
			first = pos;
		}
		if (pos > last)
		{
			last = pos;
		}
	}

	this->blend_partfirst = first;
	this->blend_partlast = first;
}

//-------------------------------------------------
//Animate
SkeletonPose onedim_Blend::Animate(float &deltatime)
{
	
	 this->anims[first].Current_Anim_Time += this->speed*deltatime;


	//keep the animations looping and make sure they are synced correctly
	if (this->anims[first].Current_Anim_Time >= this->anims[first].End_Time)
	{
		this->anims[first].Start_Animation();
	}
	

	for (auto k = this->anims.begin(); k != this->anims.end(); ++k)	//Syn these animations together so they are on the same track
	{
		k->second.Current_Anim_Time = this->anims[first].Current_Anim_Time*k->second.End_Time / this->anims[first].End_Time;
		//std::cout << this->List_Anim[0].Current_Anim_Time << std::endl;
	}
	

	return this->New_Pose();
}

//---------------------------------------------------------
//start animations
void onedim_Blend::Start_Animation()
{
	for (auto k=this->anims.begin(); k!=this->anims.end(); ++k)
	{
		k->second.Start_Animation();
	}
}

//---------------------------------------------------------
//Change blend ratio
void onedim_Blend::Change_Blend_Ratio(float time)
{
	this->blend_ratio += time;

	if (this->blend_ratio > last)
	{
		//Set the Blend_Ratio as the last animation
		this->blend_ratio = last;
		this->blend_partfirst = last;
		this->blend_partlast = last;
	}
	else if (this->blend_ratio < first)
	{
		this->blend_ratio = first;
		this->blend_partfirst = first;
		this->blend_partlast = first;
	}
	else
	{

		//First, we need to find the subinterval blend_ratio is in
		float first_b=first;
		float last_b = last;
		for (auto k = this->anims.begin(); k != this->anims.end(); ++k)
		{
			if (first_b <= k->first <= this->blend_ratio)
			{
				first_b = k->first;
			}
			if (this->blend_ratio <= k->first <= last_b)
			{
				last_b = k->first;
			}
		}

		//This tells us that blend_ratio is the interval between first_b and the next one

		//Set the Blend_partiion for when we use the New_Pose Function
		this->blend_partfirst = first_b;
		this->blend_partlast = last_b;
		
		//Next, we need to figure out what the speed is for our animation
		//We have that the playback the kth animation is related to the k-1th animation by the equation
		// F_k=(List_Anim[k].EndTime)/(List_Anim[k-1].EndTime + beta(List_Anim[k].EndTime - List_Anim[k-1].EndTime)
		//This is comes from the interpolation between the playback speed for the kth animation and the k-1th animation
		// with the relationship F_{k-1}=(List_Anim[k-1].ENdTime/List_Anim[k].EndTime) F_{k}
		if (first < blend_partfirst < last)
		{
			float beta;	//Get the blend factor between 0 and 1
			beta = (this->blend_ratio - first_b);

			float Omega = (anims[last_b].End_Time) / (anims[first_b].End_Time + beta * (anims[last_b].End_Time - anims[first_b].End_Time));

			this->speed = Omega * (this->anims[first].End_Time / this->anims[last_b].End_Time);

		}

		
	}
}



//---------------------------------------------------------------
//new pose function
SkeletonPose onedim_Blend::New_Pose()
{
	
	//returned the blended Skeletonpose
	//the beta facter in the blending needs to be between 0 and 1.  So we take the blend_ratio-(k-1). to get it between 0 and 1
	if (this->anims.size() > 1)
	{
		return this->anims[this->blend_partfirst].Blend_Pose(this->blend_ratio - this->blend_partfirst, this->anims[this->blend_partlast]);
	}
	else
	{
		return anims[first].New_Pose();
	}
}