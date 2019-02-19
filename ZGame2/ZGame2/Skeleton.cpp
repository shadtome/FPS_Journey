
#include "Skeleton.h"

//---------------------------------------------------------------------------------------------------------------
//Skeleton Methods

Skeleton::Skeleton(std::vector<Joint> &joints)
{
	for (unsigned int k = 0; k < joints.size(); ++k)
	{
		joints[k].ID = k;
	}
	
	this->Vector_Joints = joints;
	this->JointCount = joints.size();
	this->Vector_Joints[0].Parent_Index = -1;		//by construction from AsSIMP, the first joint is always the root index, by construction
	
}

unsigned int Skeleton::Search(const char* name)
{
	for (unsigned int k = 0; k < this->JointCount; ++k)
	{
		if (name = this->Vector_Joints[k].Name)
		{
			return k;
		}
	}
}


//-----------------------------------------------------------------------------
//JointPose Methods

JointPose::JointPose(Quarternion rot_quat, glm::vec3 pos_in_parent, Joint &joint)
{
	this->Rot_Quat = rot_quat;
	this->Pos_in_Parent = pos_in_parent;
	this->pJoint = &joint;
	this->Parent_Index = joint.Parent_Index;
}

JointPose::JointPose(glm::mat4 local_transform, Joint &joint)
{
	this->Total_transform = local_transform;
	this->pJoint = &joint;
	this->Parent_Index = joint.Parent_Index;
}

void JointPose::Compile_Transform()
{
	
	this->Total_transform = glm::translate(this->Total_transform, this->Pos_in_Parent); //transform to the position of the joint in parent space
	this->Total_transform = glm::mat4(this->Rot_Quat.Matrix_Rep())*this->Total_transform; //multipliy first by the translation to move to its parent joint coordinates, then rotate it that space
	this->Total_transform = this->Total_transform*this->pJoint->M_invBindPose;				//This constructs the skinning matrix(C*B^{-1}, where C is the current pose matrix, and B is the bind pose matrix(maps from joint coordinates to model coordinates)
}


//-------------------------------------------------------------------------------
// SkeletonPose Methods

SkeletonPose::SkeletonPose()
{

}

SkeletonPose::SkeletonPose(Skeleton &skeleton, std::vector<JointPose> jointposes)
{
	this->pSkeleton = &skeleton;
	this->Poses_Joints = jointposes;
}





void SkeletonPose::Setup_Pose()
{

	//Allocate enough memory for the number of joints
	this->Global_Poses.reserve(this->Poses_Joints.size());

	for (unsigned int k = 0; k < this->Poses_Joints.size(); ++k)
	{
		this->Poses_Joints[k].Compile_Transform();
	}

	//go through each joint and multiply all the corresponding matrices that from its parent joint to the root
	for (int k = 0; k < this->Poses_Joints.size(); ++k)
	{
		glm::mat4 new_matrix;

		Joint_to_Root_Transform(this->Poses_Joints, this->Poses_Joints[k], new_matrix);

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