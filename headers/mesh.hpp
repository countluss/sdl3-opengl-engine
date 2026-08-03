#pragma once

#include <GLES3/gl31.h>
#include <utility>
#include <vector>
#include "vertex.hpp"

// Usage: constructor initialization, then set vertices and index, then createMesh(), then renderMesh() to use

class Mesh {

    private:
        
        unsigned int VAO, VBO, EBO;
        unsigned int positionLocation, colorLocation, textureLocation;
        
        std::vector<Vertex> vertices; // Interleaved Vertices, pos float[3], color float[3]
        std::vector<unsigned int> indices; // Indices for EBO
        
    public:

        Mesh(unsigned int positionLocation, unsigned int colorLocation, unsigned int textureLocation, std::vector<Vertex> vertices, std::vector<unsigned int> indices);

        Mesh(Mesh& other) = delete;
        Mesh& operator=(Mesh& other) = delete;

        Mesh(Mesh&& other) noexcept :
            VAO(std::exchange(other.VAO, 0)),
            VBO(std::exchange(other.VBO, 0)),
            EBO(std::exchange(other.EBO, 0)),
            positionLocation(other.positionLocation),
            colorLocation(other.colorLocation),
            textureLocation(other.textureLocation),
            vertices(std::move(other.vertices)),
            indices(std::move(other.indices)) {};

        ~Mesh();

        void useMesh();

};