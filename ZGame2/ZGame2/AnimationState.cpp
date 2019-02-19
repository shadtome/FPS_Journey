
/*
#include "AnimationState.h"
#include <iostream>

void AnimationTree::Insert_End(Animator animator)
{
	this->Ordered_List.push_back(animator);
}


//-----------------------------------
//Delete and INsert functions here

//-----------------------------------


unsigned int AnimationTree::Search(const char* name)
{
	for (unsigned int k = 0; k < this->Ordered_List.size(); ++k)
	{
		if (this->Ordered_List[k].Cur_Anim.Name == name)
		{
			return k;
		}
	}
}

void AnimationTree::Change_State(float deltatime, const char* name)
{
	New_State = this->Search(name);
	if (New_State > this->State)
	{
		this->Ordered_List[this->State].Start_Blending(deltatime, this->Ordered_List[this->State+1]);
	}
	if (New_State < this->State)
	{
		this->Ordered_List[this->State].Start_Blending(deltatime, this->Ordered_List[this->State-1]);
	}
	//Do nothing if New State and State are the Same
	
}


std::vector<glm::mat4> AnimationTree::Animate_State(float deltatime)
{
	if (this->State == this->New_State)
	{
		if (this->Ordered_List[this->State].Animating == false)
		{
			this->Ordered_List[this->State].Start_Animation();
		}

		return this->Ordered_List[this->State].Animate(deltatime);
	}
	else if (this->State < this->New_State)
	{

		if (this->Ordered_List[this->State].Blending==false)						//Meaning we ended the blending effect for this, now proceed with the next animation
		{
			this->State += 1;
			if (this->State < this->New_State)
			{
				this->Ordered_List[this->State].Start_Blending(deltatime, this->Ordered_List[this->State + 1]);
				return this->Ordered_List[this->State].Blend_Animate(deltatime, this->Ordered_List[this->State + 1]);
			}
			else
				return this->Ordered_List[this->State].Animate(deltatime);
		}
		else
			return this->Ordered_List[this->State].Blend_Animate(deltatime, this->Ordered_List[this->State + 1]);
	}
	else
	{
		if (this->Ordered_List[this->State].Blending == false)						//Meaning we ended the blending effect for this, now proceed with the next animation
		{
			this->State -= 1;
			if (this->State > this->New_State)
			{
				this->Ordered_List[this->State].Start_Blending(deltatime, this->Ordered_List[this->State - 1]);
				return this->Ordered_List[this->State].Blend_Animate(deltatime, this->Ordered_List[this->State - 1]);
			}
			else
				return this->Ordered_List[this->State].Animate(deltatime);
		}
		else
			return this->Ordered_List[this->State].Blend_Animate(deltatime, this->Ordered_List[this->State - 1]);
	}
}
*/
