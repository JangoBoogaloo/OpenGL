#pragma once
#include <glad/glad.h>
#include <string>

class Shader
{
public:
    Shader(const char* vertexProgram, const char* fragmentProgram);
    unsigned int Id();
    unsigned int GetAttributeLocation(const std::string& name);
    void Use();
    ~Shader();

private:
    unsigned int programId;

};

