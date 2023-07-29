#include "Shader.h"
#include "GLErrorCheck.h"

Shader::Shader(const char* vertexProgram, const char* fragmentProgram)
{
	unsigned int vertexShader, fragmentShader;

	GL_EXEC(vertexShader = glCreateShader(GL_VERTEX_SHADER));
	GL_EXEC(glShaderSource(vertexShader, 1, &vertexProgram, NULL));
	GL_COMPILE(vertexShader);

	GL_EXEC(fragmentShader = glCreateShader(GL_FRAGMENT_SHADER));
	GL_EXEC(glShaderSource(fragmentShader, 1, &fragmentProgram, NULL));
	GL_COMPILE(fragmentShader);

	GL_EXEC(programId = glCreateProgram());
	GL_EXEC(glAttachShader(programId, vertexShader));
	GL_EXEC(glAttachShader(programId, fragmentShader));
	GL_LINK(programId);

	//Clean up shader after program linked, save memory
	GL_EXEC(glDetachShader(programId, vertexShader));
	GL_EXEC(glDeleteShader(vertexShader));

	GL_EXEC(glDetachShader(programId, fragmentShader));
	GL_EXEC(glDeleteShader(fragmentShader));
}

const unsigned int Shader::Id()
{
	return programId;
}

const unsigned int Shader::GetAttributeLocation(const std::string& name)
{
	unsigned int attributeLocation;
	GL_EXEC(attributeLocation = glGetAttribLocation(programId, name.c_str()));
	return attributeLocation;
}

void Shader::Use()
{
	GL_EXEC(glUseProgram(programId));
}

Shader::~Shader() 
{
	GL_EXEC(glDeleteProgram(programId));
#ifdef _DEBUG
	GLint deleteStatus;
	GL_EXEC(glGetProgramiv(programId, GL_DELETE_STATUS, &deleteStatus));
	if (deleteStatus != GL_TRUE) DEBUG_THROW;
#endif // _DEBUG
}