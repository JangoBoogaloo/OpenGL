#pragma once
#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "Mesh.h"

class Model
{
public:
    Model(const char* path);
    void Draw(Shader& shader);
    const std::vector<Mesh> Meshes() const { return meshes; }
    void LoadToGPU(int vertexAL, int normAL, int textureAL);
private:
    std::vector<Mesh> meshes;
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> LoadTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

