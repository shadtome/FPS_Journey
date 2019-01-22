#include <glad/glad.h> // 
#include <GLFW/glfw3.h> // this is the window application and the way we can look for inputs through keys and mouse
#include <iostream>
#include <cmath>
#include "Shader.h" 
#include "stb_image.h" 
#include <glm/glm.hpp> //the GLM math header files (all the awesome math goodness!!)
#include <glm/gtc/matrix_transform.hpp>  //More math stuff from glm
#include <glm/gtc/type_ptr.hpp>  //this is to change the way matrices look so that is can be implemted in to OpenGL
#include "Camera.h" 
#include "Collision.h"

#include "ResourceManager.h"
#include "Textures2D.h"
#include <map>
#include "Entity_Manager.h"
#include "Graphics.h"
#include <time.h>
#include "Zombies.h"
#include "Quaternion.h"
#include "Skeleton.h"
#include "Animation.h"




//predefined functions before they are offically defined
void framebuffer_size_callback(GLFWwindow*window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// This is the main camera we use.  (See the Camera class for more information)
Camera ourCamera;

//setting for the size of the window (you can change these to what you like and the everything will change according to it)
const unsigned int width = 800;
const unsigned int height = 600;

// used for the mouse position.  This is to make the mouse start right in the middle of the screen
float lastX = width / 2, lastY = height / 2;
bool firstMouse = true;




//delta timer, to make sure everybody has the same speed (since different computers will runner faster then others)
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool ready = true;








int main()
{
	//You can ignore this part, this is just to initilize the screen
	// glfw: initialize and configure 
	//------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);// this is to initilize the glfw structure to do certain things (telling which version of OpenGL we are using)

	//glfw window creation 
	//---------------------------------
	GLFWwindow* window = glfwCreateWindow(width, height, "learnOpenGL", NULL, NULL); // This is a structure object with type GLFWwindow*
	if (window == NULL)
	{
		std::cout << "failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//GlAD : load all OpenGL function pointers (Basically we are going in to the gpu and grabbing all the functions for OpenGL)
	//------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))  // We pass GLAD the function to load the address of the OpenGL function pointers which are OS-specifc
	{
		std::cout << "Failed to initilaize GLAD" << std::endl;
		return -1;
	}
	//------------------------------------------------------------------------------
	
	 
	
	//Shader for testing
	Shader Test;
	Test.CompileShader("/Users/Cody Tipton/Desktop/ZGame/ZGame1/Shadervs.txt", "/Users/Cody Tipton/Desktop/ZGame/ZGame1/Shaderfs.txt");
	//Define the world
	Entity_Manager world;
	//Texture 
	Texture2D container_tex = ResourceManager::LoadTexture("/Users/Cody Tipton/Desktop/ZGame/ZGame1/container.jpg", false, "Test");
	Texture2D Brick_wall = ResourceManager::LoadTexture("/Users/Cody Tipton/Desktop/GIT/Minecraft_brick.jpg", false, "Brick");
	
	// This is the testing with zombie boxes
	for (int k = 0; k <7; ++k)
	{
		Zombie_Create(world, container_tex, glm::vec3((float)k*4, 0.0f, 0.0f));
		
	}
	
	for (int k = 0; k < 2; ++k)
	{
		Wall_Spawner(world, Brick_wall, glm::vec3(1.0,-6.0, -2 + (float)k * 2));
	}
	world.components.E_Model.Data[0].Vector_Rot = glm::vec3(1.0, 1.0, 0.0);
	

	


	//Animation Testing
	//first lets define the joints
	Joint Root,L_Thigh, L_Leg, L_Foot,R_Thigh,R_Leg, R_Foot;
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
	SkeletonPose Pose_1(skelly, std::vector<JointPose>{Root_Pose,L_Thigh_Pose, L_Leg_Pose, L_Foot_Pose,R_Thigh_Pose, R_Leg_Pose, R_Foot_Pose});
	
	L_Thigh_Pose.Rot_Quat= Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Leg_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	L_Foot_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Thigh_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Leg_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	R_Foot_Pose.Rot_Quat = Quarternion(0.0, 0.0, 1.0, glm::radians(0.0));
	SkeletonPose Pose_2(skelly, std::vector<JointPose>{Root_Pose, L_Thigh_Pose,L_Leg_Pose,L_Foot_Pose,R_Thigh_Pose,R_Leg_Pose,R_Foot_Pose});

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

	Animation Anim("Wave", skelly, { {0.0, Pose_1}, { 0.5,Pose_2 },{1.0, Pose_3},{1.5,Pose_4},{2.0,Pose_5} },LOOP);
	Animator animator(skelly, Anim);
	bool animate_go = true;
	


	float angle_1=0.0;
	float angle_2 = 0.0;
	

	// Enable FaceCulling
	glEnable(GL_CULL_FACE);
	
	//Apply the depth z buffer (which is automatically used in OpenGL) (see another book for mathematical description of what is going on.)
	glEnable(GL_DEPTH_TEST);
	// captures the mouse cursor and keeps it in the screen
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	

	// render loop
	//This is the loop that makes GLFW running till it stop
	//-----------------------------------------------------
	while (!glfwWindowShouldClose(window))
	{

		//process the delta timer.  This find time it took between each frame.  This is used to make the process of moving around the same on all computers
		// Since my computer is very beefy, so it goes through a ton of frames pers sec, while another not so beefy computer will go through less.
		//This helps with making sure movement wont be faster on mine and slower on a another.  You will see this in action for moving
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//input  (This is the process input that is going on, see the function at the end)  This must always be in the beginning since it is dependent on certain things
		processInput(window); //process input for the keys(see the function below)

		//mouse callback  This GLFW gives the mouse_callback function the information it needs to process the location of the mouse
		glfwSetCursorPosCallback(window, mouse_callback);
		//clear screen
		//-----------
		glClearColor(0.0f, 0.5f, 0.4f, 1.0f); //this is a State-setting function
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //This is a state-using function that uses the current state to retrieve the clearing color from



		//rendering commmands here
		//This is the projection matrix This is the way the objects infront of the camera get projected to the screen
		//Everything on screen is using the same projection matrix
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);  //fov is good at 45 degrees (pi/4)

		//moving the camera (which we are moving the objects inve0rsly porportional to how we want to move.  So techinicall the camera location is not moving)
		glm::mat4 view;
		view = ourCamera.GetViewMatrix(); // From the Camera object we have, it gets the view matrix (see the Camera class for more information)

		

		
		float walk = 5 * deltaTime;
		//Move a box around
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			world.components.E_Model.Data[0].pos.y += walk;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			world.components.E_Model.Data[0].pos.y -= walk;
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			world.components.E_Model.Data[0].pos.x += walk;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			world.components.E_Model.Data[0].pos.x -= walk;
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			angle_1 += 100 * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		{
			angle_2 += 100 * deltaTime;
		}
		
		if (animate_go)
		{
			animate_go = false;
			animator.Start_Animation(currentFrame);
		}

		std::vector<glm::mat4> new_coords=animator.Animate(currentFrame);
		
		for (int k = 1; k < 7; ++k)
		{
			world.components.E_Model.Data[k].pos = glm::vec3(new_coords[k] * glm::vec4(0.0, 0.0, 0.0, 1.0));
		}
		

		
		//Update everything
		//ChangeModel_Pos_World(world,deltaTime); // Change the Pos of objects
		
		Draw_World(world,projection,view,Test);
		int start = clock();
		//Check_Col_World(world);
		int end = clock();


		
		//std::cout << "it took" << end - start << "ticks, or " << ((float)end - start) / CLOCKS_PER_SEC << "seconds." << std::endl;


		glfwSwapBuffers(window); //swaps between the buffers (two screens, one is being changed in the background, then they switch so we dont get wierd artifacts when it is building the scene
		glfwPollEvents(); // process the events
	}



	glfwTerminate(); // To terminate the GLFW process
	return 0;
}



// glfw: whenver the window size changed (by OS or user) this callback function executes
//--------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) //this is a function that takes in the type GLFWwindow* on the first and changes the viewport for gl
{
	//make sure the viewport matches the new window dimensions; note that width and 
	// height will be significanly largeer then the specified on retina display
	glViewport(0, 0, width, height);
}

// Process input function
//----------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// camera movement keys
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		ourCamera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		ourCamera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		ourCamera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		ourCamera.ProcessKeyboard(RIGHT, deltaTime);



}

//callback function which tells GLFW to listen to the mouse movement 
void mouse_callback(GLFWwindow*window, double xpos, double ypos)
{
	// makes the mouse not jump
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;

	};


	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top(opposite as pygame? check this)
	lastX = xpos;
	lastY = ypos;

	ourCamera.ProcessMouseMovement(xoffset, yoffset, deltaTime);


}