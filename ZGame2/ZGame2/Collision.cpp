#include "Collision.h"


//Collision source file

//AABB Class Functions
//--------------------------------------------------------------------

//Class AABB functions
//-------------------------------------------------------------
void AABB::CompileAABB(glm::vec3 center, float* radius)
{
	Center = center;
	for (int k = 0; k < 3; ++k)
	{
		Radius[k] = radius[k];
	}
}

int AABB::InterTestAABB(AABB box)
{
	if (abs(Center.x - box.Center.x) > (Radius[0] + box.Radius[0]))
	{
		//std::cout << abs(Center.x - box.Center.x) << " > " << (Radius[0]/2 + box.Radius[0]/2) << std::endl;
		//std::cout << abs(Center.x - box.Center.x)<< " > " << (Radius[0] + box.Radius[0] ) << std::endl;
		//std::cout << Center.x << "-" << box.Center.x << " >" << 1 << std::endl;
		return 0;
	}
	if (abs(Center.y - box.Center.y) > (Radius[1] + box.Radius[1]))
		return 0;
	if (abs(Center.z - box.Center.z) > (Radius[2] + box.Radius[2]))
		return 0;

	return 1;

};

void AABB::ConstructAABB_Points(std::vector<float> points)
{


	//Indices for the different direction for the max and min distance away from the origin
	int imin_x, imax_x, imin_y, imax_y, imin_z, imax_z;

	// positive x direction
	ExtremePointesAlongDirection({ 1,0,0 }, points, points.size(), &imin_x, &imax_x); //  find the points that are max and min distance away from the origin in the x-direction
	//Set the Radius in the x direction 
	Radius[0] = abs(points[3 * imin_x] - points[3 * imax_x]) / 2;
	//Find the x coordinate of the center
	Center.x = points[3 * imax_x] - Radius[0];

	// positive y-direction
	ExtremePointesAlongDirection({ 0,1,0 }, points, points.size(), &imin_y, &imax_y); // Find the points that are max and min in the y -direction
	//set the Radius in the y -direction
	Radius[1] = abs(points[3 * imin_y + 1] - points[3 * imax_y + 1]) / 2;
	//Find the y-coordinate of the center
	Center.y = points[3 * imax_y + 1] - Radius[1];

	// positive z-direction
	ExtremePointesAlongDirection({ 0,0,1 }, points, points.size(), &imin_z, &imax_z); // Find the points that are max and min in the z-direction
	// Set the Radius in the z-direction
	Radius[2] = abs(points[3 * imin_z + 2] - points[3 * imax_z + 2]) / 2;
	//Find the z-coordinate of the center
	Center.z = points[3 * imax_z + 2] - Radius[2];


	//std::cout << Center.x << " " << Center.y << " " << Center.z << std::endl;
	//std::cout << Radius[0]<< Radius[1]<< Radius[2] << std::endl;

};

void AABB::UpdateAABB(glm::vec3 center)
{
	Center = center;
}


//------------------------------------------------------------
//End of AABB functions


//Beginning of ColSphere Class functions
//------------------------------------------------------------

int ColSphere::InterSphere(ColSphere sphere)
{
	if (pow(Center.x - sphere.Center.x, 2) + pow(Center.y - sphere.Center.y, 2) + pow(Center.z - sphere.Center.z, 2) > pow(Radius + sphere.Radius, 2))
	{
		return 0;
	}

	return 1;
};

void ColSphere::CompileSphere(AABB box)
{
	Center = box.Center;


	Radius = sqrt(pow(box.Radius[0], 2) + pow(box.Radius[1], 2) + pow(box.Radius[2], 2));
};

void ColSphere::Sphere_Points(std::vector<float> points)
{
	AABB box;
	box.ConstructAABB_Points(points);
	CompileSphere(box);
};

void ColSphere::UpdateSphere(glm::vec3 center, float radius)
{
	Center = center;
	Radius = radius;
};


// -----------------------------------------------------------------
// End of Collision sphere class



//--------------------------------------
//More expensive testing by looking at the object more specifically (everything is convex for this game)



//This uses the Hyperplane seperation theorem from Functional analysis and is not very good in this state, it need to optimize it a lot 
//The second part with the cross products make it very slow.
// Primitive testing using the fact that convex objects are not intersecting if and only if their is a seperating hyperplane
bool Sep_Axis_Test(std::vector<float> &object1, glm::vec3 &center_local1, glm::vec3 &center1, std::vector<float> &object2, glm::vec3 &center_local2, glm::vec3 &center2)
{
	//This is for testing
	float r_1;
	float r_2;
	float dist;

	std::vector<float>* objects[2] = { &object1, &object2 };
	glm::vec3 vector1; // vectors for object one in the maximal direction
	glm::vec3 vector2; // vector for object two in the maximal direction
	glm::vec3 new_sep_axis_vec;
	glm::vec3 sep_axis_vec = { 1,0,0 };  //Vector in the direction of the seperating axis

	//Go through the normal of the faces of object1 and test;
	for (int j = 0; j < 2; ++j)
	{


		for (unsigned int k = 1; k < objects[j]->size() / 3 - 3; ++k)
		{
			//std::cout << objects[j]->operator[](3 * (k - 1)) << std::endl;
			vector1 = { -objects[j]->operator[](3 * (k - 1)) + objects[j]->operator[](3 * (k)),-objects[j]->operator[](3 * (k - 1) + 1) + objects[j]->operator[](3 * (k)+1),-objects[j]->operator[](3 * (k - 1) + 2) + objects[j]->operator[](3 * (k)+2) };
			vector2 = { -objects[j]->operator[](3 * (k - 1)) + objects[j]->operator[](3 * (k + 1)),-objects[j]->operator[](3 * (k - 1) + 1) + objects[j]->operator[](3 * (k + 1) + 1),-objects[j]->operator[](3 * (k - 1) + 2) + objects[j]->operator[](3 * (k + 1) + 2) };
			//Crossproduct function of vector1 and vector2
			new_sep_axis_vec = Cross_prod(vector1, vector2);
			if (Cross_prod(sep_axis_vec, new_sep_axis_vec) != glm::vec3{ 0, 0, 0 })
			{
				sep_axis_vec = new_sep_axis_vec;
				// norm of this vector
				float norm = sqrt(Dot(sep_axis_vec, sep_axis_vec));
				// index max and min
				int imin1;
				int imax1;// Min and Max for object 1
				int imin2;
				int imax2;// Min and Max for object 2
				// Find Extremeal points in the sep_axis_vec direction for both objects 
				ExtremePointesAlongDirection(sep_axis_vec, object1, object1.size(), &imin1, &imax1);
				ExtremePointesAlongDirection(sep_axis_vec, object2, object2.size(), &imin2, &imax2);

				//Find the vector from the center of the object and project on to the vector sep_axis_vec and determine the length use the comp
				vector1 = glm::vec3{ -center_local1[0] + object1[3 * imax1],-center_local1[1] + object1[3 * imax1 + 1],-center_local1[2] + object1[3 * imax1 + 2] };

				vector2 = glm::vec3{ -center_local2[0] + object2[3 * imax2],-center_local2[1] + object2[3 * imax2 + 1],-center_local2[2] + object2[3 * imax2 + 2] };

				float r1; // projection of the length of vector1 in the sep_axis_vec direction
				float r2; // ditto for vector2

				// comp of the projections
				r1 = abs(Dot(sep_axis_vec, vector1) / norm);
				r2 = abs(Dot(sep_axis_vec, vector2) / norm);


				// if the objects intersect or not (using r1+r2 < distance between the projected centers on to the line L
				// return 1 if intersection

				// Need to calculate the distance between the projection of the centers of each object



				float dist_centers = abs(Dot(center1, sep_axis_vec) - Dot(center2, sep_axis_vec)) / norm; // Distance between the projected distances
				if (r1 + r2 < dist_centers)
				{
					//std::cout << r1 << "::" << r2 << "::" << dist_centers << std::endl;
					return 0;
				}
				r_1 = r1;
				r_2 = r2;
				dist = dist_centers;
			}
		}
	}

	// Check the cross product between the edges of each object (for later, make it robust to handle the 0 vector reesulting from a cross product)
	/*for (int k = 1; k < objects[0]->size()/3-3; ++k)
	{
		for (int j = 1; j < objects[1]->size()/3-3; ++j)
		{
			// normal vector to a face of object 1
			glm::vec3 vector_Edge1 = { -objects[0]->operator[](3 * (k - 1)) + objects[0]->operator[](3 * (k)),-objects[0]->operator[](3 * (k - 1) + 1) + objects[0]->operator[](3 * (k)+1),-objects[0]->operator[](3 * (k - 1) + 2) + objects[0]->operator[](3 * (k)+2) };


			// normal vector to a face of object 2
			glm::vec3 vector_Edge2 = { -objects[1]->operator[](3 * (j - 1)) + objects[1]->operator[](3 * (j)),-objects[1]->operator[](3 * (j - 1) + 1) + objects[1]->operator[](3 * (j)+1),-objects[1]->operator[](3 * (j - 1) + 2) + objects[1]->operator[](3 * (j)+2) };


			// Cross product between the normal faces of each object
			sep_axis_vec = Cross_prod(vector_Edge1, vector_Edge2);

			if (sep_axis_vec != glm::vec3{ 0,0,0 })
			{
				float norm = sqrt(Dot(sep_axis_vec, sep_axis_vec));
				// index max and min
				int imin1;
				int imax1;// Min and Max for object 1
				int imin2;
				int imax2;// Min and Max for object 2
				// Find Extremeal points in the sep_axis_vec direction for both objects
				ExtremePointesAlongDirection(sep_axis_vec, object1, object1.size(), &imin1, &imax1);
				ExtremePointesAlongDirection(sep_axis_vec, object2, object2.size(), &imin2, &imax2);

				//Find the vector from the center of the object and project on to the vector sep_axis_vec and determine the length use the comp
				vector1 = glm::vec3{ -center_local1[0] + object1[3 * imax1],-center_local1[1] + object1[3 * imax1 + 1],-center_local1[2] + object1[3 * imax1 + 2] };

				vector2 = glm::vec3{ -center_local2[0] + object2[3 * imax2],-center_local2[1] + object2[3 * imax2 + 1],-center_local2[2] + object2[3 * imax2 + 2] };

				float r1; // projection of the length of vector1 in the sep_axis_vec direction
				float r2; // ditto for vector2

				// comp of the projections
				r1 = abs(Dot(sep_axis_vec, vector1)) / norm;
				r2 = abs(Dot(sep_axis_vec, vector2))/ norm;



				// if the objects intersect or not (using r1+r2 < distance between the projected centers on to the line L
				// return 1 if intersection

				// Need to calculate the distance between the projection of the centers of each object



				float dist_centers = abs(Dot(center1,sep_axis_vec)-Dot(center2,sep_axis_vec))/norm ; // Distance between the projected distances
				//std::cout << r1 << "::" << r2 <<"::"<< dist_centers << std::endl; // Check the mathematics (specifical the cross product of the normals)
				if (r1 + r2 < dist_centers)
				{
					//std::cout << r1 << "::" << r2 << "::" << dist_centers << std::endl;
					return 0;
				}
				//Check later on why it is returning this, even so the collision is not happening
				r_1 = r1;
				r_2 = r2;
				dist = dist_centers;

			}
		}
	}*/

	//std::cout << r_1 << "::" << r_2 << "::" << dist << std::endl;
	return 1;
}