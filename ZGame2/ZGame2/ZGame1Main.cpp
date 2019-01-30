
#include <iostream>
#include "Viewer.h"
#include "State_Machine.h"
#include "Levels.h"



int main()
{
	State state = TEST;
	Viewer::init(); // Initilize the the display and GL_enabled functions

	while (!glfwWindowShouldClose(Viewer::Window)) //Big Loop for everything
	{
		
		if (state == TEST)
		{
			Testing_Ground(state);
		}







	}

	glfwTerminate();
	return 0;
}

