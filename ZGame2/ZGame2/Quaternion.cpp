#include "Quaternion.h"



Quarternion::Quarternion(float a_1, float a_2, float a_3, float angle)
{
	this->Angle = angle;
	float norm_vector = pow(a_1, 2) + pow(a_2, 2) + pow(a_3, 2); //Avoid squareroot if we dont need it
	if (norm_vector == 1)
	{
		scalar = cos(angle / 2); //scalar part
		float sin_scale = sin(angle / 2);
		Vector[0] = a_1 * sin_scale; //Vector components
		Vector[1] = a_2 * sin_scale;
		Vector[2] = a_3 * sin_scale;

	}

	else
	{
		float norm = sqrt(pow(cos(angle / 2), 2) + sin(angle / 2)*norm_vector);
		scalar = cos(angle / 2); //scalar part
		float sin_scale = sin(angle / 2);
		Vector[0] = a_1 * sin_scale / norm; //Vector components
		Vector[1] = a_2 * sin_scale / norm;
		Vector[2] = a_3 * sin_scale / norm;
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


/*Quarternion::Quarternion(glm::vec3 vector, float angle)
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
}*/


Quarternion::Quarternion(glm::vec3 vector, float scalar)
{
	this->Vector = vector;
	this->scalar;
	Norm();
}

Quarternion::Quarternion()
{
	this->scalar = 1;

	for (int k = 0; k < 3; ++k)
	{
		this->Vector[k] = 0;
	}
}

float Quarternion::Norm() const
{
	return sqrt(pow(this->scalar, 2) + pow(this->Vector[0], 2) + pow(this->Vector[1], 2) + pow(this->Vector[2], 2));
}

Quarternion Quarternion::operator*(const Quarternion &other) const
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


Quarternion Quarternion::operator+(Quarternion other) const
{
	Quarternion result;
	result.Vector = this->Vector + other.Vector;  //Note that this is not unit anymore, i might just normalize this anyways, we will see
	result.scalar = this->scalar + other.scalar;

	return result;
}


Quarternion Quarternion::operator*(const float scale) const
{
	Quarternion result;
	result.Vector = scale * this->Vector;
	result.scalar = scale * this->scalar;
	return result;
}

Quarternion Quarternion::Invert()
{
	Quarternion temp;
	temp.Vector =-this->Vector;
	temp.scalar = this->scalar;
	return temp;
}

glm::mat3 Quarternion::Matrix_Rep() const
{
	glm::mat3 result;
	// the matrix representation of a Quarternion assuming we have unit norm
	result = glm::mat3(glm::vec3(1 - 2 * pow(Vector.y, 2) - 2 * pow(Vector.z, 2), 2 * Vector.x*Vector.y - 2 * Vector.z*scalar, 2 * Vector.x*Vector.z + 2 * Vector.y*scalar), glm::vec3(2 * Vector.x*Vector.y + 2 * Vector.z*scalar, 1 - 2 * pow(Vector.x, 2) - 2 * pow(Vector.z, 2), 2 * Vector.y*Vector.z - 2 * Vector.x*scalar), glm::vec3(2 * Vector.x*Vector.z - 2 * Vector.y*scalar, 2 * Vector.y*Vector.z + 2 * Vector.x*scalar, 1 - 2 * pow(Vector.x, 2) - 2 * pow(Vector.y, 2)));
	return result;
}


glm::vec3 Quarternion::Conjugation(glm::vec3 &vector) const
{
	Quarternion temp_quat;
	Quarternion Inverse;
	//Inverse Scalar
	Inverse.Vector = -Vector;
	Inverse.scalar = scalar;
	//Temp quarternion representing the vector
	temp_quat.Vector = vector; // Set the vector part as vector with out normalizing (we don't need to)
	temp_quat.scalar = 0;  // Set the Scalar part as 0

	glm::vec3 result;

	result = ((*this)*(temp_quat)*Inverse).Vector;   //qvq^{-1} conjugation to rotate the vector with respect to q

	return result;
}


//General functions for Quaternions

glm::vec3 sgn(const glm::vec3 vector)
{
	glm::vec3 result;
	float norm = Norm(vector);
	
	if (norm == 0)
	{
		return result;		//return the trivial vector
	}
	else
	{
		result = vector*(1 / norm);
		return result;
	}
}

float Arg(const Quarternion quat)
{
	float norm = quat.Norm();
	if (norm == 0)
	{
		return 1;			//Undefined, show I make this say undefined and not compute it?
	}
	else
	{
		return acos(quat.scalar / norm);
	}
}

Quarternion exp(const Quarternion quat)
{
	Quarternion result;
	result.Vector = exp(quat.scalar)*(float)sin(Norm(quat.Vector))*sgn(quat.Vector);
	result.scalar = exp(quat.scalar)*(cos(Norm(quat.Vector))*quat.scalar);					//exp(a)*(cos(1.0)+sin(1.0) a)



	return result;
}

Quarternion Ln(const Quarternion quat)
{
	Quarternion result;
	result.Vector = sgn(quat.Vector)*Arg(quat);
	result.scalar = log(quat.Norm());
	return result;
}

Quarternion Pow(const Quarternion quat,const float w)
{
	return exp(Ln(quat)*w);
}
