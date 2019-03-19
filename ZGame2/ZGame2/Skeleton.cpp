
#include <cstring>
#include <algorithm>
#include "Skeleton.h"

//---------------------------------------------------------------------------------------------------------------
//Skeleton Methods

Skeleton::Skeleton(std::vector<Joint> &joints)
{
	this->Vector_Joints = joints;
	for (unsigned int k = 0; k < joints.size(); ++k)
	{
		this->Vector_Joints[k].ID = k;
	}
	
	this->JointCount = joints.size();
	this->Vector_Joints[0].Parent_Index = -1;		//by construction from AsSIMP, the first joint is always the root index, by construction
	
}

int Skeleton::Search(const char* name)
{
	for (unsigned int k = 0; k < this->JointCount; ++k)
	{
		if (strcmp(name,this->Vector_Joints[k].Name)==0)
		{
			return k;
		}
	}

	//Otherwise, this means that this bone is not in the main bones (biped system) so hence not important for animation
	return -1;
	
}


//-----------------------------------------------------------------------------
//JointPose Methods

JointPose::JointPose(Quarternion &rot_quat, glm::vec3 &pos_in_parent,glm::vec3 &scale, Joint &joint,Skeleton &skeleton)
{
	this->Rot_Quat = rot_quat;
	this->Pos_in_Parent = pos_in_parent;
	this->scale = scale;
	this->pJoint = &skeleton.Vector_Joints[joint.ID];
	
}

JointPose::JointPose(Quarternion &rot_quat, glm::vec3 &pos_in_parent, Joint &joint,Skeleton &skeleton)
{
	this->Rot_Quat = rot_quat;
	this->Pos_in_Parent = pos_in_parent;
	this->ID = joint.ID;
	this->scale = glm::vec3(1.0, 1.0, 1.0);
	this->pJoint = &skeleton.Vector_Joints[joint.ID];
	
}

JointPose::JointPose(glm::mat4 local_transform, Joint &joint)
{
	this->Total_transform = local_transform;
	this->pJoint = &joint;

}

void JointPose::Compile_Transform()
{
	glm::mat4 temp;
	// we are doing it in this order becuase of the way glm does the translattion/scale operations.
	//It does it by multiplying the scale matrix or translate matrix on the right.
	//hence the bottom matrix is of the form temp=temp*T*R*S
	temp = glm::translate(temp, this->Pos_in_Parent); 
	temp = temp* glm::mat4(this->Rot_Quat.Matrix_Rep()); 
	temp = glm::scale(temp, this->scale);

	this->Total_transform = temp;
}


//-------------------------------------------------------------------------------
// SkeletonPose Methods

SkeletonPose::SkeletonPose()
{

}

SkeletonPose::SkeletonPose(Skeleton &skeleton, std::vector<JointPose> jointposes)
{
	//Sort the vector using the sort algorithmn in the STD library O(nlogn)
	std::sort(jointposes.begin(), jointposes.end());
	
	//Tell the jointposes, where their parents are in the vector
	//This vector is now sorted with respect to the same ordering as the joints in the skeleton
	for (unsigned int k = 0; k < jointposes.size(); ++k)
	{
		jointposes[k].ID = k;
		jointposes[k].Parent_Index = jointposes[k].pJoint->Parent_Index;
	}
	

	//Input the data
	this->pSkeleton = &skeleton;
	this->Poses_Joints = jointposes;
}





void SkeletonPose::Setup_Pose()
{

	//Allocate enough memory for the number of joints
	this->Global_Poses.reserve(this->Poses_Joints.size());

	//Compilre matrix transforms for the joints.
	for (unsigned int k = 0; k < this->Poses_Joints.size(); ++k)
	{
		this->Poses_Joints[k].Compile_Transform();
	}

	//go through each joint and multiply all the corresponding matrices from its parent joint to the root
	for (int k = 0; k < this->Poses_Joints.size(); ++k)
	{
		glm::mat4 new_matrix;

		//multiplies the matrices from joint coordinates to its parent coordinates till we get to the root joint
		Joint_to_Root_Transform(this->Poses_Joints, this->Poses_Joints[k], new_matrix);
		
		//This is the skinning matrix to connect the vertifces with the bones in the shader
		new_matrix = glm::inverse(this->pSkeleton->Root_Transform)* new_matrix* this->Poses_Joints[k].pJoint->M_invBindPose;	
		this->Global_Poses.push_back(new_matrix);
	}
}

void SkeletonPose::Setup_Pose_Local()
{
	//go through each joint and multiply all the corresponding matrices that from its parent joint to the root
	for (int k = 0; k < this->Poses_Joints.size(); ++k)
	{
		glm::mat4 new_matrix;

		Joint_to_Root_Transform(this->Poses_Joints, this->Poses_Joints[k], new_matrix);
		new_matrix = new_matrix * this->Poses_Joints[k].pJoint->M_invBindPose;

		this->Global_Poses.push_back(new_matrix);
	}
}


//Forward Declared function from above
//----------------------------------
// Iterative method to multiply the matrices of the corresponding joints with their parents
//Takes in the entire poses for a skeleton, and a specific pose from the smae list and return the Global Matrix to model space
void Joint_to_Root_Transform(std::vector<JointPose> &Poses, JointPose &p, glm::mat4 &result)
{


	if (p.Parent_Index == -1)
	{
		return;					// If Null, then this is the ROOT, and hence exit out
	}

	Joint_to_Root_Transform(Poses, Poses[p.Parent_Index], result);		// Go to its parent joint and do the same thing

	result = result * p.Total_transform;

}

void Local_to_Root_Transform(std::vector<glm::mat4> &local, Skeleton &skeleton, Joint &joint,glm::mat4 &result)
{
	if (joint.Parent_Index == -1)
	{
		return;
	}

	Local_to_Root_Transform(local, skeleton, skeleton.Vector_Joints[joint.Parent_Index],result);

	result = result * local[joint.ID];
}