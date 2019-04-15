#ifndef VIEWER_H
#define VIEWER_H

#include <glm/glm.hpp> //the GLM math header files (all the awesome math goodness!!)
#include <glm/gtc/matrix_transform.hpp>  //More math stuff from glm
#include <glm/gtc/type_ptr.hpp>  //this is to change the way matrices look so that is can be implemted in to OpenGL
#include <glad/glad.h> // 
#include <GLFW/glfw3.h> // this is the window application and the way we can look for inputs through keys and mouse
#include "Camera.h"
#include "Shader.h"

//Forward Deceleration



//This header will hold the class that holds all the information for the window, screen color, certain matrices for projection and view
// Also the little things light depth, keeping the mouse in the screen, controls, ect....




class Viewer
{
public:
	static Camera camera;

	static void init(); // Displays the viewer until the program ends

	static void Process_Input();	//Process the Inputs (Keyboard controllers,ect...)

	static void Swap_Buffers();		//Swap between two framebuffers

	static void Game_Loop();  //This holds all the little game loops for the game, like level1, level2, main menu, pause menu, to do a state machine ect...

	static float deltaTime;  //To make everybody have the same speed  

	static float CurrentFrame;	//Current frame

	//Window
	static GLFWwindow* Window;		//window application 

	//Projection matrix
	static glm::mat4 Projection;

	//View Matrix
	static glm::mat4 View;

	//Lighting effects, needs position of light source
	static void SetLighting(glm::vec3 pos, Shader shader);
	
private:

	//Window size
	static int Width;
	static int Height;

	//Field of View
	static float FOV;

	
	//Keeps track of the view state


	// used for the mouse position.  This is to make the mouse start right in the middle of the screen
	static float lastX;
	static float lastY;								// center where the mouse will start
	static bool firstMouse;							//is this the beginning, so we can initalize the mouse at the center

	//which frame we are at and this is used to make the deltaTime
	static float LastFrame;

	static Shader shader;
	//Holds the shader info, Later, I will use the Resource Mananger to switch between shaders on the fly

	//Constructor
	Viewer() {};
	

	//Things to be initialize
	
	static void Display(); // Initalize all the basic display for glfw
	static void SetGL(); // Set color (lighting)


	//Active functions that change
	static void Keyboard();
	static void Mouse_CallBack(GLFWwindow* window, double xpos, double ypos);
	static void DrawScene();
	static void FrameBuffer_Size_Callback(GLFWwindow* window, int width, int height);
	static void Big_Loop();  //This is the big loop that holds everything 
	
	


	

	

};













#endif
