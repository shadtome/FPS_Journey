#ifndef GRID_H
#define GRID_H


#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory.h>
#include "BasicMath.h"
#include "Components.h"
#include "ConfigureManager.h"

//Forward Decelerations


//Begininng of Grid class (the gride partitoins the space in to uniform blocks to lessen the amount of expensive testing.
//----------------------------------------------------------------------------
//Use Hashing Function
const int NUM_BUCKETS = 1024;

// Hash Combine (This is from boost library)
template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
	std::hash<T> hasher;
	seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2); // The number there is the golden Ratio
}



// Hash function for a vector
template<>
struct std::hash<glm::vec3>
{
	/*std::size_t operator()(const glm::vec3 &point) const
	{
		const int h1 = 0x8da6b343;
		const int h2 = 0xd8163841;
		const int h3 = 0xcb1ab31f;
		return  std::hash<int>()(FindIntPart(PRIME, point[0])) +  std::hash<int>()(FindIntPart(PRIME, point[1])) +  std::hash<int>()(FindIntPart(PRIME, point[2]));
	}*/

	inline std::size_t operator()(const glm::vec3 &point) const
	{
		std::size_t result;
		result = std::hash<int>()(FindIntPart(Configure_Manager::PRIME, point[0]));
		hash_combine(result, FindIntPart(Configure_Manager::PRIME, point[1]));
		hash_combine(result, FindIntPart(Configure_Manager::PRIME, point[2]));
		return result;
	}
};



class Grid
{
public:
	//Number of Buckets
	int Buckets = NUM_BUCKETS;
	// Map to hold all the data.  Set up each input will output a vector that points to the associated data
	std::unordered_map<glm::vec3, Collision*> Grid;

	//Prime for the Hashing function
	int prime=1;

	//Set Grid
	void Set_Grid();

	//Input the information 
	void Input_Into_Grid(Collision &col);

	//Input the information for the sphere 
	void Input_Into_Grid_Sphere(Collision &col);

	//Delete the Information in the Grid
	void Delete_Grid_Pos(Collision &col);
};







#endif
