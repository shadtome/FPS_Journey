#include "AssimpExchange.h"



 glm::mat4 Assimp_MatrixConv(aiMatrix4x4 &matrix, std::string &file_type)
{
	 //glm is column-major and assimp is row-major
	glm::mat4 result;
	//First column
	result[0][0] = matrix.a1;
	result[0][1] = matrix.b1;
	result[0][2] = matrix.c1;
	result[0][3] = matrix.d1;
	//second column
	result[1][0] = matrix.a2;
	result[1][1] = matrix.b2;
	result[1][2] = matrix.c2;
	result[1][3] = matrix.d2;
	//third column
	result[2][0] = matrix.a3;
	result[2][1] = matrix.b3;
	result[2][2] = matrix.c3;
	result[2][3] = matrix.d3;
	//fourth column
	result[3][0] = matrix.a4;
	result[3][1] = matrix.b4;
	result[3][2] = matrix.c4;
	result[3][3] = matrix.d4;

	//Now we need to transform it to our coordinate system
	if (file_type == ".dae" || file_type == ".fbx")
	{
		//result = glm::transpose(result);
		glm::mat4 rot(glm::vec4(1.0, 0.0, 0.0, 0.0), glm::vec4(0.0, 0.0, -1.0, 0.0), glm::vec4(0.0, 1.0, 0.0, 0.0), glm::vec4(0.0, 0.0, 0.0, 1.0));
		//glm::mat4 rot(glm::vec4(0.0, 0.0, 1.0, 0.0), glm::vec4(1.0, 0.0, 0.0, 0.0), glm::vec4(0.0, 1.0, 0.0, 0.0), glm::vec4(0.0, 0.0, 0.0, 1.0));
		result = rot * result*glm::inverse(rot);
		
	}
	
	
	//result=glm::transpose(result);

	return result;
}


 glm::vec3 Assimp_Vec3Conv(aiVector3D &vec, std::string &file_type)
 {
	 glm::vec3 result;
	 if (file_type == ".dae" || file_type == ".fbx")
	 {
		 result.x = vec.x;
		 result.y = vec.z;
		 result.z = -vec.y;
	 }
	 else
	 {
		 result.x = vec.x;
		 result.y = vec.y;
		 result.z = vec.z;
	 }
	 return result;
 }


 Quaternion Assimp_QuatConv(aiQuaternion &quat)
{
	Quaternion result;

	result.scalar = -quat.w;								//This is not right, check in the Quaternions for a explnation.  maybe the inverse function or conjuage messing it up
	result.Vector = glm::vec3(quat.x, quat.z, -quat.y);		//The coordinate system from assimp is z-up, y-forward, and x-right.
															//while opengl coordinate system is y-up, z-back, x-right
	

	return result;
}