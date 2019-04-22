#include "LinearInterpolation.h"


JointPose Lerp(float &beta, JointPose joint1, JointPose joint2,Skeleton& skeleton)
{
	Quaternion temp_quat;
	glm::vec3 temp_pos;

	//look at (1-beta) PoseJoint[k-1]+beta(PoseJoint[k]
	temp_quat = joint1.Rot_Quat*Pow(joint1.Rot_Quat.Invert()*joint2.Rot_Quat,beta);

	temp_pos = joint2.Pos_in_Parent*(beta)+joint1.Pos_in_Parent*(1 - beta);

	JointPose temp(temp_quat, temp_pos, *joint1.pJoint,skeleton);
	

	return temp;
}

JointPose Slerp(float &beta, JointPose joint1, JointPose joint2,Skeleton& skeleton)
{
	
	if (beta != 0)
	{
		
		//Temporary quarternions/Pos
		Quaternion temp_quat;
		glm::vec3 temp_pos;
		float dot = glm::dot(joint2.Rot_Quat.Vector, joint1.Rot_Quat.Vector) / (joint2.Rot_Quat.Norm()*joint1.Rot_Quat.Norm());

		
		float angle =glm::radians(SafeAcos(dot));
		
		//Take the limit angle->0+ to get this formula/ or the limit angle->pi- 
		if (angle==0 || angle==PI_f)
		{
			temp_quat = (joint2.Rot_Quat *(cos(angle*beta)* beta/cos(angle)) + joint1.Rot_Quat *(cos(angle*(1-beta))* (1 - beta)/cos(angle)));
		}
		else
		{
			
			//look at (1-beta) PoseJoint[k-1]+beta(PoseJoint[k]
			temp_quat = joint2.Rot_Quat*(sin(angle*beta) / sin(angle)) + joint1.Rot_Quat*(sin(angle*(1 - beta)) / sin(angle));
		}
		



		temp_pos = joint2.Pos_in_Parent*(beta)+joint1.Pos_in_Parent*(1 - beta);

		JointPose temp(temp_quat, temp_pos, *joint1.pJoint, skeleton);
		return temp;
	}
	else
	{
		return joint1;
	}
	
}



SkeletonPose Pose_Slerp(float &beta, SkeletonPose pose1, SkeletonPose pose2)
{
	std::vector<JointPose> joints;
	for (unsigned int k = 0; k < pose1.pSkeleton->JointCount; ++k)
	{
		joints.push_back(Slerp(beta, pose1.Poses_Joints[k], pose2.Poses_Joints[k], *pose1.pSkeleton));
	}

	return SkeletonPose(*pose1.pSkeleton, joints);
}