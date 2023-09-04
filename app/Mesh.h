#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <string>

#include "Shader.h"

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture
{
    unsigned int TextureId;
    std::string type;
    std::string path;
};

class Mesh
{

public:
    const std::vector<Vertex> const Vertices() { return vertices; }
    const std::vector<Texture> const Textures() { return textures; }
    const std::vector<unsigned int> const Indices() { return indices; }

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void Draw(Shader& Shader);
    void LoadToGPU(int vertexAL, int normAL, int textureAL);
    ~Mesh();

private:
    unsigned int vao, vbo, ebo;
    std::vector<Vertex> vertices;
    std::vector<Texture> textures;
    std::vector<unsigned int> indices;
};

