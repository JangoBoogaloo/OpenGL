#pragma once
#include <string>

#include <glad/glad.h>
#include <glm.hpp>

class Shader
{
public:
    Shader(const char* vertexProgram, const char* fragmentProgram);
    unsigned int Id();
    unsigned int GetAttributeLocation(const std::string& name);
    void Use();
    void SetUniformMatrix4fv(const std::string& name, const glm::mat4 matrix);

    ~Shader();

private:
    unsigned int programId;

};

