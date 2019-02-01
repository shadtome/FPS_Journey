#ifndef GJK_H
#define GJK_H

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "BasicMath.h"

//Forward Deceleration of a function
std::vector<float> Minkowski_Dif(std::vector<float> &object1, std::vector<float> &object2);
bool GJK_Col_3D(std::vector<float> &object1, std::vector<float> &object2);


// Class to represent a simplex in R^3
//So a 3-simplex (4 vertices)
class Simplex
{
public:
	//Note that by Cathedories theorem in convex geometry that any element in a convex subset of R^d, can be represetned by d+1 vertices
	//Represents the vertices of the simplex
	glm::vec3 Vertices[4];

	bool ContainsOrigin(glm::vec3 &dir,int &orientation);
};



#endif
