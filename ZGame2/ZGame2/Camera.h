#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Some notes, the coordinate system for the world is  that y is pointing up and z is pointing behind you and the x coordinate is right.

enum Camera_Movement {
	FORWARD=GLFW_KEY_W,
	BACKWARD=GLFW_KEY_S,
	LEFT=GLFW_KEY_A,
	RIGHT=GLFW_KEY_D
};


//Default Camera Values
const float YAW = 90.0f;
const float PITCH = 0.0f;
const float SPEED = 30.0f;
const float SENSITIVITY = 0.05f;
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, vectors and Matrices for use in OPENGL
class Camera
{
public:
	// camera Attributes
	glm::vec3 Position;  //Position of the camera
	glm::vec3 Front;  // the vector that the camera is pointing towards
	glm::vec3 Up; // this is the vector putting up from the camera
	glm::vec3 Right; // this is the vector pointing right from the camera
	glm::vec3 WorldUp;  //this is the world up basis vector (0,1,0)
	glm::vec3 ProjFront; //projectoin of the Front vector so you can move foward and look straight up
	//The Front, Up and Right create a orthonormal basis for the local coordinates of the camera

	//Euler Angles
	float Yaw;
	float Pitch;

	//Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	//constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		ProjFront = glm::vec3(Front.x, 0.0f, Front.z);
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	//Constructor with scalar Values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		ProjFront = glm::vec3(Front.x, 0, Front.z);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	//returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		/* This is a 4x4 matrix that is defined by the upper left 3x3 matrix is the basis vectors for the local coordinates of the camera.  The right column is the negative of the position of the camera.
		This matrix makes it where we look at a certain location*/
		return glm::lookAt(Position, Position + Front, Up); 
	}

	//processes input recieved from any keyboard-like input system.  Accepts input paramter in the form of camera defined ENUM.
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
		{
			Position += Front * velocity;
			
		}
		if (direction == BACKWARD)
		{
			Position -= ProjFront * velocity;
			
		}
		if (direction == LEFT)
			Position -= Right * velocity;
		if (direction == RIGHT)
			Position += Right * velocity;
	}

	//Processes input recieved from a mouse input system.  Expects the offset value in both the x and y direction
	void ProcessMouseMovement(float xoffset, float yoffset,float deltaTime, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		//Make sure that when pitch is out of bounds, the screen does not get flipped
		if (constrainPitch)
		{
			if (Pitch > 85.0f)
				Pitch = 85.0f;
			if (Pitch < -85.0f)
				Pitch = -85.0f;
		}

		//update Front, Right and Up vectors using the updated angles
		updateCameraVectors();
	}

	//procces input recieved from a mouse scroll-wheel event (ZOOM) (this is for later when I want to do scopes)
	

	//output current front vector
	glm::vec3 Front_vector()
	{
		return Front;
	}

private:
	//calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors()
	{
		//Calculate the new Front Vector
		glm::vec3 front;
		front.x = -cos(glm::radians(Yaw))*cos(glm::radians(Pitch)); //see Euler angles and do the computation in regular coordinates and then switch z and y.
		front.y = sin(glm::radians(Pitch));
		front.z = -sin(glm::radians(Yaw))*cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		ProjFront = glm::normalize(glm::vec3(Front.x, 0.0f, Front.z));

		//Also recalculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}


};

#endif
