
#include "CollisionSystem.h"
#include "Components.h"
#include "Entity_Manager.h"



/*// Construct the AABB and the boudning spheres given model Data.
void Setup_Collision(Model &model, Collision &col)
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
		/*glm::mat4 model_matrix;
		model_matrix = glm::rotate(model_matrix, model.angle, model.Vector_Rot);
		model_matrix = glm::scale(model_matrix,glm::vec3( model.scale[0], model.scale[1], model.scale[2]));
		glm::vec3 new_point;
		new_point = glm::vec3(model_matrix*glm::vec4(model.Vertices[skip*k], model.Vertices[skip*k+1], model.Vertices[skip*k+2], 1));


		points.push_back(new_point.x);
		points.push_back(new_point.y);
		points.push_back(new_point.z);
		//there should be a "* "/" here

		points.push_back(model.scale[0] * model.Vertices[skip*k]);
		points.push_back(model.scale[1] * model.Vertices[skip*k + 1]);
		points.push_back(model.scale[2] * model.Vertices[skip*k + 2]);

	}

	//Set the Collision Data
	col.box.ConstructAABB_Points(points); // Construct the AABB
	col.sphere.CompileSphere(col.box); // Using the AABB, construct the Bounding Sphere
	col.Original_Pos = col.box.Center; // Sets the Original Position in local coordinates
	col.points = points;

}*/

//Set up collision Data
void Setup_Collision(Model &model, Collision &col)
{
	//Consists of the vertices
	std::vector<float> points;

	//Put together all the vertices for the model
	//Later we can do some specific meshes of the mkodel collision
	for (auto k=model.model->meshes.begin(); k!= model.model->meshes.end(); ++k)
	{
		for (auto v = k->vertices.begin(); v != k->vertices.end(); ++v)
		{
			points.push_back(v->Position.x);
			points.push_back(v->Position.y);
			points.push_back(v->Position.z);
		}
		
		//Set up the collision data
		col.box.ConstructAABB_Points(points); // Construct the AABB
		col.sphere.CompileSphere(col.box); // Using the AABB, construct the Bounding Sphere
		col.Original_Pos = col.box.Center; // Sets the Original Position in local coordinates
		col.points = points;
	}
}

//Update the collision for the objects
void Update_Col(Collision &col, Model &model)
{
	glm::mat4 model_matrix;
	model_matrix = glm::rotate(model_matrix, model.angle, model.Vector_Rot);

	// Go through and change the orientation of the vertices of the object in its local coordinates (since that is all we need, we already know center)
	for (unsigned int k = 0; k < col.points.size() / 3; ++k)
	{
		glm::vec3 new_point = glm::vec3(model_matrix*glm::vec4(col.points[3 * k], col.points[3 * k + 1], col.points[3 * k + 2], 1));
		col.points[3 * k] = new_point.x;
		col.points[3 * k + 1] = new_point.y;
		col.points[3 * k + 2] = new_point.z;
	}
}

//Transform other object points in to the original entites world coordinates (this uses a inverse matrix, which is usually expensive)

std::vector<float> TransformInto_model_coords(std::vector<float> &points, glm::mat4 &M)
{
	std::vector<float> result_points;

	for (unsigned int k = 0; k < points.size() / 3; ++k)
	{
		glm::vec3 new_point = glm::vec3(M*glm::vec4(points[3 * k], points[3 * k + 1], points[3 * k + 2], 1));
		result_points.push_back(new_point.x);
		result_points.push_back(new_point.y);
		result_points.push_back(new_point.z);
	}

	return result_points;
}




//Set up Collision Data for the "world" (Entity_Manager)
void Setup_Collision_World(Entity_Manager &world)
{
	for (unsigned int k = 0; k < world.components.E_Col.Data.size(); ++k)
	{
		Setup_Collision(world.components.E_Model.Data[world.components.E_Col.Data[k].Model_ID], world.components.E_Col.Data[k]);
	}
}






// Check Collision between entities
//This is not done, since we need a result after collision effect (like death, push back by physics, ect..)
void Check_Col_World(Entity_Manager &world)
{
	// Set up the grid and put the entities in to their respective buckets This restarts every time this function is called
	Grid grid;
	grid.prime = 3;
	grid.Grid.reserve(world.components.E_Col.Data.size());
	for (unsigned int k = 0; k < world.components.E_Col.Data.size(); ++k)
	{
		//grid.Input_Into_Grid(world.components.E_Col.Data[k]);
		grid.Input_Into_Grid_Sphere(world.components.E_Col.Data[k]);
	}

	//Check Collision for each bucket
	for (unsigned int k = 0; k < grid.Grid.bucket_count(); ++k) // go through each bucket
	{
		if (grid.Grid.bucket_size(k) > 1) // check if the bucket has at least 2 elements
		{
			for (auto key1 = grid.Grid.begin(k); key1 != grid.Grid.end(k); ++key1) // iterate through the enetities in the bucket
			{
				for (auto key2 = grid.Grid.begin(k); key2 != grid.Grid.end(k); ++key2)
				{
					if (key1 != key2 && key1->second->Entity_ID != key2->second->Entity_ID && key1->second->State + key2->second->State != WALL + WALL) // Make sure we are not testing the same entity with it self
					{
						//Sphere
						if (key1->second->sphere.InterSphere(key2->second->sphere))
						{
							//Check the GJK algorthim
							//To do this, we need to transform one of the model global coordinates to the other local coordinates
							glm::mat4 inverse_M;
							glm::mat4 model_matrix;

							// Matrix for object for key2 from local coordinates to global coordinates
							model_matrix = glm::translate(model_matrix, world.components.E_Model.Data[key2->second->Model_ID].pos);
							model_matrix = glm::scale(model_matrix, glm::vec3(world.components.E_Model.Data[key2->second->Model_ID].scale[0], world.components.E_Model.Data[key2->second->Model_ID].scale[1], world.components.E_Model.Data[key2->second->Model_ID].scale[2]));
							model_matrix = glm::rotate(model_matrix, world.components.E_Model.Data[key2->second->Model_ID].angle, world.components.E_Model.Data[key2->second->Model_ID].Vector_Rot);

							//Inverse matrix from global coordinates to local model coordinates for object for key1
							inverse_M = glm::rotate(inverse_M, -world.components.E_Model.Data[key1->second->Model_ID].angle, world.components.E_Model.Data[key1->second->Model_ID].Vector_Rot);
							inverse_M = glm::scale(inverse_M, glm::vec3(1 / world.components.E_Model.Data[key1->second->Model_ID].scale[0], 1 / world.components.E_Model.Data[key1->second->Model_ID].scale[1], 1 / world.components.E_Model.Data[key1->second->Model_ID].scale[2]));
							inverse_M = glm::translate(inverse_M, -world.components.E_Model.Data[key1->second->Model_ID].pos);

							// Go from local coordinates key2 in to global coordinates and then in to local coordinates of key1
							model_matrix = inverse_M * model_matrix;
							std::vector<float> key2_newpoints = TransformInto_model_coords(key2->second->points, model_matrix);

							//std::cout << GJK_Col_3D(key1->second->points, key2_newpoints) << std::endl;
							if (GJK_Col_3D(key1->second->points, key2_newpoints))
							{
								//physics
							}
						}
					}
				}
			}
		}
	}



}




