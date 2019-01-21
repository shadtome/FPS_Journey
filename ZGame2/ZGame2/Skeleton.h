#ifndef SKELETON_H
#define SKELETON_H

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Quaternion.h"

//Forward Deceleration for classes
class SkeletonPose;
class JointPose;



// the joints to a skeleton
struct Joint
{
public:
	glm::mat4 M_invBindPose;  // Inverse bind pose transform

	const char* Name;  // Name of the joint

	unsigned int ID;    // just the name of the joint, might just use a ID

	int Parent_Index;  // if any, it might be the root of the skeleton so make it null

};


// the skeleton that holds all of its joints 

class Skeleton
{
public:
	unsigned int JointCount; // Number of Joints

	std::vector<Joint>	Vector_Joints;  //Vector of joints

	//Constructor
	Skeleton(std::vector<Joint> joints);

};

// pose of the joints
class JointPose
{
public:
	Quarternion Rot_Quat; // Rotation Unit Quaternion

	glm::vec3 Pos_in_Parent; //Position of this joint in its parent's coordinate system

	float scale; //uniform scale for the joint pose

	Joint* pJoint;  // Point to the Joint , delete later

	unsigned int ID;  // Unique Identifier 

	glm::mat4 Total_transform;  // The transofmration pose for this joint with respect to its parent

	int Parent_Index;  // The index of its parent

	//Constructor
	JointPose(Quarternion rot_quat, glm::vec3 pos_in_parent, Joint &joint);
	
	//Methods
	// This is used to put together the Quarternion rotation and the pos_in_parent to make a transform for this joint
	void Compile_Transform();
};


//Skeleton pose that is constructed through all the joints
class SkeletonPose
{
public:
	Skeleton* pSkeleton; //skeleton that this will point and associated with

	//Make sure that the joints are always next to their parents, or as close enough, or we can use vector, with a linked list
	std::vector<JointPose> Poses_Joints; // the posed for each of the joints,  Make sure that this is dynamically allocated enough for the number of joints

	std::vector<glm::mat4> Global_Poses; // Poses from the children to the root (we can have different branches, like a human has different braches of of the skeleton)

	//Constructor
	SkeletonPose(Skeleton &skeleton, std::vector<JointPose> jointposes);

	//Methods
	void Setup_Pose();
};



//---------------------------------------------------------------------------------------------------------------
//Skeleton Methods

Skeleton::Skeleton(std::vector<Joint> joints)
{
	this->Vector_Joints = joints;
	this->JointCount = joints.size();
	for (int k = 0; k < this->JointCount; ++k)
	{
		this->Vector_Joints[k].ID = k;
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

void JointPose::Compile_Transform()
{
	
	this->Total_transform = glm::translate(this->Total_transform, this->Pos_in_Parent); //transform to the position of the joint in parent space
	this->Total_transform = glm::mat4(this->Rot_Quat.Matrix_Rep())*this->Total_transform; //multipliy first by the translation to move to its parent joint coordinates, then rotate it that space
	
}


//-------------------------------------------------------------------------------
// SkeletonPose Methods

SkeletonPose::SkeletonPose(Skeleton &skeleton, std::vector<JointPose> jointposes)
{
	this->pSkeleton = &skeleton;
	this->Poses_Joints = jointposes;
}

//Forward Deceleration for function which is down below
void Joint_to_Root_Transform(std::vector<JointPose> &Poses, JointPose &p, glm::mat4 &result);

void SkeletonPose::Setup_Pose()
{
	
	//Allocate enough memory for the number of joints
	this->Global_Poses.reserve(this->Poses_Joints.size());

	//go through each joint and multiply all the corresponding matrices that from its parent joint to the root
	for (int k = 0; k < this->Poses_Joints.size(); ++k)
	{
		glm::mat4 new_matrix;

		Joint_to_Root_Transform(this->Poses_Joints, this->Poses_Joints[k],new_matrix);

		this->Global_Poses.push_back(new_matrix);
	}
}


//Forward Declared function from above
//----------------------------------
// Iterative method to multiply the matrices of the corresponding joints with their parents
//Takes in the entire poses for a skeleton, and a specific pose from the smae list and return the Global Matrix to model space
void Joint_to_Root_Transform(std::vector<JointPose> &Poses, JointPose &p,glm::mat4 &result)
{
	if (p.Parent_Index == -1)
	{
		return;					// If Null, then this is the ROOT, and hence exit out
	}

	Joint_to_Root_Transform(Poses, Poses[p.Parent_Index],result);		// Go to its parent joint and do the same thing

	result = result * p.Total_transform;

}


#endif 