#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <string>

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
};

class Mesh
{
public:
    std::vector<Vertex> vertices;
    std::vector<Texture> textures;
    std::vector<unsigned int> indices;
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    ~Mesh();
private:
    unsigned int vao, vbo, ebo;
};

