#include <iostream>
#include "Viewer.h"

//Implementation of Viewer code

//Declare Static members Variables
GLFWwindow* Viewer::Window;
int Viewer::Width = 800;
int Viewer::Height = 600;
float Viewer::FOV = 45.0;
float Viewer::lastX =(float)Width / 2;
float Viewer::lastY =(float)Height / 2;
float Viewer::LastFrame = 0.0;
float Viewer::CurrentFrame = 0.0;
float Viewer::deltaTime = 0.0;
bool Viewer::firstMouse = true;
Camera Viewer::camera;
glm::mat4 Viewer::View;
glm::mat4 Viewer::Projection = glm::perspective(glm::radians(FOV), (float)Width / (float)Height, 0.1f, 100.0f);


void Viewer::init()
{
	Display();
	SetGL();
}

void Viewer::Process_Input()
{
	//process the deltaTime
	CurrentFrame = glfwGetTime();
	deltaTime = CurrentFrame - LastFrame;
	LastFrame = CurrentFrame;

	Keyboard(); //process input for the keys(see the function below)

	//mouse callback  This GLFW gives the mouse_callback function the information it needs to process the location of the mouse
	glfwSetCursorPosCallback(Viewer::Window, Mouse_CallBack);

	//clear screen
		//-----------
	glClearColor(0.0f, 0.3f, 0.4f, 1.0f); //this is a State-setting function
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //This is a state-using function that uses the current state to retrieve the clearing color from

	//Get the view matrix from the camera
	View = camera.GetViewMatrix();

}

void Viewer::Swap_Buffers()
{
	glfwSwapBuffers(Viewer::Window); //swaps between the buffers (two screens, one is being changed in the background, then they switch so we dont get wierd artifacts when it is building the scene
	glfwPollEvents(); // process the events
}

void Viewer::Display()
{

	// glfw: initialize and configure 
	//------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//glfw window creation 
	//---------------------------------
	Window = glfwCreateWindow(Width, Height, "Main_Window", NULL, NULL); // This is a structure object with type GLFWwindow*
	if (Window == NULL)
	{
		std::cout << "failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(Window);
	glfwSetFramebufferSizeCallback(Window, FrameBuffer_Size_Callback);

	//GlAD : load all OpenGL function pointers (Basically we are going in to the gpu and grabbing all the functions for OpenGL)
	//------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))  // We pass GLAD the function to load the address of the OpenGL function pointers which are OS-specifc
	{
		std::cout << "Failed to initilaize GLAD" << std::endl;
		return;
	}
	//------------------------------------------------------------------------------


}


void Viewer::SetGL()
{
	glEnable(GL_CULL_FACE); // Enable culling counter clockwise
	glEnable(GL_DEPTH_TEST); // Depth Testing
	//glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Keep the mouse on the screen

}






void Viewer::Keyboard()
{
	if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(Window, true);

	// camera movement keys
	if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void Viewer::Mouse_CallBack(GLFWwindow* window, double xpos, double ypos)
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

	camera.ProcessMouseMovement(xoffset, yoffset, deltaTime);
}



void Viewer::FrameBuffer_Size_Callback(GLFWwindow* window, int width, int height)
{
	//make sure the viewport matches the new window dimensions; note that width and 
	// height will be significanly largeer then the specified on retina display
	glViewport(0, 0, width, height);
}


//LIGHTING EFFECT!

void Viewer::SetLighting(glm::vec3 pos, Shader shader)
{
	shader.use();
	shader.setVec3("viewPos", Viewer::camera.Position);
	shader.setVec3("light.ambient", 0.2, 0.2, 0.2);
	shader.setVec3("light.diffuse", 0.5, 0.5, 0.5);
	shader.setVec3("light.specular", 1.0, 1.0, 1.0);
	shader.setVec3("lightPos", pos);
}