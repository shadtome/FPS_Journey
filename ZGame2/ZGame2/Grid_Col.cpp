
#include "Grid_Col.h"
#include "ConfigureManager.h"


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

	//Input the information in to the grid 
	// THis is for the center
	//Grid[col.box.Center] = &col;
	// Now lets do the vertices of the AABB so we can get the most range of the collision
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