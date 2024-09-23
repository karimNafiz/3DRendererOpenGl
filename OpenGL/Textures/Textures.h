#pragma once

#include "glew.h"
#include <iostream>
#include <string>
#include "./3rdParty/stb_image.h"



class Texture
{
private:
    GLuint textureID;
    GLenum textureTarget;
    int width, height, numChannels;

public:
    Texture() = default;

    bool loadTexture(const std::string& filePath, GLenum textureTarget = GL_TEXTURE_2D)
    {
        this->textureTarget = textureTarget;

        // Load image using stb_image
        stbi_set_flip_vertically_on_load(true);
        unsigned char* imageData = stbi_load(filePath.c_str(), &width, &height, &numChannels, 0);

        if (!imageData)
        {
            std::cerr << "Failed to load texture: " << filePath << std::endl;
            return false;
        }

        // Determine format
        GLenum format;
        if (numChannels == 1)
            format = GL_RED;
        else if (numChannels == 3)
            format = GL_RGB;
        else if (numChannels == 4)
            format = GL_RGBA;
        else
        {
            std::cerr << "Unknown number of channels: " << numChannels << std::endl;
            stbi_image_free(imageData);
            return false;
        }

        // Generate and bind texture
        glGenTextures(1, &textureID);
        glBindTexture(textureTarget, textureID);

        // Load texture data into OpenGL
        glTexImage2D(textureTarget, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(textureTarget);

        // Set texture parameters
        glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Unbind texture
        glBindTexture(textureTarget, 0);

        // Free image data
        stbi_image_free(imageData);

        return true;
    }

    void bind(GLenum textureUnit)
    {
        glActiveTexture(textureUnit);
        glBindTexture(textureTarget, textureID);
    }

    void unbind()
    {
        glBindTexture(textureTarget, 0);
    }

    ~Texture()
    {
        glDeleteTextures(1, &textureID);
    }
};
