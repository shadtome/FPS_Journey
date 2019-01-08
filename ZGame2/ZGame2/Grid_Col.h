#ifndef GRID_H
#define GRID_H


#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory.h>
#include "BasicMath.h"


//Forward Decelerations
class Collision;


//NOTE: There is something wrong with the GRID system right now, since it uses a ton of memory as you move our character, since
// It is filling up the unordered map to the max and doing a leakage all over, hence slowing it all down.
// Should I just allocate the memory everytime I use it? seems a waste, but it is fast still.



//Begininng of Grid class (the gride partitoins the space in to uniform blocks to lessen the amount of expensive testing.
//----------------------------------------------------------------------------
//Use Hashing Function
const int NUM_BUCKETS = 1024;
int PRIME = 3;

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
		result = std::hash<int>()(FindIntPart(PRIME,point[0]));
		hash_combine(result, FindIntPart(PRIME, point[1]));
		hash_combine(result, FindIntPart(PRIME, point[2]));
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

//Grid Methods/Functions

void Grid::Set_Grid()
{
	
}



void Grid::Input_Into_Grid(Collision &col)
{
	
	//Input the information in to the grid 
	// THis is for the center
	//Grid[col.box.Center] = &col;
	// Now lets do the vertices of the AABB so we can get the most range of the collision
	PRIME = prime;
	std::hash<glm::vec3> hasher;
	 // Initalize the center as the starting point for testing the hashed location
	//Hash the Center for a starting point
	int q = hasher(col.box.Center);

	//Add the Center in to the Grid
	Grid[col.box.Center] = &col;

	glm::vec3 Vertex;  // The vectex vector that will be tested for which grid it is in
	// List of Hash values
	std::vector<int> Hashed;
	// Put the center as the first hashed value
	Hashed.push_back(q);
	
	

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			for (int k = 0; k < 2; ++k)
			{
				Vertex = col.box.Center + glm::vec3(pow(-1, i)*col.box.Radius[0], pow(-1, j)* col.box.Radius[1], pow(-1, k)*col.box.Radius[2]);
				int m = hasher(Vertex);
				int counter = 0;
				for (int t = 0; t < Hashed.size(); ++t)
				{
					if (Hashed[t] != m)
					{
						counter += 1;
					}
					if (counter == Hashed.size())
					{
						Hashed.push_back(m);
						//std::cout << Vertex[0] << "::" << Vertex[1] << "::" << Vertex[2] << "::" << std::endl;
						Grid[Vertex] = &col;
						
					}
				}
				Vertex = glm::vec3();
			}
		}
	}
	
}

void Grid::Input_Into_Grid_Sphere(Collision &col)
{
	
	//Input the information in to the grid 
	// THis is for the center
	//Grid[col.box.Center] = &col;
	// Now lets do the vertices of the AABB so we can get the most range of the collision
	PRIME = prime;
	std::hash<glm::vec3> hasher;
	// Initalize the center as the starting point for testing the hashed location
   //Hash the Center for a starting point
	int q = hasher(col.box.Center);

	//Add the Center in to the Grid
	Grid[col.box.Center] = &col;

	glm::vec3 Vertex;  // The vectex vector that will be tested for which grid it is in
	// List of Hash values
	std::vector<int> Hashed;
	// Put the center as the first hashed value
	Hashed.push_back(q);



	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			for (int k = 0; k < 2; ++k)
			{
				Vertex = col.sphere.Center + glm::vec3(pow(-1,i)*col.sphere.Radius, pow(-1, j)* col.sphere.Radius, pow(-1, k)*col.sphere.Radius);
				int m = hasher(Vertex);
				int counter = 0;
				for (int t = 0; t < Hashed.size(); ++t)
				{
					if (Hashed[t] != m)
					{
						counter += 1;
					}
					if (counter == Hashed.size())
					{
						Hashed.push_back(m);
						//std::cout << Vertex[0] << "::" << Vertex[1] << "::" << Vertex[2] << "::" << std::endl;
						Grid[Vertex] = &col;

					}
				}
				Vertex = glm::vec3();
			}
		}
	}
}

void Grid::Delete_Grid_Pos(Collision &col)
{
	for (int k = 0; k < col.List_Vertices.size(); ++k)
	{
		Grid.erase(col.List_Vertices[k]);
	}
}


//End of Grid Class
// ---------------------------------------------





#endif
