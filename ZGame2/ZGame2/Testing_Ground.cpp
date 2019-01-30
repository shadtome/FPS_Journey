

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

//Forward Deceleration


//Testing ground for testing animation or anything really



void Testing_Ground(State &state)
{
	//Shader for testing
	Shader Test;
	Test.CompileShader("/Users/Cody Tipton/Desktop/GIT/ZGame2/Shadervs.txt", "/Users/Cody Tipton/Desktop/GIT/ZGame2/Shaderfs.txt");
	//Define the world
	Entity_Manager world;
	//Texture 
	Texture2D container_tex = ResourceManager::LoadTexture("/Users/Cody Tipton/Desktop/GIT/ZGame2/container.jpg", false, "Test");

	// This is the testing with zombie boxes
	for (int k = 0; k < 7; ++k)
	{
		Zombie_Create(world, container_tex, glm::vec3((float)k * 4, 0.0f, 0.0f));

	}
	Zombie_Create(world, container_tex, glm::vec3(0.0, 0.0, 0.0));






	//Animation Testing
	//first lets define the joints
	Joint Root, L_Thigh, L_Leg, L_Foot, R_Thigh, R_Leg, R_Foot;
	Root.Parent_Index = -1;
	L_Thigh.Parent_Index = 0;
	L_Leg.Parent_Index = 1;
	L_Foot.Parent_Index = 2;
	R_Thigh.Parent_Index = 0;
	R_Leg.Parent_Index = 4;
	R_Foot.Parent_Index = 5;
	std::vector<Joint> Skelly_Joints = { Root,L_Thigh, L_Leg, L_Foot,R_Thigh, R_Leg, R_Foot };


	Skeleton skelly(Skelly_Joints);

	//Set up the Joint Poses
	JointPose Root_Pose(Quarternion(0.0, 0.0, 0.0, glm::radians(0.0)), glm::vec3(0.0, 0.0, 0.0), Root);
	JointPose L_Thigh_Pose(Quarternion(0.0, 0.0, 1.0, glm::radians(0.0)), glm::vec3(0.0, 0.0, 0.6), L_Thigh);
	JointPose L_Leg_Pose(Quarternion(0.0, 0.0, 1.0, glm::radians(-90.0)), glm::vec3(0.0, -1.0, 0.0), L_Leg);
	JointPose L_Foot_Pose(Quarternion(0.0, 0.0, 1.0, glm::radians(-60.0)), glm::vec3(0.0, -1.0, 0.0), L_Foot);
	JointPose R_Thigh_Pose(Quarternion(0.0, 0.0, 1.0, glm::radians(0.0)), glm::vec3(0.0, 0.0, -0.6), Root);
	JointPose R_Leg_Pose(Quarternion(0.0, 0.0, 1.0, glm::radians(90.0)), glm::vec3(0.0, -1.0, 0.0), R_Leg);
	JointPose R_Foot_Pose(Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0)), glm::vec3(0.0, -1.0, 0.0), R_Foot);
	SkeletonPose Pose_1(skelly, std::vector<JointPose>{Root_Pose, L_Thigh_Pose, L_Leg_Pose, L_Foot_Pose, R_Thigh_Pose, R_Leg_Pose, R_Foot_Pose});

	L_Thigh_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Leg_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Foot_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Thigh_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Leg_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Foot_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	SkeletonPose Pose_2(skelly, std::vector<JointPose>{Root_Pose, L_Thigh_Pose, L_Leg_Pose, L_Foot_Pose, R_Thigh_Pose, R_Leg_Pose, R_Foot_Pose});

	L_Thigh_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Leg_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(90.0));
	L_Foot_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	R_Thigh_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(.0));
	R_Leg_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-90.0));
	R_Foot_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-60.0));
	SkeletonPose Pose_3(skelly, std::vector<JointPose>{Root_Pose, L_Thigh_Pose, L_Leg_Pose, L_Foot_Pose, R_Thigh_Pose, R_Leg_Pose, R_Foot_Pose});

	L_Thigh_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Leg_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Foot_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Thigh_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Leg_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Foot_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	SkeletonPose Pose_4(skelly, std::vector<JointPose>{Root_Pose, L_Thigh_Pose, L_Leg_Pose, L_Foot_Pose, R_Thigh_Pose, R_Leg_Pose, R_Foot_Pose});

	L_Thigh_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(.0));
	L_Leg_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-90.0));
	L_Foot_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-60.0));
	R_Thigh_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Leg_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(90.0));
	R_Foot_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	SkeletonPose Pose_5(skelly, std::vector<JointPose>{Root_Pose, L_Thigh_Pose, L_Leg_Pose, L_Foot_Pose, R_Thigh_Pose, R_Leg_Pose, R_Foot_Pose});

	Animation Anim("Wave", skelly, { {0.0, Pose_1}, { 0.5,Pose_2 },{1.0, Pose_3},{1.5,Pose_4},{2.0,Pose_5} }, LOOP);
	Animator animator(skelly, Anim);
	bool animate_go = true;



	float angle_1 = 0.0;
	float angle_2 = 0.0;


	while (!glfwWindowShouldClose(Viewer::Window) && state==TEST)
	{
		Viewer::Process_Input();

		float walk = 5 * Viewer::deltaTime;
		//Move a box around
		if (glfwGetKey(Viewer::Window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			world.components.E_Model.Data[0].pos.y += walk;
		}
		if (glfwGetKey(Viewer::Window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			world.components.E_Model.Data[0].pos.y -= walk;
		}
		if (glfwGetKey(Viewer::Window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			world.components.E_Model.Data[0].pos.x += walk;
		}
		if (glfwGetKey(Viewer::Window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			world.components.E_Model.Data[0].pos.x -= walk;
		}
		if (glfwGetKey(Viewer::Window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			angle_1 += 100 * Viewer::deltaTime;
		}
		if (glfwGetKey(Viewer::Window, GLFW_KEY_T) == GLFW_PRESS)
		{
			angle_2 += 100 * Viewer::deltaTime;
		}

		if (animate_go)
		{
			animate_go = false;
			animator.Start_Animation(Viewer::CurrentFrame);
		}

		std::vector<glm::mat4> new_coords = animator.Animate(Viewer::CurrentFrame);

		for (int k = 1; k < 7; ++k)
		{
			world.components.E_Model.Data[k].pos = glm::vec3(new_coords[k] * glm::vec4(0.0, 0.0, 0.0, 1.0));
		}


		//Change_Position
		//Update everything
		//ChangeModel_Pos_World(world,deltaTime); // Change the Pos of objects

		Draw_World(world, Viewer::Projection, Viewer::View, Test);
		//Check_Col_World(world);

		//Draw World



		Viewer::Swap_Buffers();
	}



}



