#ifndef COLLISION_H
#define COLLISION_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector> 
#include "BasicMath.h"




// Goals for the Collision header
//---------------------------------------------
/* 1) make a sphere bounding region and make it where it can construct its self for any convex region
	2) Then make a hierchay of bounding regions, so big circle, and check if there is other objects in the same big circle as the specific one
	3) Then do a grid based detection in that neighborhood 
	4) then do local detection (the most expensive) which might require linear programming and some statisitcs to best figure out if detection is close as possible
	Most of this will be with squares and simplices, since they are the most primitive convex regions.*/






//Region R={(x,y,z) : |c.x-x|<=rx, |c.y - y|<= ry, |c.z - z| <= rz}
class AABB
{
public:
	glm::vec3 Center; // Center of the box
	float Radius[3];	//Radius with repsect to each coordinate axis direction

	//Compile the data for this AABB
	void CompileAABB(glm::vec3 center, float* radius);

	//Intersection testing for AABB
	int InterTestAABB(AABB box);

	// Construct a best fit AABB for a convex object
	void ConstructAABB_Points(std::vector<float> points);

	// Update AABB
	void UpdateAABB(glm::vec3 center);
};


//-----------------------------------------------------------
//End of AABB class information

//Beginning of Sphere collision class
//----------------------------------------------------------
// Region defined as R={(x,y,z) | (x-c.x)^2+(y-c.y)^2+(z-c.z)^2 <= r^2 }
class ColSphere
{
public:
	//Radius of the sphere
	float Radius;

	//Center of the sphere
	glm::vec3 Center;


	//Collision between two spheres
	int InterSphere(ColSphere sphere);

	// Compile a sphere using the data from a AABB
	void CompileSphere(AABB box);

	//Construct a sphere using the defining vertices for a polygon
	void Sphere_Points(std::vector<float> points);

	//Update the sphere
	void UpdateSphere(glm::vec3 center, float radius);
		

};







#endif