
#pragma once

#include <string>
#include <utility>

class Texture {

    private:
        unsigned int texture;
        unsigned short unit;
        bool status = false;

    public:
        Texture(std::string filePath, unsigned short unit);
        Texture(Texture&& other) noexcept :
            texture(std::exchange(other.texture, 0)), unit(other.unit), status(other.status) {};

        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

        ~Texture();

        void useTexture();
        bool getTextureStatus();
        unsigned int getUnit();
};