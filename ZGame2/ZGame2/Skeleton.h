#ifndef SKELETON_H
#define SKELETON_H

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include "Quaternion.h"

//Forward Deceleration for classes
class SkeletonPose;
class JointPose;

//Forward Deceleration of function
void Joint_to_Root_Transform(std::vector<JointPose> &Poses, JointPose &p, glm::mat4 &result);

// the joints to a skeleton
struct Joint
{
public:
	glm::mat4 M_invBindPose;  // Inverse bind pose transform

	std::string Name;  // Name of the joint

	unsigned int ID;    // just the name of the joint, might just use a ID

	int Parent_Index;  // if any, it might be the root of the skeleton so make it null

	Joint* parent;		//Point to parent

};


// the skeleton that holds all of its joints 
//The question is, should I use a simple tree structure for this?
//Once I get all the data in, it should be constant time in access the joints of this and their parents if I let them hold the index of its parent.
class Skeleton
{
public:
	unsigned int JointCount; // Number of Joints

	//Root Transform
	glm::mat4 Root_Transform;

	std::vector<Joint>	Vector_Joints;  //Vector of joints

	//Constructors
	Skeleton() {};
	Skeleton(std::vector<Joint> &joints);

	//Functions

	/*Skeleton search function
	* Search through the joints for the joint that has the specified name
	* and out puts the index in the vector for this joint
	*/
	int Search(std::string name);

};

// pose of the joints
class JointPose
{
public:
	Quaternion Rot_Quat; // Rotation Unit Quaternion

	glm::vec3 Pos_in_Parent; //Position of this joint in its parent's coordinate system

	glm::vec3 scale; // scale for the joint pose

	Joint* pJoint;  // Point to the Joint , delete later

	unsigned int ID;  // Unique Identifier 

	glm::mat4 Total_transform;  // The transofmration pose for this joint with respect to its parent

	int Parent_Index;  // The index of its parent

	//Constructor
	JointPose(glm::mat4 local_transform, Joint &joint);
	JointPose(Quaternion &rot_quat, glm::vec3 &pos_in_parent,glm::vec3 &scale, Joint &joint,Skeleton &skeleton);
	JointPose(Quaternion &rot_quat, glm::vec3 &pos_in_parent, Joint &joint,Skeleton &skeleton);

	
	//Methods
	// This is used to put together the Quarternion rotation and the pos_in_parent to make a transform for this joint
	void Compile_Transform();

	//Overloaded comparison function
	bool operator<(const JointPose& other)const
	{
		return pJoint->ID < other.pJoint->ID;
	}
};

//Compare function for JointPose
//This is used for when sorting a vector of JointPoses, to be just like the vector of Joints in a skeleton




//Skeleton pose that is constructed through all the joints
class SkeletonPose
{
public:
	Skeleton* pSkeleton; //skeleton that this will point and associated with

	//Make sure that the joints are always next to their parents, or as close enough, or we can use vector, with a linked list
	std::vector<JointPose> Poses_Joints; // the posed for each of the joints,  Make sure that this is dynamically allocated enough for the number of joints

	std::vector<glm::mat4> Global_Poses; // Poses from the children to the root (we can have different branches, like a human has different braches of of the skeleton)

	//Constructor
	SkeletonPose();
	SkeletonPose(Skeleton &skeleton, std::vector<JointPose> jointposes);
	SkeletonPose(Skeleton &skeleton, std::vector<glm::mat4> local_transforms);

	//Methods
	void Setup_Pose();
	void Setup_Pose_Local();
};





#endif 