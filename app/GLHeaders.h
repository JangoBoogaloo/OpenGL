#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

//Very useful light-weight error checking that helps debug OpenGL

#ifdef _DEBUG
#define DEBUG_THROW do { \
  std::cout << "Exception at "<< __FILE__ <<":"<< __LINE__ << std::endl; \
  abort(); \
} while (0)
#else
#define DEBUG_THROW
#endif

inline bool GL_ExecuteOK(const char* stmt)
{
	GLenum errorCode = glGetError();
	if (errorCode != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		default:							   error = "Unknown Error code: " + errorCode; break;
		}
		std::cout << stmt << " " << error.c_str() << std::endl;
		return false;
	}
	else
	{
		return true;
	}
}

#ifdef _DEBUG
#define GL_EXEC(stmt) do { \
	stmt; \
	if(!GL_ExecuteOK(#stmt)) { DEBUG_THROW; }\
} while (0)
#else
#define GL_EXEC(stmt) stmt
#endif


inline bool GL_CompileOK(GLint shader) {
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}
	else
	{
		std::cout << "Shader Compile Succeed" << std::endl;
		return true;
	}
}

#ifdef _DEBUG
#define GL_COMPILE(shader) do { \
	glCompileShader(shader); \
  if(!GL_CompileOK(shader)) { DEBUG_THROW; }\
} while (0)
#else
#define GL_COMPILE(shader) do { \
	glCompileShader(shader); \
} while (0)
#endif

inline bool GL_LinkOK(GLint shaderProgram) {
	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
		return false;
	}
	else
	{
		std::cout << "Program Link Succeed" << std::endl;
		return true;
	}
}

#ifdef _DEBUG
#define GL_LINK(program) do { \
	glLinkProgram(program); \
  if(!GL_LinkOK(program)) { DEBUG_THROW; }\
} while (0)
#else
#define GL_LINK(program) do { \
	glLinkProgram(program); \
} while (0)
#endif