#ifndef LINEARINTERPOLATION_H
#define LINEARINTERPOLATION_H


#include "Skeleton.h"

//Lerp Function

JointPose Lerp(float &beta, JointPose joint1, JointPose joint2,Skeleton& skeleton);		//Lerp interpolation between two JointPoses

JointPose Slerp(float &beta, JointPose joint1, JointPose joint2,Skeleton& skeleton);		//Spherical Lerp interpolation between two Joint Poses

SkeletonPose Pose_Slerp(float &beta, SkeletonPose pose1, SkeletonPose pose2);



#endif

