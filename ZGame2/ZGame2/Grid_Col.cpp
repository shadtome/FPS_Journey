
#include "Grid_Col.h"
#include "ConfigureManager.h"


//Grid Methods/Functions

void Grid::Set_Grid()
{

}



void Grid::Input_Into_Grid(Collision &col)
{

	//Determine the hash function using this prime
	Configure_Manager::PRIME = prime;
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


	//Go through and input the vertices of the AABB box in to the hash grid, and not to input more then one vertices if they are in the same grid
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			for (int k = 0; k < 2; ++k)
			{
				Vertex = col.box.Center + glm::vec3(pow(-1, i)*col.box.Radius[0], pow(-1, j)* col.box.Radius[1], pow(-1, k)*col.box.Radius[2]);
				int m = hasher(Vertex);
				int counter = 0;
				for (unsigned int t = 0; t < Hashed.size(); ++t)
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

	
	//Determine the hash function with chosen prime number
	Configure_Manager::PRIME = prime;
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


	//Input the 6 different sides of the sphere nin to the grid and make sure that the sphere is not put in tot eh same grid multply times
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			for (int k = 0; k < 2; ++k)
			{
				Vertex = col.sphere.Center + glm::vec3(pow(-1, i)*col.sphere.Radius, pow(-1, j)* col.sphere.Radius, pow(-1, k)*col.sphere.Radius);
				int m = hasher(Vertex);
				int counter = 0;
				for (unsigned int t = 0; t < Hashed.size(); ++t)
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
	for (unsigned int k = 0; k < col.List_Vertices.size(); ++k)
	{
		Grid.erase(col.List_Vertices[k]);
	}
}


//End of Grid Class
// ---------------------------------------------