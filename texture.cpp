
#include "headers/texture.hpp"
#include <GLES3/gl31.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"


// TODO: Accept the wrapping parameters for texture in constructor
Texture::Texture(std::string filePath, unsigned short unit): unit(unit) {

    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    unsigned char* imgData = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

    if (!imgData) {
        std::cout << "Failed to load image" << std::endl;
        return;
    }

    glGenTextures(1, &texture);

    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (nrChannels == 3) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                   GL_UNSIGNED_BYTE, imgData);
    } else if (nrChannels == 4) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                   GL_UNSIGNED_BYTE, imgData);
    }

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(imgData);

    status = true;
}

void Texture::useTexture() {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, texture);
}

bool Texture::getTextureStatus() {
    return status;
}

Texture::~Texture() {

    if (texture != 0) {
        glDeleteTextures(1, &texture);
        std::cout << "Destroying Texture" << std::endl;
    }
}

unsigned int Texture::getUnit() {
    return unit;
}
