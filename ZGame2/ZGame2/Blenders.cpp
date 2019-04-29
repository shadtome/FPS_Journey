#include "Blenders.h"
#include "LinearInterpolation.h"

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
void onedim_Blend::Change_Blend_Ratio(float delta)
{
	this->blend_ratio += delta;

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
			if (first_b <= k->first && k->first <= this->blend_ratio)
			{
				first_b = k->first;
			}
			if (this->blend_ratio < k->first && k->first <= last_b)
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


void onedim_Blend::Set_Blend_Ratio(float delta)
{
	if (delta > last)
	{
		//Set the Blend_Ratio as the last animation
		this->blend_ratio = last;
		this->blend_partfirst = last;
		this->blend_partlast = last;
	}
	else if (delta < first)
	{
		this->blend_ratio = first;
		this->blend_partfirst = first;
		this->blend_partlast = first;
	}
	else
	{
		this->blend_ratio = delta;
		//First, we need to find the subinterval blend_ratio is in
		float first_b = first;
		float last_b = last;
		for (auto k = this->anims.begin(); k != this->anims.end(); ++k)
		{
			if (first_b <= k->first && k->first <= this->blend_ratio)
			{
				first_b = k->first;
			}
			if (this->blend_ratio < k->first && k->first <= last_b)
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

float onedim_Blend::Cur_Blend_Ratio()
{
	return this->blend_ratio-blend_partfirst;
}



//---------------------------------------------------------------
//new pose function
SkeletonPose onedim_Blend::New_Pose()
{
	
	//returned the blended Skeletonpose
	//the beta facter in the blending needs to be between 0 and 1.  So we take the blend_ratio-blend_part first. to get it between 0 and 1
	if (this->anims.size() > 1)
	{
		
		return this->anims[this->blend_partfirst].Blend_Pose(this->blend_ratio - this->blend_partfirst, this->anims[this->blend_partlast]);
	}
	else
	{
		return anims[first].New_Pose();
	}
}


//--------------------------------------------------------------------
//Square Two Dimensional Blending.

void Square_twodim_Blend::Top_insert(Animator &anim, float pos)
{
	Top_b.insert(anim, pos);
}

void Square_twodim_Blend::Bot_insert(Animator &anim, float pos)
{
	Bot_b.insert(anim, pos);
}

SkeletonPose Square_twodim_Blend::Animate(float &deltatime)
{
	this->Cur_time = deltatime / this->speed;
	
	

	return this->New_Pose(Top_b.Animate(Cur_time), Bot_b.Animate(Cur_time));
}

void Square_twodim_Blend::Start_Animation()
{
	Top_b.Start_Animation();
	Bot_b.Start_Animation();
}


void Square_twodim_Blend::Change_Blend_Ratio(float delta_x,float delta_y)
{
	//Change the blend ratio in the x direction
	Top_b.Change_Blend_Ratio(delta_x);
	Bot_b.Change_Blend_Ratio(delta_x);

	//Lets change the blend Ratio in the y_direction

	this->blend_y += delta_y;

	if (this->blend_y > 1)
	{
		this->blend_y = 1;
	}
	if (this->blend_y < 0)
	{
		this->blend_y = 0;
	}

	//Next, we need to change the speed
	this->speed = this->blend_y * Bot_b.Cur_Blend_Ratio() + (1 - this->blend_y)*Top_b.Cur_Blend_Ratio();
}

void Square_twodim_Blend::Set_Blend_Ratio(float delta_x, float delta_y)
{
	this->Bot_b.Set_Blend_Ratio(delta_x);
	this->Top_b.Set_Blend_Ratio(delta_x);
	this->blend_y = delta_y;
}


SkeletonPose Square_twodim_Blend::New_Pose(SkeletonPose Top, SkeletonPose Bot)
{
	return Pose_Slerp(this->blend_y,Top, Bot);
}




//----------------------------------------------
//Difference Animator

//Functions

//Constructors!
Difference_Animator::Difference_Animator(Animator &source, Animator &reference)
{
	this->source = source;
	this->reference = reference;

	this->reference.End_Time = source.End_Time / reference.End_Time;
}


//Add difference pose to another.
SkeletonPose Difference_Animator::Add_Pose(SkeletonPose &target,float &deltatime)
{
	//First set up the pose for S-R
	SkeletonPose RS = Setup_Pose(deltatime);

	//First we need to multiply D * T (matrix wise) 

	std::vector<JointPose> temp_jp;
	
	for (unsigned int k = 0; k <RS.pSkeleton->JointCount; ++k)
	{
		Quaternion temp_q;
		glm::vec3 temp_p; 

		temp_q = target.Poses_Joints[k].Rot_Quat*RS.Poses_Joints[k].Rot_Quat;

		temp_p = RS.Poses_Joints[k].Pos_in_Parent + target.Poses_Joints[k].Pos_in_Parent;

		temp_jp.push_back(JointPose(temp_q, temp_p, *RS.Poses_Joints[k].pJoint, *RS.pSkeleton));
	}

	SkeletonPose DT(*RS.pSkeleton, temp_jp);

	return Pose_Slerp(beta, target, DT);
}



SkeletonPose Difference_Animator::Setup_Pose(float &deltatime)
{
	std::vector<JointPose> temp_jp;

	SkeletonPose source_pose = source.Animate(deltatime);
	SkeletonPose reference_pose = reference.Animate(deltatime);

	for (unsigned int k = 0; k < source.pskeleton->JointCount; ++k)
	{
		Quaternion temp_q;	//temp quaternion
		glm::vec3 temp_p;	//temp position

		temp_q = reference_pose.Poses_Joints[k].Rot_Quat.inverse()*source_pose.Poses_Joints[k].Rot_Quat;

		temp_p = -reference_pose.Poses_Joints[k].Pos_in_Parent + source_pose.Poses_Joints[k].Pos_in_Parent;

		temp_jp.push_back(JointPose(temp_q, temp_p, *source_pose.Poses_Joints[k].pJoint, *source_pose.pSkeleton));
	}

	return SkeletonPose(*source_pose.pSkeleton, temp_jp);
}


void Difference_Animator::start(float Beta)
{
	this->source.Start_Animation();
	this->reference.Start_Animation();

	this->beta = Beta;
}

void Difference_Animator::stop()
{
	this->source.End_Animation();
	this->reference.End_Animation();
}

void Difference_Animator::Change_blend_factor(float &deltabeta)
{
	this->beta += deltabeta;
}

void Difference_Animator::Set_blend_factor(float &Beta)
{
	this->beta = Beta;
}