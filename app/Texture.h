#pragma once

typedef unsigned int (*TextureLoadfun)(int width, int height, int nrChannel, const unsigned char* data);

class Texture
{
public:
    Texture(const char* textureFile);
    unsigned int Load(TextureLoadfun fun);
    ~Texture();
private:
    int width;
    int height;
    int nrChannels;
    unsigned char* data;
};