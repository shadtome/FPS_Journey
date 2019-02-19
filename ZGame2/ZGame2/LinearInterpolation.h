#ifndef LINEARINTERPOLATION_H
#define LINEARINTERPOLATION_H


#include "Skeleton.h"

//Lerp Function

JointPose Lerp(float &beta, JointPose joint1, JointPose joint2);		//Lerp interpolation between two JointPoses

JointPose Slerp(float &beta, JointPose joint1, JointPose joint2);		//Spherical Lerp interpolation between two Joint Poses





#endif

