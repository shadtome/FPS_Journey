

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
	ResourceManager::LoadModel("/Users/Cody Tipton/Desktop/GIT/Models/Dummy/E05 player run animation.fbx", "DUMMYfbx", true, false);
	ResourceManager::LoadModel("/Users/Cody Tipton/Desktop/GIT/Models/Dummy/E05 player run animation.dae", "DUMMYdae", true, false);
	
	//ResourceManager::LoadAnimation("/Users/Cody Tipton/Desktop/GIT/Models/Dummy/E05 player run animation.dae", LOOP, "run", "DUMMY");
	ResourceManager::LoadAnimation("/Users/Cody Tipton/Desktop/GIT/Models/Dummy/E05 player walk animation.dae", LOOP, "walk", "DUMMYdae");
	//ResourceManager::LoadAnimation("/Users/Cody Tipton/Desktop/GIT/Models/Dummy/Left Up.dae", LOOP, "up right", "DUMMY");
	//ResourceManager::LoadAnimation("/Users/Cody Tipton/Desktop/GIT/Models/Dummy/Right Up.dae", LOOP, "up left", "DUMMY");
	//ResourceManager::LoadAnimation("/Users/Cody Tipton/Desktop/GIT/Models/Dummy/Down Left.dae", LOOP, "down left", "DUMMY");
	//ResourceManager::LoadAnimation("/Users/Cody Tipton/Desktop/GIT/Models/Dummy/Down Right.dae", LOOP, "down right", "DUMMY");
	//ResourceManager::LoadAnimation("/Users/Cody Tipton/Desktop/GIT/Models/Dummy/headleft.dae", LOOP, "head left", "DUMMY");
	
	ResourceManager::LoadAnimation("/Users/Cody Tipton/Desktop/GIT/Models/Dummy/E05 player run animation.fbx", LOOP, "running", "DUMMYfbx");
	ResourceManager::LoadAnimation("/Users/Cody Tipton/Desktop/GIT/Models/Dummy/E05 player run animation.dae", LOOP, "running", "DUMMYdae");
	
	
	Full_Model Crysis("/Users/Cody Tipton/Desktop/GIT/Models/Crysis/nanosuit.obj",false,true);
	

	
	
	//Full_Model Dummy("/Users/Cody Tipton/Desktop/GIT/Models/Dummy/free3Dmodel.dae", true,false);

	//Spawn the DUMMY!
	IEntity* dummy=Spawner(world, "DUMMYfbx", glm::vec3(2.0, 0.0, 0.0), ANIMATION);
	IEntity* dummy2 = Spawner(world, "DUMMYdae", glm::vec3(-4.0, 0.0, 0.0), ANIMATION);

	std::vector<IEntity*> dummies;
	int num_dum = 10;
	for (unsigned int k = 0; k < num_dum; ++k)
	{
		dummies.push_back(Spawner(world, "DUMMYfbx", glm::vec3(cos(glm::radians((float)k)), 0.0, 0.0), ANIMATION));
	}

	/*std::vector<IEntity*> rundum;
	for (unsigned int k = 0; k < ResourceManager::GetModel("DUMMY").Animations["run"].KeyFrames.size(); ++k)
	{
		rundum.push_back(Spawner(world, "DUMMY", glm::vec3(3.0 + k, 2.0, 0.0), ANIMATION));
	}*/
	
	
	
	
	
	Animator running = world.access_model(dummy)->animators["running1"];
	std::cout << ResourceManager::GetModel("DUMMYfbx").Animations.begin()->second.KeyFrames.size() << std::endl;
	std::cout << ResourceManager::GetModel("DUMMYfbx").Animations.size() << std::endl;
	running.Start_Animation();

	std::cout <<"End time"<<running.End_Time << std::endl;


	std::cout << ResourceManager::GetModel("DUMMYfbx").Animations.begin()->first << std::endl;
	Animation what =ResourceManager::GetModel("DUMMYfbx").Animations.begin()->second;
	
	for (auto k = ResourceManager::GetModel("DUMMYfbx").Animations.begin(); k!=ResourceManager::GetModel("DUMMYfbx").Animations.end(); ++k)
	{
		std::cout << k->first << std::endl;
		for (auto j = k->second.KeyFrames.begin(); j != k->second.KeyFrames.end(); ++j)
		{
			std::cout << j->first << std::endl;
		}
	}

	float y_axis = 0;
	float delta=0;
	world.access_model(dummy->ID)->Vector_Rot = glm::vec3(1.0, 0.0, 0.0);
		
	/*onedim_Blend blender;
	blender.insert(world.access_model(dummy)->animators["walk"],0.0);
	blender.insert(world.access_model(dummy)->animators["run"], 1.0);
	blender.Start_Animation();
	/*
	SyncAnim syn;
	syn.Insert(ResourceManager::Models["DUMMY"].Animations["walk"]);
	syn.Insert(ResourceManager::Models["DUMMY"].Animations["run"]);
	syn.Start_Animation();
	*/

	/*//Testing square blend
	Square_twodim_Blend testing;
	testing.Top_insert(world.access_model(dummy)->animators["up left"], 0.0);
	testing.Top_insert(world.access_model(dummy)->animators["up right"], 1.0);
	testing.Bot_insert(world.access_model(dummy)->animators["down left"], 0.0);
	testing.Bot_insert(world.access_model(dummy)->animators["down right"], 1.0);
	testing.Start_Animation();
	testing.Set_Blend_Ratio(0.5, 0.5);
	*/

	//Testing Additive Blending
	//Difference_Animator AB(world.access_model(dummy)->animators["head left"], world.access_model(dummy)->animators["run"]);
	//AB.start(1.0);

	while (!glfwWindowShouldClose(Viewer::Window) && state==TEST)
	{
		Viewer::Process_Input();

		std::vector<glm::mat4> Blend_Coords;

		float walk = 5 * Viewer::deltaTime;
		//Move a box around
		if (glfwGetKey(Viewer::Window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			
			world.access_model(dummy->ID)->angle += walk;
			//testing.Change_Blend_Ratio(0.0, -0.1);
			//world.components.E_Model.Data[0].pos.y += walk;
		}
		if (glfwGetKey(Viewer::Window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			//testing.Change_Blend_Ratio(0.0, 0.1);
			world.access_model(dummy->ID)->angle -= walk;
			//world.components.E_Model.Data[0].pos.y -= walk;
		}
		if (glfwGetKey(Viewer::Window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			y_axis -= walk;
			//testing.Change_Blend_Ratio(0.1, 0.0);
			//blender.Change_Blend_Ratio(.01);
			//syn.Change_Blend_Ratio(.01);
			//world.components.E_Model.Data[0].pos.x += walk;
			
		}
		if (glfwGetKey(Viewer::Window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			y_axis += walk;
			//testing.Change_Blend_Ratio(-0.1, 0.0);
			//blender.Change_Blend_Ratio(-.01);
			//syn.Change_Blend_Ratio(-0.01);
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
	
		/*
		for (unsigned int k = 0; k < ResourceManager::GetModel("DUMMY").Animations["run"].KeyFrames.size(); ++k)
		{
			world.Update_Animation(rundum[k], ResourceManager::GetModel("DUMMY").Animations["run"].KeyFrames[k].second);
		}
		*/
		
		
		




		//SkeletonPose cody3 = syn.Animate(Viewer::deltaTime);
		//SkeletonPose cody4 = blender.Animate(Viewer::deltaTime);
		//SkeletonPose aim = testing.Animate(Viewer::deltaTime);
		SkeletonPose cody5 = running.Animate(Viewer::deltaTime);






		






		delta += 0.01;
		
		Viewer::SetLighting(glm::vec3(0.0, 10.0, 20.0), Nano);
		world.Update_Animation(dummy, cody5);
		//world.Update_Animation(dummy2, AB.Add_Pose(cody5, Viewer::deltaTime));
		//world.Update_Animation(dummies[0], aim);

		
		world.Update_Position(dummy2, glm::vec3(-4.0+delta, 0.0, 0.0));
		world.Update_Position(dummy->ID, glm::vec3(0.0, y_axis, 0.0));
		
		for (unsigned int j = 0; j < num_dum; ++j)
		{
			world.Update_Position(dummies[j]->ID, glm::vec3(0.0, y_axis, 0.0));
			world.Update_Animation(dummies[j]->ID, cody5);
		}

		world.Update_Position(dummies[0], glm::vec3(5.0,0.0,0.0));
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




