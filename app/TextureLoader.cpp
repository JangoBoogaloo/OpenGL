#include "TextureLoader.h"

#include "GLHeaders.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int TextureLoader::TextureFromFile(const char* textureFile, bool flip) {
    // tell stb_image.h to flip loaded texture's on the y-axis.
    stbi_set_flip_vertically_on_load(flip);
    int width, height, nrChannels;
    auto data = stbi_load(textureFile, &width, &height, &nrChannels, 0);
    if (!data) {
        std::cerr << "Failed to load texture: " << textureFile << std::endl;
        stbi_image_free(data);
#ifdef _DEBUG
        DEBUG_THROW;
#endif
        return -1;
    }

    unsigned int textureID;
    GL_EXEC(glGenTextures(1, &textureID));
    GL_EXEC(glBindTexture(GL_TEXTURE_2D, textureID));

    // set the texture wrapping/filtering options (on the currently bound texture object)
    GL_EXEC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_EXEC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    //When scale down, make it more blocked pattern
    GL_EXEC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    //When scale up, make it more linear pattern
    GL_EXEC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    GLenum format = GL_RGB;
    if (nrChannels == 1)
        format = GL_RED;
    else if (nrChannels == 3)
        format = GL_RGB;
    else if (nrChannels == 4)
        format = GL_RGBA;


    GL_EXEC(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
    GL_EXEC(glGenerateMipmap(GL_TEXTURE_2D));
    GL_EXEC(glBindTexture(GL_TEXTURE_2D, 0));
    stbi_image_free(data);
    return textureID;
}