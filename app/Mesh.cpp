#include "Mesh.h"
#include <assimp/Importer.hpp>

#include "GLHeaders.h"

const unsigned int ElementPerVertex = 3;
const unsigned int ElementPerTex = 2;

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    vao = -1;
    vbo = -1;
    ebo = -1;
}

void Mesh::Draw(Shader& shader)
{
    // draw mesh
    GL_EXEC(glBindVertexArray(vao));
    GL_EXEC(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0));
    GL_EXEC(glBindVertexArray(0));
}

void Mesh::LoadToGPU(int vertexAL, int normAL, int textureAL)
{
    GL_EXEC(glGenVertexArrays(1, &vao));
    GL_EXEC(glGenBuffers(1, &vbo));
    GL_EXEC(glBindVertexArray(vao));
    GL_EXEC(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GL_EXEC(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW));
    if (vertexAL >= 0) {
        // vertex positions
        GL_EXEC(glEnableVertexAttribArray(vertexAL));
        GL_EXEC(glVertexAttribPointer(vertexAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));
    }
    if (normAL >= 0) {
        // vertex normals
        GL_EXEC(glEnableVertexAttribArray(normAL));
        GL_EXEC(glVertexAttribPointer(normAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)));
    }
    if (textureAL >= 0) {
        // vertex texture coords
        GL_EXEC(glEnableVertexAttribArray(textureAL));
        GL_EXEC(glVertexAttribPointer(textureAL, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)));
    }
    GL_EXEC(glGenBuffers(1, &ebo));
    GL_EXEC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
    GL_EXEC(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        &indices[0], GL_STATIC_DRAW));

    GL_EXEC(glBindVertexArray(0));
}

Mesh::~Mesh()
{
    if (vao > 0) GL_EXEC(glDeleteVertexArrays(1, &vao));
    if (vbo > 0) GL_EXEC(glDeleteBuffers(1, &vbo));
    if (ebo > 0) GL_EXEC(glDeleteBuffers(1, &ebo));
}