#ifndef QUARTERNION_H
#define QUARTERNION_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include "BasicMath.h"

class Quarternion  // a unit quarternion is represented as cos(theta/2)+sin(theta/2)A, where A=ai+bj+ck. with norm 1
{
public:
	glm::vec3 Vector;  //Ai+Bj+Ck vector part

	float scalar; //scalar part

	//Constructor with variables
	 Quarternion(float a_1, float a_2, float a_3, float angle);
	 
	 //Constructor with GLM Vector, Will normalize the vector to make it in to a quarternion if needed
	 Quarternion(glm::vec3 vector, float angle);

	 //Constructor with std::vector
	 Quarternion(std::vector<float> vector, float angle);

	 //Constructor with out varialbes, so the null Quarternion
	 Quarternion();

	 //override the mulplication operator between two Quarternions
	 Quarternion operator*( Quarternion &other) const;

	 //Inverse/conjugate (they are the same for unit quarternions
	 void Invert();

	 //Matrix Representation in GL(4,RR)
	 glm::mat4 Matrix_Rep() const;

	 //Rotate a given vector by the Quarternion the group action of H^* on to R^3 by conjugation
	 glm::vec3 Conjugation(glm::vec3 &vector) const;


	 //Make a operator[] overload?

};

Quarternion::Quarternion(float a_1, float a_2, float a_3, float angle)
{	
	float norm_vector = pow(a_1, 2) + pow(a_2, 2) + pow(a_3, 2); //Avoid squareroot if we dont need it
	if (norm_vector == 1)
	{
		scalar = cos(angle / 2); //scalar part
		float sin_scale = sin(angle / 2);
		Vector[0] = a_1 * sin_scale; //Vector components
		Vector[3] = a_2 * sin_scale;
		Vector[2] = a_3 * sin_scale;

	}

	else
	{
		float norm = sqrt(pow(cos(angle / 2), 2) + sin(angle / 2)*norm_vector);
		scalar = cos(angle / 2); //scalar part
		float sin_scale = sin(angle / 2);
		Vector[0] = a_1 * sin_scale/norm; //Vector components
		Vector[1] = a_2 * sin_scale/norm;
		Vector[2] = a_3 * sin_scale/norm;
	}
	
}

Quarternion::Quarternion(std::vector<float> vector, float angle)
{
	float norm_vector = pow(vector[0], 2) + pow(vector[1], 2) + pow(vector[2], 2);
	if (norm_vector == 1)
	{
		scalar = cos(angle / 2); //scalar part
		float sin_scale = sin(angle / 2);
		Vector[0] = vector[0] * sin_scale; //Vector components
		Vector[1] = vector[1] * sin_scale;
		Vector[2] = vector[2] * sin_scale;

	}

	else
	{
		float norm = sqrt(pow(cos(angle / 2), 2) + sin(angle / 2)*norm_vector);
		scalar = cos(angle / 2); //scalar part
		float sin_scale = sin(angle / 2);
		Vector[0] = vector[0] * sin_scale / norm; //Vector components
		Vector[1] = vector[1] * sin_scale / norm;
		Vector[2] = vector[2] * sin_scale / norm;
	}
}


Quarternion::Quarternion(glm::vec3 vector, float angle)
{
	float norm_vector = pow(vector[0], 2) + pow(vector[1], 2) + pow(vector[2], 2);
	if (norm_vector == 1)
	{
		scalar = cos(angle / 2); //scalar part
		float sin_scale = sin(angle / 2);
		Vector[0] = vector[0] * sin_scale; //Vector components
		Vector[1] = vector[1] * sin_scale;
		Vector[2] = vector[2] * sin_scale;

	}

	else
	{
		float norm = sqrt(pow(cos(angle / 2), 2) + sin(angle / 2)*norm_vector);
		scalar = cos(angle / 2); //scalar part
		float sin_scale = sin(angle / 2);
		Vector[0] = vector[0] * sin_scale / norm; //Vector components
		Vector[1] = vector[1] * sin_scale / norm;
		Vector[2] = vector[2] * sin_scale / norm;
	}
}

Quarternion::Quarternion()
{
	Vector[0] = 1;
	
	for (int k = 0; k < 3; ++k)
	{
		Vector[k + 1] = 0;
	}
}

Quarternion Quarternion::operator*( Quarternion &other) const
{
	Quarternion result;
	result.scalar = other.scalar*scalar - Dot(other.Vector, Vector); //Set the scalar part
	glm::vec3 cross_prod = Cross_prod(other.Vector, Vector);
	
	for (int k = 0; k < 3; ++k)
	{
		result.Vector[k] = other.scalar*Vector[k] + scalar * other.Vector[k] + cross_prod[k];
	}

	return result;
}

void Quarternion::Invert()
{
	Vector = -Vector;
}

glm::mat4 Quarternion::Matrix_Rep() const
{
	glm::mat4 result;
	// the matrix representation of a Quarternion assuming we have unit norm
	result = glm::mat3(glm::vec3(1 - 2 * pow(Vector.y, 2) - 2 * pow(Vector.z, 2), 2 * Vector.x*Vector.y - 2 * Vector.z*scalar, 2 * Vector.x*Vector.z + 2 * Vector.y*scalar), glm::vec3(2 * Vector.x*Vector.y + 2 * Vector.z*scalar, 1 - 2 * pow(Vector.x, 2) - 2 * pow(Vector.z, 2), 2 * Vector.y*Vector.z - 2 * Vector.x*scalar), glm::vec3(2 * Vector.x*Vector.z - 2 * Vector.y*scalar, 2 * Vector.y*Vector.z + 2 * Vector.x*scalar, 1 - 2 * pow(Vector.x, 2) - 2 * pow(Vector.y, 2)));
	return result;
}


glm::vec3 Quarternion::Conjugation(glm::vec3 &vector) const
{
	Quarternion* temp_quat=new Quarternion;
	Quarternion Inverse;
	//Inverse Scalar
	Inverse.Vector = -Vector; 
	Inverse.scalar = scalar;
	//Temp quarternion representing the vector
	temp_quat->Vector = vector; // Set the vector part as vector with out normalizing (we don't need to)
	temp_quat->scalar = 0;  // Set the Scalar part as 0

	glm::vec3 result;

	result = ((*this)*(*temp_quat)*Inverse).Vector;   //qvq^{-1} conjugation to rotate the vector with respect to q

	return result;
}



class Stuff
{

};


#endif