

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

	
	Full_Model Crysis("/Users/Cody Tipton/Desktop/GIT/Models/Crysis/nanosuit.obj",false,true);
	Full_Model Dummy2("/Users/Cody Tipton/Desktop/GIT/Models/Dummy/E05 player run animation.dae",true,false);
	Full_Model Dummy("/Users/Cody Tipton/Desktop/GIT/Models/Dummy/free3Dmodel.fbx", true,false);

	std::vector<JointPose> poses;

	for (unsigned int k = 0; k < Dummy.skeleton.JointCount; ++k)
	{
		glm::mat4 mat;
		JointPose temp(glm::inverse(Dummy.skeleton.Vector_Joints[k].M_invBindPose),Dummy.skeleton.Vector_Joints[k]);
		poses.push_back(temp);
		
	}
	SkeletonPose test(Dummy.skeleton, poses);
	
	std::vector<glm::mat4> matrices;
	test.Setup_Pose_Local();
	matrices = test.Global_Poses;
	
	/*for (unsigned int k = 0; k < matrices.size(); ++k)
	{
		for (unsigned int j = 0; j < 4; ++j)
		{
			std::cout << matrices[k][0][j] << "::" << matrices[k][1][j] << "::" << matrices[k][2][j] << "::" << matrices[k][3][j] << "::" << std::endl;
		}
	}*/
	
	
	
	// This is the testing with zombie boxes
	for (int k = 0; k < 7; ++k)
	{
		Zombie_Create(world, container_tex, glm::vec3((float)k * 4, 0.0f, 0.0f));

	}
	
	Zombie_Create(world, container_tex, glm::vec3(0.0, 10.0, 20.0));





	/*
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
	JointPose L_Leg_Pose(Quarternion(0.0, 0.0, 1.0, glm::radians(0.0)), glm::vec3(0.0, -1.5, 0.0), L_Leg);
	JointPose L_Foot_Pose(Quarternion(0.0, 0.0, 1.0, glm::radians(0.0)), glm::vec3(0.0, -1.2, 0.0), L_Foot);
	JointPose R_Thigh_Pose(Quarternion(0.0, 0.0, 1.0, glm::radians(0.0)), glm::vec3(0.0, 0.0, -0.6), Root);
	JointPose R_Leg_Pose(Quarternion(0.0, 0.0, 1.0, glm::radians(0.0)), glm::vec3(0.0, -1.5, 0.0), R_Leg);
	JointPose R_Foot_Pose(Quarternion(0.0, 0.0, 1.0, glm::radians(0.0)), glm::vec3(0.0, -1.2, 0.0), R_Foot);
	SkeletonPose Pose_1(skelly, std::vector<JointPose>{Root_Pose, L_Thigh_Pose, L_Leg_Pose, L_Foot_Pose, R_Thigh_Pose, R_Leg_Pose, R_Foot_Pose});

	L_Thigh_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Leg_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	L_Foot_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	R_Thigh_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Leg_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(10.0));
	R_Foot_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	SkeletonPose Pose_2(skelly, std::vector<JointPose>{Root_Pose, L_Thigh_Pose, L_Leg_Pose, L_Foot_Pose, R_Thigh_Pose, R_Leg_Pose, R_Foot_Pose});



	L_Thigh_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Leg_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-20.0));
	L_Foot_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	R_Thigh_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Leg_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(20.0));
	R_Foot_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	SkeletonPose Pose_3(skelly, std::vector<JointPose>{Root_Pose, L_Thigh_Pose, L_Leg_Pose, L_Foot_Pose, R_Thigh_Pose, R_Leg_Pose, R_Foot_Pose});

	L_Thigh_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Leg_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	L_Foot_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	R_Thigh_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Leg_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(10.0));
	R_Foot_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	SkeletonPose Pose_4(skelly, std::vector<JointPose>{Root_Pose, L_Thigh_Pose, L_Leg_Pose, L_Foot_Pose, R_Thigh_Pose, R_Leg_Pose, R_Foot_Pose});

	L_Thigh_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Leg_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Foot_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Thigh_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Leg_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Foot_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	SkeletonPose Pose_5(skelly, std::vector<JointPose>{Root_Pose, L_Thigh_Pose, L_Leg_Pose, L_Foot_Pose, R_Thigh_Pose, R_Leg_Pose, R_Foot_Pose});

	L_Thigh_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Leg_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(10.0));
	L_Foot_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	R_Thigh_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Leg_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	R_Foot_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	SkeletonPose Pose_6(skelly, std::vector<JointPose>{Root_Pose, L_Thigh_Pose, L_Leg_Pose, L_Foot_Pose, R_Thigh_Pose, R_Leg_Pose, R_Foot_Pose});

	L_Thigh_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Leg_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(20.0));
	L_Foot_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	R_Thigh_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(.0));
	R_Leg_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-20.0));
	R_Foot_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	SkeletonPose Pose_7(skelly, std::vector<JointPose>{Root_Pose, L_Thigh_Pose, L_Leg_Pose, L_Foot_Pose, R_Thigh_Pose, R_Leg_Pose, R_Foot_Pose});

	L_Thigh_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Leg_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(10.0));
	L_Foot_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	R_Thigh_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Leg_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	R_Foot_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	SkeletonPose Pose_8(skelly, std::vector<JointPose>{Root_Pose, L_Thigh_Pose, L_Leg_Pose, L_Foot_Pose, R_Thigh_Pose, R_Leg_Pose, R_Foot_Pose});

	L_Thigh_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Leg_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Foot_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Thigh_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Leg_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Foot_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	SkeletonPose Pose_9(skelly, std::vector<JointPose>{Root_Pose, L_Thigh_Pose, L_Leg_Pose, L_Foot_Pose, R_Thigh_Pose, R_Leg_Pose, R_Foot_Pose});



	Animation Anim("Walk", skelly, { {0.0, Pose_1}, { .3,Pose_2 },{0.6, Pose_3},{0.9,Pose_4},{1.2,Pose_5},{1.5, Pose_6 },{1.8, Pose_7},{2.1,Pose_8 },{2.4,Pose_9} }, LOOP);
	Animator animator(skelly, Anim);
	bool animate_go = true;

	//Idle Animation
	//Set up the Joint Poses
	JointPose Root_Pose_Idle(Quarternion(0.0, 0.0, 0.0, glm::radians(0.0)), glm::vec3(0.0, 0.0, 0.0), Root);
	JointPose L_Thigh_Pose_Idle(Quarternion(0.0, 0.0, 1.0, glm::radians(0.0)), glm::vec3(0.0, 0.0, 0.6), L_Thigh);
	JointPose L_Leg_Pose_Idle(Quarternion(0.0, 0.0, 1.0, glm::radians(0.0)), glm::vec3(0.0, -1.5, 0.0), L_Leg);
	JointPose L_Foot_Pose_Idle(Quarternion(0.0, 0.0, 1.0, glm::radians(0.0)), glm::vec3(0.0, -1.2, 0.0), L_Foot);
	JointPose R_Thigh_Pose_Idle(Quarternion(0.0, 0.0, 1.0, glm::radians(0.0)), glm::vec3(0.0, 0.0, -0.6), Root);
	JointPose R_Leg_Pose_Idle(Quarternion(0.0, 0.0, 1.0, glm::radians(0.0)), glm::vec3(0.0, -1.5, 0.0), R_Leg);
	JointPose R_Foot_Pose_Idle(Quarternion(0.0, 0.0, 1.0, glm::radians(0.0)), glm::vec3(0.0, -1.2, 0.0), R_Foot);
	SkeletonPose PoseIdle(skelly, std::vector<JointPose>{Root_Pose_Idle, L_Thigh_Pose_Idle, L_Leg_Pose_Idle, L_Foot_Pose_Idle, R_Thigh_Pose_Idle, R_Leg_Pose_Idle, R_Foot_Pose_Idle});

	Animation anim_IDLE("Idle", skelly, { {0.0, PoseIdle}, {1.0, PoseIdle} }, LOOP);
	Animator anim_Idle(skelly, anim_IDLE);

	//Running Animation
	JointPose Root_Pose_Run(Quarternion(0.0, 0.0, 0.0, glm::radians(0.0)), glm::vec3(0.0, 0.0, 0.0), Root);
	JointPose L_Thigh_Pose_Run(Quarternion(0.0, 0.0, 1.0, glm::radians(0.0)), glm::vec3(0.0, 0.0, 0.6), L_Thigh);
	JointPose L_Leg_Pose_Run(Quarternion(0.0, 0.0, 1.0, glm::radians(0.0)), glm::vec3(0.0, -1.5, 0.0), L_Leg);
	JointPose L_Foot_Pose_Run(Quarternion(0.0, 0.0, 1.0, glm::radians(0.0)), glm::vec3(0.0, -1.2, 0.0), L_Foot);
	JointPose R_Thigh_Pose_Run(Quarternion(0.0, 0.0, 1.0, glm::radians(0.0)), glm::vec3(0.0, 0.0, -0.6), Root);
	JointPose R_Leg_Pose_Run(Quarternion(0.0, 0.0, 1.0, glm::radians(0.0)), glm::vec3(0.0, -1.5, 0.0), R_Leg);
	JointPose R_Foot_Pose_Run(Quarternion(0.0, 0.0, 1.0, glm::radians(0.0)), glm::vec3(0.0, -1.2, 0.0), R_Foot);
	SkeletonPose Pose_1_Run(skelly, std::vector<JointPose>{Root_Pose_Run, L_Thigh_Pose_Run, L_Leg_Pose_Run, L_Foot_Pose_Run, R_Thigh_Pose_Run, R_Leg_Pose_Run, R_Foot_Pose_Run});
	
	L_Thigh_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Leg_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-30.0));
	L_Foot_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	R_Thigh_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Leg_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(30.0));
	R_Foot_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	SkeletonPose Pose_2_Run(skelly, std::vector<JointPose>{Root_Pose_Run, L_Thigh_Pose_Run, L_Leg_Pose_Run, L_Foot_Pose_Run, R_Thigh_Pose_Run, R_Leg_Pose_Run, R_Foot_Pose_Run});

	L_Thigh_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Leg_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-60.0));
	L_Foot_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	R_Thigh_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Leg_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(80.0));
	R_Foot_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-40.0));
	SkeletonPose Pose_3_Run(skelly, std::vector<JointPose>{Root_Pose_Run, L_Thigh_Pose_Run, L_Leg_Pose_Run, L_Foot_Pose_Run, R_Thigh_Pose_Run, R_Leg_Pose_Run, R_Foot_Pose_Run});

	L_Thigh_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Leg_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-30.0));
	L_Foot_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	R_Thigh_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Leg_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(30.0));
	R_Foot_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	SkeletonPose Pose_4_Run(skelly, std::vector<JointPose>{Root_Pose_Run, L_Thigh_Pose_Run, L_Leg_Pose_Run, L_Foot_Pose_Run, R_Thigh_Pose_Run, R_Leg_Pose_Run, R_Foot_Pose_Run});

	L_Thigh_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Leg_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Foot_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Thigh_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Leg_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Foot_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	SkeletonPose Pose_5_Run(skelly, std::vector<JointPose>{Root_Pose_Run, L_Thigh_Pose_Run, L_Leg_Pose_Run, L_Foot_Pose_Run, R_Thigh_Pose_Run, R_Leg_Pose_Run, R_Foot_Pose_Run});

	L_Thigh_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Leg_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(30.0));
	L_Foot_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	R_Thigh_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Leg_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-30.0));
	R_Foot_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	SkeletonPose Pose_6_Run(skelly, std::vector<JointPose>{Root_Pose_Run, L_Thigh_Pose_Run, L_Leg_Pose_Run, L_Foot_Pose_Run, R_Thigh_Pose_Run, R_Leg_Pose_Run, R_Foot_Pose_Run});

	L_Thigh_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Leg_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(80.0));
	L_Foot_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-40.0));
	R_Thigh_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(.0));
	R_Leg_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-60.0));
	R_Foot_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	SkeletonPose Pose_7_Run(skelly, std::vector<JointPose>{Root_Pose_Run, L_Thigh_Pose_Run, L_Leg_Pose_Run, L_Foot_Pose_Run, R_Thigh_Pose_Run, R_Leg_Pose_Run, R_Foot_Pose_Run});

	L_Thigh_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Leg_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(30.0));
	L_Foot_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	R_Thigh_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Leg_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-30.0));
	R_Foot_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	SkeletonPose Pose_8_Run(skelly, std::vector<JointPose>{Root_Pose_Run, L_Thigh_Pose_Run, L_Leg_Pose_Run, L_Foot_Pose_Run, R_Thigh_Pose_Run, R_Leg_Pose_Run, R_Foot_Pose_Run});

	L_Thigh_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Leg_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Foot_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Thigh_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Leg_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Foot_Pose_Run.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	SkeletonPose Pose_9_Run(skelly, std::vector<JointPose>{Root_Pose_Run, L_Thigh_Pose_Run, L_Leg_Pose_Run, L_Foot_Pose_Run, R_Thigh_Pose_Run, R_Leg_Pose_Run, R_Foot_Pose_Run});

	Animation Anim_Run("Run", skelly, { {0.0, Pose_1_Run}, { 0.05,Pose_2_Run },{0.1, Pose_3_Run},{0.15,Pose_4_Run},{0.2,Pose_5_Run},{0.25,Pose_6_Run},{0.3, Pose_7_Run},{0.35,Pose_8_Run},{0.4, Pose_9_Run} }, LOOP);
	Animator animator_Run(skelly, Anim_Run);


	JointPose Root_Pose_J(Quarternion(0.0, 0.0, 0.0, glm::radians(0.0)), glm::vec3(0.0, 0.0, 0.0), Root);
	JointPose L_Thigh_Pose_J(Quarternion(0.0, 0.0, 1.0, glm::radians(0.0)), glm::vec3(0.0, 0.0, 0.6), L_Thigh);
	JointPose L_Leg_Pose_J(Quarternion(0.0, 0.0, 1.0, glm::radians(0.0)), glm::vec3(0.0, -1.5, 0.0), L_Leg);
	JointPose L_Foot_Pose_J(Quarternion(0.0, 0.0, 1.0, glm::radians(0.0)), glm::vec3(0.0, -1.2, 0.0), L_Foot);
	JointPose R_Thigh_Pose_J(Quarternion(0.0, 0.0, 1.0, glm::radians(0.0)), glm::vec3(0.0, 0.0, -0.6), Root);
	JointPose R_Leg_Pose_J(Quarternion(0.0, 0.0, 1.0, glm::radians(0.0)), glm::vec3(0.0, -1.5, 0.0), R_Leg);
	JointPose R_Foot_Pose_J(Quarternion(0.0, 0.0, 1.0, glm::radians(0.0)), glm::vec3(0.0, -1.2, 0.0), R_Foot);
	SkeletonPose Pose_1_J(skelly, std::vector<JointPose>{Root_Pose_J, L_Thigh_Pose_J, L_Leg_Pose_J, L_Foot_Pose_J, R_Thigh_Pose_J, R_Leg_Pose_J, R_Foot_Pose_J});

	L_Thigh_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Leg_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-20.0));
	L_Foot_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	R_Thigh_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Leg_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(20.0));
	R_Foot_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	SkeletonPose Pose_2_J(skelly, std::vector<JointPose>{Root_Pose_J, L_Thigh_Pose_J, L_Leg_Pose_J, L_Foot_Pose_J, R_Thigh_Pose_J, R_Leg_Pose_J, R_Foot_Pose_J});

	L_Thigh_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Leg_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-40.0));
	L_Foot_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	R_Thigh_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Leg_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(40.0));
	R_Foot_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	SkeletonPose Pose_3_J(skelly, std::vector<JointPose>{Root_Pose_J, L_Thigh_Pose_J, L_Leg_Pose_J, L_Foot_Pose_J, R_Thigh_Pose_J, R_Leg_Pose_J, R_Foot_Pose_J});

	L_Thigh_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Leg_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-20.0));
	L_Foot_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	R_Thigh_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Leg_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(20.0));
	R_Foot_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	SkeletonPose Pose_4_J(skelly, std::vector<JointPose>{Root_Pose_J, L_Thigh_Pose_J, L_Leg_Pose_J, L_Foot_Pose_J, R_Thigh_Pose_J, R_Leg_Pose_J, R_Foot_Pose_J});

	L_Thigh_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Leg_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Foot_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Thigh_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Leg_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Foot_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	SkeletonPose Pose_5_J(skelly, std::vector<JointPose>{Root_Pose_J, L_Thigh_Pose_J, L_Leg_Pose_J, L_Foot_Pose_J, R_Thigh_Pose_J, R_Leg_Pose_J, R_Foot_Pose_J});

	L_Thigh_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Leg_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(20.0));
	L_Foot_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	R_Thigh_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Leg_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-20.0));
	R_Foot_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	SkeletonPose Pose_6_J(skelly, std::vector<JointPose>{Root_Pose_J, L_Thigh_Pose_J, L_Leg_Pose_J, L_Foot_Pose_J, R_Thigh_Pose_J, R_Leg_Pose_J, R_Foot_Pose_J});

	L_Thigh_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Leg_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(40.0));
	L_Foot_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	R_Thigh_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(.0));
	R_Leg_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-40.0));
	R_Foot_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	SkeletonPose Pose_7_J(skelly, std::vector<JointPose>{Root_Pose_J, L_Thigh_Pose_J, L_Leg_Pose_J, L_Foot_Pose_J, R_Thigh_Pose_J, R_Leg_Pose_J, R_Foot_Pose_J});

	L_Thigh_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Leg_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(20.0));
	L_Foot_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	R_Thigh_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Leg_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-20.0));
	R_Foot_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(-10.0));
	SkeletonPose Pose_8_J(skelly, std::vector<JointPose>{Root_Pose_J, L_Thigh_Pose_J, L_Leg_Pose_J, L_Foot_Pose_J, R_Thigh_Pose_J, R_Leg_Pose_J, R_Foot_Pose_J});

	L_Thigh_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Leg_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Foot_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Thigh_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Leg_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Foot_Pose_J.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	SkeletonPose Pose_9_J(skelly, std::vector<JointPose>{Root_Pose_J, L_Thigh_Pose_J, L_Leg_Pose_J, L_Foot_Pose_J, R_Thigh_Pose_J, R_Leg_Pose_J, R_Foot_Pose_J});

	Animation Anim_Jog("Jog", skelly, { {0.0, Pose_1_J}, { 0.1,Pose_2_J },{0.2, Pose_3_J},{0.3,Pose_4_J},{0.4,Pose_5_J},{0.5,Pose_6_J},{0.6, Pose_7_J},{0.7,Pose_8_J},{0.8, Pose_9_J} }, LOOP);
	Animator animator_Jog(skelly, Anim_Jog);
	

	//Animation Sync
	SyncAnim Jog;
	Jog.Insert(Anim);
	Jog.Insert(Anim_Jog);
	Jog.Insert(Anim_Run);
	Jog.Start_Animation();
	//walk_Idle.Ordered_List[0].Start_Animation();


	float angle_1 = 0.0;
	float angle_2 = 0.0;
	*/

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
	
		/*
		if (animate_go)
		{
			animate_go = false;
			//animator.Start_Animation(Viewer::CurrentFrame);
			//anim_Idle.Start_Animation();
		}

		//std::vector<glm::mat4> new_coords = animator.Animate(Viewer::deltaTime);
		//std::vector<glm::mat4> new_coords_idle = anim_Idle.Animate(Viewer::deltaTime);
		Blend_Coords = Jog.Animate(Viewer::deltaTime);
		

		for (int k = 1; k < Blend_Coords.size(); ++k)
		{
			world.components.E_Model.Data[k].pos = glm::vec3(Blend_Coords[k] * glm::vec4(0.0, 0.0, 0.0, 1.0));
		}
		*/

		
		Viewer::SetLighting(glm::vec3(0.0, 10.0, 20.0), Nano);
		Crysis.Draw(Viewer::Projection,Viewer::View,glm::vec3(0.0,0.0,0.0),Nano);
		Viewer::SetLighting(glm::vec3(0.0, 10.0, 20.0), Nano);
		Dummy.Draw(Viewer::Projection, Viewer::View, glm::vec3(0.0, 0.0, 0.0), Nano,matrices);
		Dummy2.Draw(Viewer::Projection, Viewer::View, glm::vec3(0.0, 0.0, 0.0), Nano, matrices);

		//Change_Position
		//Update everything
		//ChangeModel_Pos_World(world,deltaTime); // Change the Pos of objects

		Draw_World(world, Viewer::Projection, Viewer::View, Test);
		//Check_Col_World(world);

		//Draw World



		Viewer::Swap_Buffers();
	}



}




