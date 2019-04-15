

#include <glad/glad.h> // 
#include <GLFW/glfw3.h> // this is the window application and the way we can look for inputs through keys and mouse
#include "Shader.h"
#include "Entity_Manager.h"
#include "Textures2D.h"
#include "Zombies.h"
#include "ResourceManager.h"
#include "Animation.h"
#include "Viewer.h"
#include "State_Machine.h"
#include "Levels.h"
#include "SyncFade.h"
#include "Model.h"
#include "AnimationSystem.h"
#include "Blenders.h"

//Forward Deceleration


//Testing ground for testing animation or anything really



void Testing_Ground(State &state)
{
	//Shader for testing
	Shader Test;
	Test.CompileShader("/Users/Cody Tipton/Desktop/GIT/ZGame2/Shadervs.txt", "/Users/Cody Tipton/Desktop/GIT/ZGame2/Shaderfs.txt");
	Shader Nano;
	Nano.CompileShader("/Users/Cody Tipton/Desktop/GIT/ZGame2/Shaders/GeneralShadervs.txt", "/Users/Cody Tipton/Desktop/GIT/ZGame2/Shaders/GeneralShaderfs.txt");
	//Define the world
	Entity_Manager world;
	//Texture 
	Texture2D container_tex = ResourceManager::LoadTexture("/Users/Cody Tipton/Desktop/GIT/ZGame2/container.jpg", false, "Test");

	//Import Models and corresponding animations
	ResourceManager::LoadModel("/Users/Cody Tipton/Desktop/GIT/Models/Dummy/E05 player run animation.dae", "DUMMY", true, false);
	ResourceManager::LoadAnimation("/Users/Cody Tipton/Desktop/GIT/Models/Dummy/E05 player run animation.dae", LOOP, "run", "DUMMY");
	ResourceManager::LoadAnimation("/Users/Cody Tipton/Desktop/GIT/Models/Dummy/E05 player walk animation.dae", LOOP, "walk", "DUMMY");
	
	Full_Model Crysis("/Users/Cody Tipton/Desktop/GIT/Models/Crysis/nanosuit.obj",false,true);
	

	
	
	//Full_Model Dummy("/Users/Cody Tipton/Desktop/GIT/Models/Dummy/free3Dmodel.dae", true,false);

	//Spawn the DUMMY!
	IEntity* dummy=Spawner(world, "DUMMY", glm::vec3(-3.0, 4.0, 0.0), ANIMATION);
	IEntity* dummy2 = Spawner(world, "DUMMY", glm::vec3(9.0, 0.0, 0.0), ANIMATION);

	std::vector<IEntity*> dummies;
	for (unsigned int k = 0; k < 2; ++k)
	{
		dummies.push_back(Spawner(world, "DUMMY", glm::vec3(3.0 + k, 0.0, 0.0), ANIMATION));
	}
	
	
	
	


	
	
	onedim_Blend blender;
	blender.insert(world.access_model(dummy)->animators["walk"],0.0);
	blender.insert(world.access_model(dummy)->animators["run"], 1.0);
	blender.Start_Animation();
	
	SyncAnim syn;
	syn.Insert(ResourceManager::Models["DUMMY"].Animations["walk"]);
	syn.Insert(ResourceManager::Models["DUMMY"].Animations["run"]);
	syn.Start_Animation();


	

	

	while (!glfwWindowShouldClose(Viewer::Window) && state==TEST)
	{
		Viewer::Process_Input();

		std::vector<glm::mat4> Blend_Coords;

		float walk = 5 * Viewer::deltaTime;
		//Move a box around
		if (glfwGetKey(Viewer::Window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			
			//world.components.E_Model.Data[0].pos.y += walk;
		}
		if (glfwGetKey(Viewer::Window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			
	
			//world.components.E_Model.Data[0].pos.y -= walk;
		}
		if (glfwGetKey(Viewer::Window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			blender.Change_Blend_Ratio(.01);

			//world.components.E_Model.Data[0].pos.x += walk;
		}
		if (glfwGetKey(Viewer::Window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			blender.Change_Blend_Ratio(-.01);
			//world.components.E_Model.Data[0].pos.x -= walk;
			
		}
		if (glfwGetKey(Viewer::Window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			//angle_1 += 100 * Viewer::deltaTime;
		}
		if (glfwGetKey(Viewer::Window, GLFW_KEY_T) == GLFW_PRESS)
		{
			//angle_2 += 100 * Viewer::deltaTime;
		}
	
		
		
		
		
		
		SkeletonPose cody3 = syn.Animate(Viewer::deltaTime);
		SkeletonPose cody4 = blender.Animate(Viewer::deltaTime);
		
		
	
		Viewer::SetLighting(glm::vec3(0.0, 10.0, 20.0), Nano);
		world.Update_Animation(dummy, cody3);
		world.Update_Animation(dummy2, cody4);
		
		
		
		world.Update_Position(dummies[0], Viewer::camera.Position+3.0f*Viewer::camera.Front_vector());
		Crysis.Draw(Viewer::Projection,Viewer::View,glm::vec3(0.0,0.0,0.0),Nano);
		Viewer::SetLighting(glm::vec3(0.0, 10.0, 20.0), Nano);
		

		//Change_Position
		//Update everything
		//ChangeModel_Pos_World(world,deltaTime); // Change the Pos of objects

		Draw_World(world, Viewer::Projection, Viewer::View, Nano);
		//Check_Col_World(world);

		//Draw World



		Viewer::Swap_Buffers();
	}



}




