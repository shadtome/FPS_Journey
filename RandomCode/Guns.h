#ifndef GUNS_H
#define GUNS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "BulletVAO.h"
#include "Bullets.h"

#include <vector>


class Guns
{

public:
	// Muzzle velocity of the gun
	float Muzzle_Vel;

	//mass of the bullet it shoots
	float Mass_Bullet;

	//automatic(True) or semi (False)
	bool Automatic;

	//is this gun active?
	bool Active;

	//which clip is ready (I have three gun clips so that the bullets that are out are not interuppted while they are traveling
	std::vector<bool> clip = { true,false,false };

	//which clip is it
	int clip_number = 0;

	//number of bullets in the gun
	int Number_Bullets;

	//Timer, how fast the gun is allowed to shoot
	float Gun_Gap;

	//Timer for when the gun shot and the next time it will shoot
	float First_shot;
	float Last_shot;

	int Old_M_State = 0;

	//which bullet we are on
	int which_bullet;

	//Bullet list
	std::vector<Bullets> Bullet_list;

	//pointer to shader
	Shader* shaderPointer;
	//pointer to VAO

	BulletVAO* VAOPointer;



	Guns(Shader &shader, BulletVAO &vao,float muzzlevel, float mass, bool automatic,int numberbullets, float gungap) : which_bullet(0), First_shot( 0.0f), Last_shot(0.0f)
	{
		shaderPointer = &shader;
		VAOPointer = &vao;
		Gun_Gap = gungap;
		Muzzle_Vel = muzzlevel;
		Mass_Bullet = mass;
		Automatic = automatic;
		Number_Bullets = 3*numberbullets;
		

		for (int k = 0; k < Number_Bullets; ++k)
		{
			Bullet_list.push_back(Bullets(shader,vao));


		}
	}

	void shoot(GLFWwindow* window, float &deltaTime, glm::mat4 &projection, glm::mat4 &view, glm::vec3 &direction, glm::vec3 &initial_position)
	{
		
		if (Active) //checks if this gun is active
		{
			for (int k = 0; k < 3; ++k)
			{
				if (clip[k]) //Process through the clips to the true one
				{

					if (Automatic) // If the gun was set to autmatic use this activation
					{
						//Checks if the mouse button was clicked and the appropertate time gap was evaluated for the gun to shoot
						if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && glfwGetTime() - Last_shot > Gun_Gap)
						{

							Bullet_list[k*Number_Bullets/3+which_bullet].shootcallback(direction, initial_position); // Goes to the next bullet to shoot
							which_bullet = (which_bullet + 1) % (Number_Bullets/3);
							Last_shot = glfwGetTime();
						}
					}

					else
					{
						int New_M_State = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT); // This is used to make sure you cant hold down the mouse key to shoot
						if (New_M_State == GLFW_PRESS && Old_M_State == GLFW_RELEASE && glfwGetTime() - Last_shot > Gun_Gap)
						{
							Bullet_list[k*Number_Bullets/3+which_bullet].shootcallback(direction, initial_position);
							which_bullet = (which_bullet + 1) % (Number_Bullets/3);
							Last_shot = glfwGetTime();
						}
						Old_M_State = New_M_State;
					}
				}
			}
		}
		

		for (int k = 0; k < Number_Bullets; ++k) // This draws all the bullets, for this pparticular gun
		{
			Bullet_list[k].Draw(deltaTime, projection, view);
		}
	}

	// This is to reload and change the clip to the next one
	void Reload(GLFWwindow* window)
	{

		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		{
			for (int k = 0; k < 3; ++k)
			{
				if (k == clip_number && Bullet_list[k*Number_Bullets/3].ready_shoot==false)
				{
					//change which clip is in the gun
					clip[(k+1)%3] = true;
					clip[k % 3] = false;
					clip[(k + 2) % 3] = false;
					which_bullet = 0;

					//reload all the bullets for this clip ready to go
					for (int j = 0; j < Number_Bullets / 3; ++j)
					{
						Bullet_list[((k+1)%3)*Number_Bullets/3 + j].ready_shoot = true;
						Bullet_list[((k+1)%3)*Number_Bullets/3 + j].shooting = false;
					}
					clip_number = (k + 1) % 3;

				}
			}
		}
	}
	
};

























#endif