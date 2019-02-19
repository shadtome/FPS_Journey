#include "LinearInterpolation.h"


JointPose Lerp(float &beta, JointPose joint1, JointPose joint2)
{
	Quarternion temp_quat;
	glm::vec3 temp_pos;

	//look at (1-beta) PoseJoint[k-1]+beta(PoseJoint[k]
	temp_quat = joint1.Rot_Quat*Pow(joint1.Rot_Quat.Invert()*joint2.Rot_Quat,beta);

	temp_pos = joint2.Pos_in_Parent*(beta)+joint1.Pos_in_Parent*(1 - beta);

	JointPose temp(temp_quat, temp_pos, *joint1.pJoint);

	return temp;
}

JointPose Slerp(float &beta, JointPose joint1, JointPose joint2)
{
	//Temporary quarternions/Pos
	Quarternion temp_quat;
	glm::vec3 temp_pos;
	float angle = acos(glm::dot(joint2.Rot_Quat.Vector, joint1.Rot_Quat.Vector));
	//look at (1-beta) PoseJoint[k-1]+beta(PoseJoint[k]
	temp_quat = joint2.Rot_Quat*(sin(glm::radians(angle)*beta) / sin(glm::radians(angle))) + joint1.Rot_Quat*(sin(glm::radians(angle)*(1 - beta)) / sin(glm::radians(angle)));

	temp_pos = joint2.Pos_in_Parent*(beta)+joint1.Pos_in_Parent*(1 - beta);

	JointPose temp(temp_quat, temp_pos, *joint1.pJoint);
	return temp;
}