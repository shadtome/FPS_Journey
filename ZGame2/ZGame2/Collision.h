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


//Class AABB functions
//-------------------------------------------------------------
void AABB::CompileAABB(glm::vec3 center, float* radius)
{
	Center = center;
	for (int k = 0; k < 3; ++k)
	{
		Radius[k] = radius[k];
	}
}

int AABB::InterTestAABB(AABB box)
{
	if (abs(Center.x - box.Center.x) > (Radius[0]  + box.Radius[0] ))
	{
		//std::cout << abs(Center.x - box.Center.x) << " > " << (Radius[0]/2 + box.Radius[0]/2) << std::endl;
		//std::cout << abs(Center.x - box.Center.x)<< " > " << (Radius[0] + box.Radius[0] ) << std::endl;
		//std::cout << Center.x << "-" << box.Center.x << " >" << 1 << std::endl;
		return 0;
	}
	if (abs(Center.y - box.Center.y) > (Radius[1] + box.Radius[1]))
		return 0;
	if (abs(Center.z - box.Center.z) > (Radius[2] + box.Radius[2]))
		return 0;

	return 1;

};

void AABB::ConstructAABB_Points(std::vector<float> points)
{	
	

	//Indices for the different direction for the max and min distance away from the origin
	int imin_x, imax_x, imin_y, imax_y, imin_z, imax_z; 

	// positive x direction
	ExtremePointesAlongDirection({ 1,0,0 }, points, points.size() / 3, &imin_x, &imax_x); //  find the points that are max and min distance away from the origin in the x-direction
	//Set the Radius in the x direction 
	Radius[0] = abs(points[3*imin_x] - points[3*imax_x]) / 2;
	//Find the x coordinate of the center
	Center.x = points[3*imax_x] - Radius[0];

	// positive y-direction
	ExtremePointesAlongDirection({ 0,1,0 }, points, points.size() / 3, &imin_y, &imax_y); // Find the points that are max and min in the y -direction
	//set the Radius in the y -direction
	Radius[1] = abs(points[3*imin_y + 1] - points[3*imax_y + 1]) / 2;
	//Find the y-coordinate of the center
	Center.y = points[3*imax_y + 1] - Radius[1];

	// positive z-direction
	ExtremePointesAlongDirection({ 0,0,1 }, points, points.size() / 3, &imin_z, &imax_z); // Find the points that are max and min in the z-direction
	// Set the Radius in the z-direction
	Radius[2] = abs(points[3*imin_z + 2] - points[3*imax_z + 2]) / 2;
	//Find the z-coordinate of the center
	Center.z = points[3*imax_z + 2] - Radius[2];


	//std::cout << Center.x << " " << Center.y << " " << Center.z << std::endl;
	//std::cout << Radius[0]<< Radius[1]<< Radius[2] << std::endl;

};

void AABB::UpdateAABB(glm::vec3 center)
{
	Center = center;
}



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


int ColSphere::InterSphere(ColSphere sphere)
{
	if (pow(Center.x - sphere.Center.x, 2) + pow(Center.y - sphere.Center.y, 2) + pow(Center.z - sphere.Center.z, 2) > pow(Radius + sphere.Radius, 2))
	{
		return 0;
	}
 
	return 1;
};

void ColSphere::CompileSphere(AABB box)
{
	Center = box.Center;
	float radius=0;
	for (int k = 0; k < 3; ++k)
	{
		radius = radius + pow(box.Radius[k], 2);
	}
	Radius = radius;
};

void ColSphere::Sphere_Points(std::vector<float> points)
{
	AABB box;
	box.ConstructAABB_Points(points);
	CompileSphere(box);
};

void ColSphere::UpdateSphere(glm::vec3 center, float radius)
{
	Center = center;
	Radius = radius;
};


// -----------------------------------------------------------------
// End of Collision sphere class




#endif