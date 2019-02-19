
#include "BasicMath.h"


//----------------------------------------------------------------
//Dot Product 
//Take the dot product of two Vectors
float Dot(std::vector<float> &V, std::vector<float> &W)
{
	float result = 0;
	for (unsigned int k = 0; k < V.size(); ++k)
	{
		result = result + V[k] * W[k];
	}
	return result;
}

//Const version
float Dot(const std::vector<float> &V, const std::vector<float> &W)
{
	float result = 0;
	for (unsigned int k = 0; k < V.size(); ++k)
	{
		result = result + V[k] * W[k];
	}
	return result;
}

//Overloaded Dot product
float Dot(glm::vec3 V, glm::vec3 W)
{
	float result = 0;
	for (int k = 0; k < 3; ++k)
	{
		result = result + V[k] * W[k];
	}
	return result;
}

//Overload Dot Product for arrays
float Dot(float V[3], float W[3])
{
	float result = 0;
	for (int k = 0; k < 3; ++k)
	{
		result = result = V[k] * W[k];
	}
	return result;
}

//Const Version for the Dot Product and array
float Dot(const float V[3], const float W[3])
{
	float result = 0;
	for (int k = 0; k < 3; ++k)
	{
		result = result = V[k] * W[k];
	}
	return result;
}

//-----------------------------------------------------------------
//Norm
float Norm(const glm::vec3 V)
{
	return sqrt(Dot(V, V));
}

//-------------------------------------------------------------------

//Negate a vector
glm::vec3 Negate(glm::vec3 &vector)
{
	return glm::vec3(-vector[0], -vector[1], -vector[2]);
}




//---------------------------------------------------------------------------
//Cross Product of two vectors

//Cross Product of two vectors
std::vector<float> Cross_prod(std::vector<float> &vec1, std::vector<float> &vec2)
{
	std::vector<float> result;
	result = { vec1[1] * vec2[2] - vec2[1] * vec1[2],-vec1[0] * vec2[2] + vec1[2] * vec2[0],vec1[0] * vec2[1] - vec2[0] * vec1[1] }; //vec1 x vec2
	return result;
}

//Overlaod the crossproduct for float array and outputs a vector (since arrays can not be a output for a function, I can fix this by making a Vector class that has all the capabilties that I want)
//Maybe glm::vec3 is sufficient? Might just make my own vector container to be efficient
std::vector<float> Cross_Prod(float vec1[3], float vec2[3])
{
	std::vector<float> result;
	result = { vec1[1] * vec2[2] - vec2[1] * vec1[2],-vec1[0] * vec2[2] + vec1[2] * vec2[0],vec1[0] * vec2[1] - vec2[0] * vec1[1] }; //vec1 x vec2
	return result;
}

//Const version
std::vector<float> Cross_Prod(const float vec1[3], const float vec2[3])
{
	std::vector<float> result;
	result = { vec1[1] * vec2[2] - vec2[1] * vec1[2],-vec1[0] * vec2[2] + vec1[2] * vec2[0],vec1[0] * vec2[1] - vec2[0] * vec1[1] }; //vec1 x vec2
	return result;
}

//Overload the cross product for glm
glm::vec3 Cross_prod(const glm::vec3 vec1, const glm::vec3 vec2)
{
	glm::vec3 result;
	result = { vec1[1] * vec2[2] - vec2[1] * vec1[2],-vec1[0] * vec2[2] + vec1[2] * vec2[0],vec1[0] * vec2[1] - vec2[0] * vec1[1] }; //vec1 x vec2
	return result;
}

// Orientation of 4 points
int Orientation(glm::vec3 &A, glm::vec3 &B, glm::vec3 &C, glm::vec3 D)
{
	float triple_product = Dot(A - D, Cross_prod(B - D, C - D));
	// look at the plane determined by ABC

	if (triple_product < 0) // D lies above the plane supported by ABC in the sense that ABC apear counterclockwise
		return 1;
	if (triple_product > 0) // Dlies below, in the sense that ABC apear clockwise
		return -1;
	return 0; //This means triple_product ==0, and they are coplanar

}


// There is a better way to do this, use linear programming by rotating the towards the correct direction and knowing that Convex polygons are the intersection of half planes.  This will reduce the computations
// Function that determines the points that correspdon to the max or min distance along a directional vector.
void ExtremePointesAlongDirection(std::vector<float> dir, std::vector<float> &vertex_coord, int n, int* imin, int* imax)
{
	float minproj = FLT_MAX, maxproj = -FLT_MAX; // Set the min proj distance as the max representation of a float, and let the max proj distance be the negative of the maximum representation

	for (int i = 0; i < n / 3; ++i)
	{
		std::vector<float> vec = { vertex_coord[3 * i], vertex_coord[3 * i + 1], vertex_coord[3 * i + 2] };
		float proj = Dot(dir, vec); // compute the distance of the point from the center

		// Keep track of least distance point along the directional vector
		if (proj < minproj)
		{
			minproj = proj;
			*imin = i;
		}

		// keep track of most distant point along direction vector
		if (proj > maxproj)
		{
			maxproj = proj;
			*imax = i;
		}
	}
}

// Overload the Extremal points along a direction
void ExtremePointesAlongDirection(glm::vec3 &dir, std::vector<float> &vertex_coord, int n, int* imin, int* imax)
{
	float minproj = FLT_MAX, maxproj = -FLT_MAX; // Set the min proj distance as the max representation of a float, and let the max proj distance be the negative of the maximum representation

	for (int i = 0; i < n / 3; ++i)
	{
		glm::vec3 vec = { vertex_coord[3 * i], vertex_coord[3 * i + 1], vertex_coord[3 * i + 2] };
		float proj = Dot(dir, vec); // compute the distance of the point from the center

		// Keep track of least distance point along the directional vector
		if (proj < minproj)
		{
			minproj = proj;
			*imin = i;
		}

		// keep track of most distant point along direction vector
		if (proj > maxproj)
		{
			maxproj = proj;
			*imax = i;
		}
	}
}

//Supporting function for a convex region (determines the vertex of maximal distance in a certain vector direction d

glm::vec3 Supporting_Function(glm::vec3 &dir, std::vector<float> &points)
{
	int imax;
	int imin;
	ExtremePointesAlongDirection(dir, points, points.size(), &imin, &imax);

	return glm::vec3(points[3 * imax], points[3 * imax + 1], points[3 * imax + 2]);
}



// Qausi Hash function of my own design to partition the space in to grids based on the size of the largest object and the number of buckets (actually I could have a infinte number of buckets)
int FindIntPart(int prime, float x)
{
	int result;

	if (x > 0)
	{
		result = ((int)x) - ((int)x) % prime; // Find the largest multply of the prime that is less then x
	}
	else
	{
		result = ((int)x) + ((int)x) % prime; // Find the largest multply of the prime that is less then x
	}
	return result;
}