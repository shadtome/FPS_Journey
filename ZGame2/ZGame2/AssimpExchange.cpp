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
		glm::mat4 rot(glm::vec4(1.0, 0.0, 0.0, 0.0), glm::vec4(0.0, 0.0, -1.0, 0.0), glm::vec4(0.0, 1.0, 0.0, 0.0), glm::vec4(0.0, 0.0, 0.0, 1.0));
		//glm::mat4 rot(glm::vec4(0.0, 0.0, 1.0, 0.0), glm::vec4(1.0, 0.0, 0.0, 0.0), glm::vec4(0.0, 1.0, 0.0, 0.0), glm::vec4(0.0, 0.0, 0.0, 1.0));
		result = rot * result;
	}
	
	
	//result=glm::transpose(result);

	return result;
}





 Quarternion Assimp_QuatConv(aiQuaternion &quat)
{
	Quarternion result;

	result.scalar = quat.w;
	result.Vector = glm::vec3(quat.x, quat.z, -quat.y);
	result.Norm();

	return result;
}