#ifndef BASICMATH_H
#define BASICMATH_H

#include <iostream>
#include <vector>

//--------------------------------------------------------------
//Basis Collision Math

//Take the dot product of two Vectors
float Dot(std::vector<float> V, std::vector<float> W)
{
	float result = 0;
	for (int k = 0; k < V.size(); ++k)
	{
		result = result + V[k] * W[k];
	}
	return result;
}


// There is a better way to do this, use linear programming by rotating the towards the correct direction and knowing that Convex polygons are the intersection of half planes.  This will reduce the computations
// Function that determines the points that correspdon to the max or min distance along a directional vector.
void ExtremePointesAlongDirection(std::vector<float> dir, std::vector<float> vertex_coord, int n, int* imin, int* imax)
{
	float minproj = FLT_MAX, maxproj = -FLT_MAX; // Set the min proj distance as the max representation of a float, and let the max proj distance be the negative of the maximum representation

	for (int i = 0; i < n/3; ++i)
	{
		std::vector<float> vec = { vertex_coord[3*i], vertex_coord[3*i + 1], vertex_coord[3*i + 2] };
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


// Qausi Hash function of my own design to partition the space in to grids based on the size of the largest object and the number of buckets (actually I could have a infinte number of buckets)
int FindIntPart(int prime, float x)
{
	int result;
	result= ((int)x) - ((int)x) % prime; // Find the largest multply of the prime that is less then x
	return result;
}
// This does not work right now, look back at python code for this (for somereason it crashes) Also do a where function that will find the index where it is true which is where the primes are)
// Find a list of primes less then equal to a integer
std::vector<bool> Prime_list(int n)
{
	int sqrt_n = (int)sqrt(n);
	std::vector<bool> primes(n + 1, true); // Makes a list of length n+1 of True to start
	primes[0] = false; // 0 is not a prime
	primes[1] = false; // 1 is not a prime
	for (int k = 0; k < (int)(n - 2) / 2; ++k)
	{
		primes[2 * k + 4] = false;
	}
	int j = 3;
	while (j < sqrt_n)
	{
		if (primes[j] == true) // Check at the jth index if it is true or false
		{
			for (int k = 0; k < (int)(n - j * j - 1) / (2 * j) + 1; ++k) 
			{
				primes[j*j + 2 * k] = false; // Place all the multiples of j except j by False by starting at j*j
			}
		}
	}
	
	return primes;
}
// Find the smallest prime that is bigger then a number
int Infimum_Prime(float x)
{
	return 1;
}








#endif
