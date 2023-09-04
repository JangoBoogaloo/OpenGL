#include "Model.h"

#include <iostream>
#include "TextureLoader.h"
#include "GLHeaders.h"

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    for (auto i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, scene));
    }
    // then do the same for each of its children
    for (auto i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    for (auto i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        vertex.Position.x = mesh->mVertices[i].x;
        vertex.Position.y = mesh->mVertices[i].y;
        vertex.Position.z = mesh->mVertices[i].z;
        if (mesh->mNormals) {
            vertex.Normal.x = mesh->mNormals[i].x;
            vertex.Normal.y = mesh->mNormals[i].y;
            vertex.Normal.z = mesh->mNormals[i].z;
        }
        if (mesh->mTextureCoords[0]) {  // per mesh can have 8 different texture coord, but here we only use first
            vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
            vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
        }
        vertices.push_back(vertex);
    }

    for (auto i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (auto j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = LoadTextures(material,
            aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = LoadTextures(material,
            aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        Texture texture;
        texture.TextureId = TextureLoader::TextureFromFile(str.C_Str());
        texture.type = typeName;
        texture.path = str.C_Str();
        textures.push_back(texture);
    }
    return textures;
}

Model::Model(const char* path)
{
    Assimp::Importer import;
    const aiScene * scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "Fail Loading " << path << std::endl;
        std::cerr << "    ERROR: " << import.GetErrorString() << std::endl;
#ifdef _DEBUG
        DEBUG_THROW;
#endif
        return;
    }
    ProcessNode(scene->mRootNode, scene);
}


void Model::Draw(Shader& shader)
{
    for (auto i = 0; i < meshes.size(); i++) {
        meshes[i].Draw(shader);
    }
}

void Model::LoadToGPU(int vertexAL, int normAL, int textureAL)
{
    for (auto i = 0; i < meshes.size(); i++) {
        meshes[i].LoadToGPU(vertexAL, normAL, textureAL);
    }
}