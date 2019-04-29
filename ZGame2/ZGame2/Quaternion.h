#ifndef QUARTERNION_H
#define QUARTERNION_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include "BasicMath.h"
#include <math.h>

//Make sure to Change this class to do calculations better.  

//Make this a general class, and then make the derived class for unit Quaternions? probobly the safest route. Need to do this, now that I need to have non unit Quarternions 
//Unless I use Slerp, which is a Geodesic on the 4 dimensional sphere, and hence the image of the quarternion is still on the Sphere
class Quaternion  // a unit quarternion is represented as cos(theta/2)+sin(theta/2)A, where A=ai+bj+ck. with norm 1
{
public:
	glm::vec3 Vector;  //Ai+Bj+Ck vector part

	float scalar; //scalar part

	float Angle;

	//Constructor with variables
	 Quaternion(float a_1, float a_2, float a_3, float angle);
	 
	 //Constructor with GLM Vector, Will normalize the vector to make it in to a quarternion if needed
	 //Quarternion(glm::vec3 vector, float angle);

	 //Constructor with std::vector
	 Quaternion(std::vector<float> vector, float angle);

	 //Constructor with vector and scalar
	 Quaternion(glm::vec3 vector, float scalar);

	 //Constructor with out varialbes, so the null Quarternion
	 Quaternion();

	 //Initalize information for the Quarternion
		// function that takes in variables, like the constructors and make sure they are normal and initalizes it ready for use

	 //Quarternion norm
	 float Norm() const;

	 void Normalize();

	 //override the mulplication operator between two Quarternions
	 Quaternion operator*(const Quaternion &other) const;


	 //Multiply Scalars on to the Quarternion
	 Quaternion operator*(const float scale) const;

	 //Add two Quarternions, but note that it wont be normalized anymore
	 Quaternion operator+(Quaternion other) const;

	 //Inverse/conjugate (they are the same for unit quarternions
	 Quaternion inverse()const;

	 //Matrix Representation in GL(4,RR)
	 glm::mat3 Matrix_Rep() const;

	 //Rotate a given vector by the Quarternion the group action of H^* on to R^3 by conjugation
	 glm::vec3 Conjugation(glm::vec3 &vector) const;

	


	 //Make a operator[] overload?

};







//General functions for Quarternions
//arguemnt value (I did not do for a general non unit Quarternion for these functions, change later)
float Arg(const Quaternion quat);

//Sign function (norm)
glm::vec3 sgn(const glm::vec3 vector);

//exponential function 
Quaternion exp(const Quaternion quat);

//Log function
Quaternion Ln(const Quaternion quat);			

//Power function for Quarternion
Quaternion Pow(const Quaternion quat, float w);



#endif