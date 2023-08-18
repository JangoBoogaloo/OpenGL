#include "Mesh.h"
#include <assimp/Importer.hpp>

#include "GLHeaders.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    GL_EXEC(glGenVertexArrays(1, &vao));
    GL_EXEC(glGenBuffers(1, &vbo));

    GL_EXEC(glBindVertexArray(vao));
    GL_EXEC(glBindBuffer(GL_ARRAY_BUFFER, vbo));

    GL_EXEC(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW));

    GL_EXEC(glGenBuffers(1, &ebo));
    GL_EXEC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
    GL_EXEC(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        &indices[0], GL_STATIC_DRAW));

    // vertex positions
    GL_EXEC(glEnableVertexAttribArray(0));
    GL_EXEC(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));
    // vertex normals
    GL_EXEC(glEnableVertexAttribArray(1));
    GL_EXEC(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)));
    // vertex texture coords
    GL_EXEC(glEnableVertexAttribArray(2));
    GL_EXEC(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)));

    GL_EXEC(glBindVertexArray(0));
}

Mesh::~Mesh()
{
    GL_EXEC(glDeleteVertexArrays(1, &vao));
    GL_EXEC(glDeleteBuffers(1, &vbo));
    GL_EXEC(glDeleteBuffers(1, &ebo));
}