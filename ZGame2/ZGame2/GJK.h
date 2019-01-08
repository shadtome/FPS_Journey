#ifndef GJK_H
#define GJK_H

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "BasicMath.h"

//Forward Deceleration of a function
std::vector<float> Minkowski_Dif(std::vector<float> &object1, std::vector<float> &object2);


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

bool Simplex::ContainsOrigin(glm::vec3 &dir, int &orientation)
{
	//The vertices are named A,B,C,D in order
	//Find the normals to all the faces that are connected with the last vertex that was added (since the face opposite of it, the veroni region does not contain the origin
	if (orientation == -1)
	{
		
		if (Orientation(Vertices[3], Vertices[1], Vertices[0], glm::vec3(0, 0, 0)) == 1)
		{
			//replace vertex 2 by vertex 3 since it is irrevelent now
			Vertices[2] = Vertices[3];
			dir = Cross_prod(Vertices[1] - Vertices[2], Vertices[0] - Vertices[2]);
			
			return false;
		}

		if (Orientation(Vertices[3], Vertices[2], Vertices[1], glm::vec3(0, 0, 0)) == 1)
		{
			//replace vertex 0 by vertex 3
			Vertices[0] = Vertices[3];
			dir = Cross_prod(Vertices[2] - Vertices[0], Vertices[1] - Vertices[0]);
			
			return false;
		}

		if (Orientation(Vertices[3], Vertices[0], Vertices[2], glm::vec3(0, 0, 0)) == 1)
		{
			//replace vertex 0 by vertex 3
			Vertices[1] = Vertices[3];
			dir = Cross_prod(Vertices[0] - Vertices[1], Vertices[2] - Vertices[1]);
			
				return false;
		}
	}

	// Now for the other orientation case
	if (orientation == 1)
	{
		
		if (Orientation(Vertices[3], Vertices[0], Vertices[1], glm::vec3(0, 0, 0)) == 1)
		{
			//replace vertex 2 by vertex 3 since it is irrevelent now
			Vertices[2] = Vertices[3];
			dir = Cross_prod(Vertices[0] - Vertices[2], Vertices[1] - Vertices[2]);
			
			return false;
		}

		if (Orientation(Vertices[3], Vertices[1], Vertices[2], glm::vec3(0, 0, 0)) == 1)
		{
			//replace vertex 0 by vertex 3
			Vertices[0] = Vertices[3];
			dir = Cross_prod(Vertices[1] - Vertices[0], Vertices[2] - Vertices[0]);
			
				return false;
		}

		if (Orientation(Vertices[3], Vertices[2], Vertices[0], glm::vec3(0, 0, 0)) == 1)
		{
			//replace vertex 0 by vertex 3
			Vertices[1] = Vertices[3];
			dir = Cross_prod(Vertices[2] - Vertices[1], Vertices[0] - Vertices[1]);
				return false;
		}
	}
	
	// since they are not in any of the Voronnoi Regions outside of the simplex, then it must be in the simplex
	return true;
}


//








// Something better then the expensive collision test (the hyperplane sepertaion theorem) is to use
// the GJK algorthim (Gilbert-Johnson-Keerthi Distance algorthm) that finds the distance between two convex sets using a iterative method
// by looking athe Minkowski difference between the two convex sets and finding the distance to the origin 

bool GJK_Col_3D(std::vector<float> &object1, std::vector<float> &object2)
{
	// We will be working with the Minkowski difference between these two convex regions and determing if the origin is contained in this bigger convex region
	// More specifically object1 -object2;
	//Set Initial Directional Vector
	glm::vec3 Dir = { 1.0,0.0,0.0 };
	glm::vec3 Neg_Dir = Negate(Dir);

	//Define the simplex
	Simplex simplex;

	// Determine the first vertex of the simplex
	simplex.Vertices[0] = Supporting_Function(Dir, object1) - Supporting_Function(Neg_Dir, object2);
	simplex.Vertices[1] = Supporting_Function(Neg_Dir, object1) - Supporting_Function(Dir, object2);

	Dir = { 0.0, 1.0, 1.0 };
	Neg_Dir = Negate(Dir);

	simplex.Vertices[2] = Supporting_Function(Dir, object1) - Supporting_Function(Neg_Dir, object2);


	//We have a triangle, so we are determing which side of the supporting plane for the triangle that the origin is on and then
	//determinet he vertex of the region to make a complete simplex
	int orientation = Orientation(simplex.Vertices[0], simplex.Vertices[1], simplex.Vertices[2], glm::vec3(0, 0, 0));
	
	//Obtain the normal to the face in the correct direction
	Dir =(float)orientation*Cross_prod(simplex.Vertices[1] - simplex.Vertices[0], simplex.Vertices[2] - simplex.Vertices[0]);


	
	// The iterative part of the algorthim where we keep moving the simplex around till we get a collision in or not.
	while (true)
	{
		
		//If orientation is 0, this means it is on the plane determined by these and we need to go to the 2D case
		if (orientation == 0)
		{
			
			//Do the 2D case for a plane that they are coplaner
			//Idea is to go back one step and find the normal to the plane that they are coplaner on
			// Then find a new vertex [2] in this direction and then hopefully this will result in a 3D case as usual
			glm::vec3 New_Dir = Cross_prod(simplex.Vertices[1] - simplex.Vertices[0], simplex.Vertices[2] - simplex.Vertices[0]);
			glm::vec3 Neg_New_Dir = Negate(New_Dir);
			simplex.Vertices[2] = Supporting_Function(New_Dir, object1) - Supporting_Function(Neg_New_Dir, object2);

			orientation = Orientation(simplex.Vertices[0], simplex.Vertices[1], simplex.Vertices[2], glm::vec3(0, 0, 0));
			Dir = (float)orientation*Cross_prod(simplex.Vertices[1] - simplex.Vertices[0], simplex.Vertices[2] - simplex.Vertices[0]);

		}
		
		Neg_Dir = Negate(Dir);
		//Find the 4 vertex to the simplex
		simplex.Vertices[3] = Supporting_Function(Dir, object1) - Supporting_Function(Neg_Dir, object2);
		
		if (Dot(Dir, simplex.Vertices[3]) <= 0)
		{
			return false;
		}
		

		//Determine if the origin is in the simplex
		if (simplex.ContainsOrigin(Dir,orientation))
			return true; // meaning the two objects intersect

	}
}




//Compute the Minkowski Difference for two objects
std::vector<float> Minkowski_Dif(std::vector<float> &object1, std::vector<float> &object2)
{
	std::vector<float> result;
	for (int k = 0; k < object1.size() / 3; ++k)
	{
		for (int j = 0; j < object2.size() / 3; ++j)
		{
			result.push_back(object1[3 * k] - object2[3 * j]);
			result.push_back(object1[3 * k + 1] - object2[3 * j + 1]);
			result.push_back(object1[3 * k + 2] - object2[3 * j + 2]);

		}
	}
	return result;
}

#endif
