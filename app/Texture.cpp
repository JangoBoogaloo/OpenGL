#include "Texture.h"
#include "GLErrorCheck.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const char* textureFile)
{
    // tell stb_image.h to flip loaded texture's on the y-axis.
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(textureFile, &width, &height, &nrChannels, 0);
#ifdef _DEBUG
    if (!data) {
        std::cout << "Failed to load texture: " << textureFile << std::endl;
        DEBUG_THROW;
    }
#endif // _DEBUG
}

unsigned int Texture::Load(TextureLoadfun fun)
{
    return fun(width, height, nrChannels, data);
}
Texture::~Texture()
{
    stbi_image_free(data);
}