#ifndef SHADER_H  //this is a prepoccesor directive.  The compiler goes to this, and if this header was already used it skips the contents of this header
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <string>
#include <fstream>  //file stream class for handling files
#include <sstream>
#include <iostream>
#include <map>

class Shader
{
public:
	// the program ID :: each shader has a unique ID number that goes with it
	unsigned int ID;

	//Default Constructor
	Shader() {};
	//Constructor
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	// Compile Shader
	void CompileShader(const GLchar* vertexPath, const GLchar* fragmentPath);

	//use/activate the shader
	void use();
	void Stop();
	//utility uniform functions
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	

	//utility uniform functions for vec3
	void setVec3(const std::string &name, glm::vec3 vector) const;
	void setVec3(const std::string &name, float x, float y, float z) const;

	//utility uniform functions for Mat4
	void setMat4(const std::string &name, glm::mat4 &mat) const;



};




#endif
