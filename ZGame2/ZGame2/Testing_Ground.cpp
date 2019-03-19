

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

	//Full_Model arm("/Users/Cody Tipton/Desktop/GIT/Models/Coordinate/testing.dae", true, false);
	Full_Model x_axis("/Users/Cody Tipton/Desktop/Git/Models/Coordinate/x.dae", false, true);
	Full_Model y_axis("/Users/Cody Tipton/Desktop/Git/Models/Coordinate/y.dae", false, true);
	Full_Model z_axis("/Users/Cody Tipton/Desktop/Git/Models/Coordinate/z.dae", false, true);
	Full_Model Crysis("/Users/Cody Tipton/Desktop/GIT/Models/Crysis/nanosuit.obj",false,true);
	Full_Model Dummy2("/Users/Cody Tipton/Desktop/GIT/Models/Dummy/E05 player run animation.dae",true,false);
	//Full_Model Dummy("/Users/Cody Tipton/Desktop/GIT/Models/Dummy/free3Dmodel.dae", true,false);


	Dummy2.Animations.begin()->second.Type = LOOP;
	
	
	Animator testing(Dummy2.skeleton, Dummy2.Animations.begin()->second);
	Animation hmmm= Dummy2.Animations.begin()->second;
	testing.Start_Animation();

	std::vector<JointPose> poses;

	/*for (unsigned int k = 0; k < Dummy.skeleton.JointCount; ++k)
	{
		glm::mat4 mat;
		//JointPose temp(glm::inverse(Dummy.skeleton.Vector_Joints[k].M_invBindPose),Dummy.skeleton.Vector_Joints[k]);
		JointPose temp(mat, Dummy.skeleton.Vector_Joints[k]);
		poses.push_back(temp);

	}
	SkeletonPose test(Dummy.skeleton, poses);

	std::vector<glm::mat4> matrices;

	test.Setup_Pose_Local();

	matrices = test.Global_Poses;
	*/

	std::vector<JointPose> poses2;

	for (unsigned int k = 0; k < Dummy2.skeleton.JointCount; ++k)
	{
		glm::mat4 mat;
		JointPose temp(glm::inverse(Dummy2.skeleton.Vector_Joints[k].M_invBindPose),Dummy2.skeleton.Vector_Joints[k]);
		//JointPose temp(mat, Dummy2.skeleton.Vector_Joints[k]);
		poses2.push_back(temp);
		
	}
	SkeletonPose test2(Dummy2.skeleton, poses2);
	
	std::vector<glm::mat4> matrices2;
	
	test2.Setup_Pose_Local();
	
	matrices2 = test2.Global_Poses;
	
	
	
	
	
	// This is the testing with zombie boxes
	for (int k = 1; k < 7; ++k)
	{
		Zombie_Create(world, container_tex, glm::vec3((float)k * 4, 0.0f, 0.0f));

	}
	
	Zombie_Create(world, container_tex, glm::vec3(0.0, 10.0, 20.0));





	

	testing.Start_Animation();

	while (!glfwWindowShouldClose(Viewer::Window) && state==TEST)
	{
		Viewer::Process_Input();

		std::vector<glm::mat4> Blend_Coords;

		float walk = 5 * Viewer::deltaTime;
		//Move a box around
		if (glfwGetKey(Viewer::Window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			//Jog.Change_Blend_Ratio(.01);
			world.components.E_Model.Data[0].pos.y += walk;
		}
		if (glfwGetKey(Viewer::Window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			//Jog.Change_Blend_Ratio(-0.01);
	
			world.components.E_Model.Data[0].pos.y -= walk;
		}
		if (glfwGetKey(Viewer::Window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{


			world.components.E_Model.Data[0].pos.x += walk;
		}
		if (glfwGetKey(Viewer::Window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			world.components.E_Model.Data[0].pos.x -= walk;
			//Blend_Coords = walk_Idle.Animate_State(walk);
		}
		if (glfwGetKey(Viewer::Window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			//angle_1 += 100 * Viewer::deltaTime;
		}
		if (glfwGetKey(Viewer::Window, GLFW_KEY_T) == GLFW_PRESS)
		{
			//angle_2 += 100 * Viewer::deltaTime;
		}
	
		
		
		

		std::vector<glm::mat4> cody = testing.Animate(Viewer::deltaTime);

		/*for (unsigned int k = 0; k < cody.size(); ++k)
		{
			for (unsigned int j = 0; j < 4; ++j)
			{
				std::cout << cody[k][0][j] << "::" << cody[k][1][j] << "::" << cody[k][2][j] << "::" << cody[k][3][j] << "::" << std::endl;
			}
		}*/
		

		
		Viewer::SetLighting(glm::vec3(0.0, 10.0, 20.0), Nano);
		//x_axis.Draw(Viewer::Projection, Viewer::View, glm::vec3(0.0, 0.0, 0.0), Nano);
		//y_axis.Draw(Viewer::Projection, Viewer::View, glm::vec3(0.0, 0.0, 0.0), Nano);
		//z_axis.Draw(Viewer::Projection, Viewer::View, glm::vec3(0.0, 0.0, 0.0), Nano);
		Crysis.Draw(Viewer::Projection,Viewer::View,glm::vec3(0.0,0.0,0.0),Nano);
		Viewer::SetLighting(glm::vec3(0.0, 10.0, 20.0), Nano);
		Dummy2.Draw(Viewer::Projection, Viewer::View, glm::vec3(0.0, 0.0, 0.0), Nano,cody);
		//Dummy.Draw(Viewer::Projection, Viewer::View, glm::vec3(0.0, 0.0, 0.0), Nano, matrices);

		//Change_Position
		//Update everything
		//ChangeModel_Pos_World(world,deltaTime); // Change the Pos of objects

		Draw_World(world, Viewer::Projection, Viewer::View, Test);
		//Check_Col_World(world);

		//Draw World



		Viewer::Swap_Buffers();
	}



}




