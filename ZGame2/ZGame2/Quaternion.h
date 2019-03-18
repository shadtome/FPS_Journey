#ifndef QUARTERNION_H
#define QUARTERNION_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include "BasicMath.h"
#include <math.h>

//Make this a general class, and then make the derived class for unit Quaternions? probobly the safest route. Need to do this, now that I need to have non unit Quarternions 
//Unless I use Slerp, which is a Geodesic on the 4 dimensional sphere, and hence the image of the quarternion is still on the Sphere
class Quarternion  // a unit quarternion is represented as cos(theta/2)+sin(theta/2)A, where A=ai+bj+ck. with norm 1
{
public:
	glm::vec3 Vector;  //Ai+Bj+Ck vector part

	float scalar; //scalar part

	float Angle;

	//Constructor with variables
	 Quarternion(float a_1, float a_2, float a_3, float angle);
	 
	 //Constructor with GLM Vector, Will normalize the vector to make it in to a quarternion if needed
	 //Quarternion(glm::vec3 vector, float angle);

	 //Constructor with std::vector
	 Quarternion(std::vector<float> vector, float angle);

	 //Constructor with vector and scalar
	 Quarternion(glm::vec3 vector, float scalar);

	 //Constructor with out varialbes, so the null Quarternion
	 Quarternion();

	 //Initalize information for the Quarternion
		// function that takes in variables, like the constructors and make sure they are normal and initalizes it ready for use

	 //Quarternion norm
	 float Norm() const;

	 //override the mulplication operator between two Quarternions
	 Quarternion operator*(const Quarternion &other) const;


	 //Multiply Scalars on to the Quarternion
	 Quarternion operator*(const float scale) const;

	 //Add two Quarternions, but note that it wont be normalized anymore
	 Quarternion operator+(Quarternion other) const;

	 //Inverse/conjugate (they are the same for unit quarternions
	 Quarternion Invert();

	 //Matrix Representation in GL(4,RR)
	 glm::mat3 Matrix_Rep() const;

	 //Rotate a given vector by the Quarternion the group action of H^* on to R^3 by conjugation
	 glm::vec3 Conjugation(glm::vec3 &vector) const;

	


	 //Make a operator[] overload?

};



//General functions for Quarternions
//arguemnt value (I did not do for a general non unit Quarternion for these functions, change later)
float Arg(const Quarternion quat);

//Sign function (norm)
glm::vec3 sgn(const glm::vec3 vector);

//exponential function 
Quarternion exp(const Quarternion quat);

//Log function
Quarternion Ln(const Quarternion quat);			

//Power function for Quarternion
Quarternion Pow(const Quarternion quat, float w);



#endif