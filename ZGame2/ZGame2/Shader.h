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


Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	Shader::CompileShader(vertexPath, fragmentPath);
}

void Shader::CompileShader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	//1. retrieve the vertex/fragment source code from filepath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		//open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		//read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		//close file handlers
		vShaderFile.close();
		fShaderFile.close();
		//convert stream into string
		vertexCode = vShaderStream.str(); //see documentation pg 412 in c++primer
		fragmentCode = fShaderStream.str();

	}
	catch (std::ifstream::failure e) // look at this later for exceptions in the c++ book
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str(); // turn the c++ string in to c-string
	const char* fShaderCode = fragmentCode.c_str();

	// compile shaders
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	//vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER); // give the unique integer to the vertex shader
	glShaderSource(vertex, 1, &vShaderCode, NULL); //associated the C-string course with this vertex shader
	glCompileShader(vertex);
	//print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	//print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//shader program


	//This links the above vertex and fragment shaders in to one shader 
	ID = glCreateProgram();  //Note that ID is already object of this class
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	//print linking errors if any
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

// run the shader program
void Shader::use()
{
	glUseProgram(ID);
}

//uniform setter functions  (do the ones for vec2, vec3, vec4 ,ect)
//----------------------------------------------------------------
void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
//----------------------------------------------------------------
void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
//-----------------------------------------------------------------
void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

// uniform setter functions for vec3

void Shader::setVec3(const std::string &name, glm::vec3 vector) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), vector.x, vector.y, vector.z);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}


void Shader::setMat4(const std::string &name, glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

#endif
