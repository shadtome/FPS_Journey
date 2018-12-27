#ifndef COLLISIONSYST_H
#define COLLISIONSYST_H

#include "Components.h"
#include "Collision.h"
#include "Entity_Manager.h"


// --------------------------------------------
// This is the Entity System for Collision

// Construct the AABB and the boudning spheres given model Data.
void Setup_Collision(Model &model,Collision &col)
{
	int skip = 0;
	for (int k = 0; k < model.Number_Vertex_Attributes; ++k)
	{
		skip += model.Number_Tuples[k];
	}
	
	std::vector<float> points;
	
	//strip all the uneccessary data associated with the vertex attributes that is not the positions.
	for (int k = 0; k < model.Number_Vertices; ++k)
	{
		points.push_back( model.scale[0]*model.Vertices[skip*k]);
		points.push_back( model.scale[1]*model.Vertices[skip*k + 1]);
		points.push_back(model.scale[2]*model.Vertices[skip*k + 2]);
		
	}
	
	//Set the Collision Data
	col.box.ConstructAABB_Points(points); // Construct the AABB
	col.sphere.CompileSphere(col.box); // Using the AABB, construct the Bounding Sphere
	col.Original_Pos = col.box.Center; // Sets the Original Position in local coordinates
	
}

//Set up Collision Data for the "world" (Entity_Manager)
void Setup_Collision_World(Entity_Manager &world)
{
	for (int k = 0; k < world.components.E_Col.Data.size(); ++k)
	{
		Setup_Collision(world.components.E_Model.Data[world.components.E_Col.Data[k].Model_ID], world.components.E_Col.Data[k]);
	}
}

// Check Collision between entities
//This is not done, since we need a result after collision effect (like death, push back by physics, ect..)
void Check_Col_World(Entity_Manager &world)
{
	for (int k = 0; k < world.components.E_Col.Data.size(); ++k) // This is horrible( this is comlexity O(n^2), but I have a way to fix this using the grid, once I get to that
	{
		for (int j = k + 1; j < world.components.E_Col.Data.size(); ++j)
		{
			
			if (world.components.E_Col.Data[k].box.InterTestAABB(world.components.E_Col.Data[j].box))
			{
				// Do something (the AI and physics functions go here between the two objects
				//This will be very long function just becuase of this
				//std::cout << "Entity" << k << "and" << j << " INtersect" << std::endl;
			}
		}
	}
}



//Begininng of Grid class (the gride partitoins the space in to uniform blocks to lessen the amount of expensive testing.
//----------------------------------------------------------------------------
//Use Hashing Function
const int NUM_BUCKETS = 1024;
//The Hashing Function
int Hashing_Grid(glm::vec3 point, int prime)
{
	std::vector<int> result;
	result[0] = FindIntPart(prime, point.x);
	result[1] = FindIntPart(prime, point.y);
	result[2] = FindIntPart(prime, point.z);

	const int h1 = 0x8da6b343; // Large multiplication constants
	const int h2 = 0xd8163841;
	const int h3 = 0xcb1ab31f;

	int n = h1 * result[0] + h2 * result[1] + h3 * result[2];
	n = n % NUM_BUCKETS;
	if (n < 0)
	{
		n += NUM_BUCKETS;
	}


	return n;
}





class Grid
{
public:
	//Number of Buckets
	int Buckets=NUM_BUCKETS;
	// Map to hold all the data.  Set up each input will output a vector that points to the associated data
	std::vector<std::vector<Model*>> grid;

	//Prime for the Hashing function
	int prime;

	//Set Grid
	void Set_Grid();

	//Input the information 
	void Input_Into_Grid(Model &model);
};

//Grid Methods/Functions

void Grid::Set_Grid()
{
	for (int k = 0; k < Buckets; ++k)
	{
		
	}
}



void Grid::Input_Into_Grid(Model &model)
{
	// Find the location of hashing the vertex position
	int n = Hashing_Grid(model.pos, prime);

	//Input the information in to the grid
	


}


//End of Grid Class
// ---------------------------------------------

class Color
{

};

#endif
