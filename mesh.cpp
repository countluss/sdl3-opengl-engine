
#include "headers/mesh.hpp"
#include "headers/vertex.hpp"
#include <GLES3/gl31.h>
#include <cstddef>
#include <iostream>

Mesh::~Mesh() {

    if (VAO != 0)
        glDeleteVertexArrays(1, &VAO);
    if (VBO != 0)
        glDeleteBuffers(1, &VBO);
    if (EBO != 0)
        glDeleteBuffers(1, &EBO);

    if (VAO != 0 || VBO != 0 || EBO != 0)
        std::cout << "Destroying Mesh" << std::endl;
}

Mesh::Mesh(unsigned int positionLocation, unsigned int colorLocation, unsigned int textureLocation, std::vector<Vertex> vertices, std::vector<unsigned int> indices)
    : positionLocation(positionLocation), colorLocation(colorLocation), textureLocation(textureLocation), vertices(vertices), indices(indices) {

  // Generate
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  // Attach
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * this->vertices.size(),
               this->vertices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * this->indices.size(),
               this->indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE,
                        sizeof(Vertex), (void *)offsetof(Vertex, position));
  glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, color));
  glVertexAttribPointer(textureLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, uv));

  // TODO: Temporary                        
  unsigned int normalLocation = 3;
  glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));

  glEnableVertexAttribArray(positionLocation);
  glEnableVertexAttribArray(colorLocation);
  glEnableVertexAttribArray(textureLocation);
  glEnableVertexAttribArray(normalLocation);

  // Detach
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Attach Texture


}

void Mesh::useMesh() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}