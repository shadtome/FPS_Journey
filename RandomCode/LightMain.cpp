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
#include "Cube.h"
#include "Plane.h"
#include "BulletVAO.h"
#include "Bullets.h"
#include "Guns.h"
#include "Lines.h"
#include "VAO.h"
#include "ResourceManager.h"
#include "Textures2D.h"
#include <map>




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


//Guns for this game to be ready to shoot.  (see input proccessing function and the end of the rending loop)
bool Pistol_ready = true;
bool Submachine_ready = false;








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

	//---------------------------------------------------------------------------------------------
	// BULLET AND GUN INFORMATION
	//BulletShader and class
	/*Shader BulletShader("/Users/Cody Tipton/Desktop/OpenGLlearning/OpenGLZombie001/BulletShadervs.txt", "/Users/Cody Tipton/Desktop/OpenGLlearning/OpenGLZombie001/BulletShaderfs.txt");
	BulletVAO VAOBullet;



	//Bind the textures for BulletShader
	VAOBullet.bindVAO();
	BulletShader.use();
	BulletShader.setInt("texture1", 0);
	VAOBullet.unbindVAO();

	//Gun object  (see class for Guns)
	Guns Pistol(BulletShader, VAOBullet, 400, 400, false, 10, 0.1f);
	Guns SubMachineGun(BulletShader, VAOBullet, 400, 400, true, 30, 0.1f);*/
	// BULLET AND GUN information end
	//--------------------------------------------------------------------------------------------------

	// Define our Shaders from Resource managment 
	ResourceManager::LoadShader("/Users/Cody Tipton/Desktop/OpenGLlearning/OpenGL4/LightShadervs.txt", "/Users/Cody Tipton/Desktop/OpenGLlearning/OpenGL4/LightShaderfs.txt", "lighting");
	ResourceManager::LoadShader("/Users/Cody Tipton/Desktop/OpenGLlearning/OpenGL4/LampShadervs.txt", "/Users/Cody Tipton/Desktop/OpenGLlearning/OpenGL4/LampShaderfs.txt", "lamp");
	
	unsigned VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_normal), vertices_normal, GL_STATIC_DRAW);
	//Lamp VAO
	//VertexAO LightVAO(vertices, 0, false);
	unsigned int LightVAO;
	glGenVertexArrays(1, &LightVAO);
	glBindVertexArray(LightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//LightVAO.UnbindVAO();

	


	
	//VertexAO ContainerVAO(vertices, 0, false);
	unsigned int ContainerVAO;
	glGenVertexArrays(1,&ContainerVAO);
	glBindVertexArray(ContainerVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	










	//Apply the depth z buffer (which is automatically used in OpenGL) (see another book for mathematical description of what is going on.)
	glEnable(GL_DEPTH_TEST);



	// captures the mouse cursor and keeps it in the screen
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	// this is how much of each texture I want to show more then the other (This was for the opacity of the smily face and the other texture on the floating cubes)
	float alpha = 0.2f;

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
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //this is a State-setting function
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //This is a state-using function that uses the current state to retrieve the clearing color from



		//rendering commmands here
		//This is the projection matrix This is the way the objects infront of the camera get projected to the screen
		//Everything on screen is using the same projection matrix
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);  //fov is good at 45 degrees (pi/4)

		//moving the camera (which we are moving the objects inve0rsly porportional to how we want to move.  So techinicall the camera location is not moving)
		glm::mat4 view;
		view = ourCamera.GetViewMatrix(); // From the Camera object we have, it gets the view matrix (see the Camera class for more information)
		
		//initalize a model matrix
		glm::mat4 model;
		glm::mat4 NormalMatrix;
		//Lighting tutorial stuff
		float time= glfwGetTime();
		glm::vec3 lightPos=glm::vec3(2.0*cos(time), 2.0*sin(time), 2.0f);// Light position


		// The lamp
		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		ResourceManager::GetShader("lamp").use();
		ResourceManager::GetShader("lamp").setMat4("view", view);
		ResourceManager::GetShader("lamp").setMat4("projection", projection);
		ResourceManager::GetShader("lamp").setMat4("model", model);
		glBindVertexArray(LightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		

		//Position of the boxes
		std::vector<glm::vec3> Cube_Pos = {
			   glm::vec3(0.2f, -2.0f, 1.0f),
			   glm::vec3(1.2f, -1.0f, 4.0f),
			   glm::vec3(3.0f, -10.0f, 1.0f)
		};
		for (int k = 0; k < Cube_Pos.size(); ++k)
		{

			model = glm::mat4();
			model = glm::translate(model, Cube_Pos[k]);
			NormalMatrix = glm::transpose(glm::inverse(model));
			ResourceManager::GetShader("lighting").use();
			ResourceManager::GetShader("lighting").setMat4("view", view);
			ResourceManager::GetShader("lighting").setMat4("projection", projection);
			ResourceManager::GetShader("lighting").setMat4("model", model);
			ResourceManager::GetShader("lighting").setMat4("NormalMatrix", NormalMatrix);
			ResourceManager::GetShader("lighting").setVec3("viewPos", ourCamera.Position);
			ResourceManager::GetShader("lighting").setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
			ResourceManager::GetShader("lighting").setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
			ResourceManager::GetShader("lighting").setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
			ResourceManager::GetShader("lighting").setFloat("material.shininess", 32.0f);
			ResourceManager::GetShader("lighting").setVec3("light.ambient", 0.2f,0.2f,0.2f);
			ResourceManager::GetShader("lighting").setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
			ResourceManager::GetShader("lighting").setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
			ResourceManager::GetShader("lighting").setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
			ResourceManager::GetShader("lighting").setVec3("lightColor", 1.0f, 1.0f, 1.0f);
			ResourceManager::GetShader("lighting").setVec3("lightPos", lightPos);
			glBindVertexArray(ContainerVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);

		}
		
		






		//Gun and bullet Rendering beginning
		//-----------------------------------------------------
		//See the gun and bullet classes
		//goes through the process for the gun that is active
		/*Pistol.Active = Pistol_ready;
		SubMachineGun.Active = Submachine_ready;
		Pistol.shoot(window, deltaTime, projection, view, ourCamera.Front, ourCamera.Position);
		SubMachineGun.shoot(window, deltaTime, projection, view, ourCamera.Front, ourCamera.Position);
		Pistol.Reload(window);
		SubMachineGun.Reload(window);*/

		//End of Gun and BUllet Rendering
		//------------------------------------------------------------

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

	//Gun processing
	// THis picks the weapon to choose
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		Pistol_ready = true;
		Submachine_ready = false;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		Pistol_ready = false;
		Submachine_ready = true;
	}

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