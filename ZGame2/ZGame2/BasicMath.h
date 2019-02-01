#ifndef BASICMATH_H
#define BASICMATH_H

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

//--------------------------------------------------------------
//Basis Math

//Dot Product

float Dot(std::vector<float> &V, std::vector<float> &W);
float Dot(const std::vector<float> &V, const std::vector<float> &W);
float Dot(glm::vec3 V, glm::vec3 W);
float Dot(float V[3], float W[3]);
float Dot(const float V[3], const float W[3]);


//Negate Vector
glm::vec3 Negate(glm::vec3 &vector);

//Cross Product
std::vector<float> Cross_prod(std::vector<float> &vec1, std::vector<float> &vec2);
std::vector<float> Cross_Prod(float vec1[3], float vec2[3]);
std::vector<float> Cross_Prod(const float vec1[3], const float vec2[3]);
glm::vec3 Cross_prod(glm::vec3 vec1, glm::vec3 vec2);


//Orientation for 4 points
int Orientation(glm::vec3 &A, glm::vec3 &B, glm::vec3 &C, glm::vec3 D);

//Find the extremal points in for a region in the direction of a vector
void ExtremePointesAlongDirection(std::vector<float> dir, std::vector<float> &vertex_coord, int n, int* imin, int* imax);
void ExtremePointesAlongDirection(glm::vec3 &dir, std::vector<float> &vertex_coord, int n, int* imin, int* imax);

//Supporting function for a convex region in the direction of a vector
glm::vec3 Supporting_Function(glm::vec3 &dir, std::vector<float> &points);

//Find the Int Part of a number
int FindIntPart(int prime, float x);








/*
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
*/






#endif
