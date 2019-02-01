#ifndef COLLISIONSYST_H
#define COLLISIONSYST_H


#include "Collision.h"
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Grid_Col.h"
#include "GJK.h"

//Forward Deceleration
class Entity_Manager;
class Model;
class Collision;

// --------------------------------------------
// This is the Entity System for Collision

void Setup_Collision(Model &model, Collision &col); // Set up the collision given model and collision data

void Update_Col(Collision &col, Model &model);   // Update the collision data based on orientation of the object

std::vector<float> TransformInto_model_coords(std::vector<float> &points, glm::mat4 &M);  // Transform in to model coords

void Setup_Collision_World(Entity_Manager &world);   // Set up the collision for all entities in world

void Check_Col_World(Entity_Manager &world);			// Check collision between objects in the world




#endif
