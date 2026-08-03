
#include "headers/entity.hpp"
#include <SDL3/SDL_log.h>

// TODO: Replace the matrices with a proper general transform
Entity::Entity(Mesh&& mesh, Shader&& shader, std::optional<Texture>&& texture): 
    mesh(std::move(mesh)), shader(std::move(shader)), texture(std::move(texture)), transform(glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f)) {

        SDL_Log("Entity Created");
    }

Entity::~Entity() {
    SDL_Log("Destroying Entity");
}

void Entity::useEntity() {

    shader.useProgram();

    transform.applyTransform(shader);

    if (texture.has_value()) {
        
        texture->useTexture();
        int textureLocation = glGetUniformLocation(shader.getProgram(), shader.getUniformLocations().textureLocation0);
        glUniform1i(textureLocation, texture->getUnit());
    }
    
    mesh.useMesh();
}

void Entity::updateTransform(Transform transform) {
    this->transform = transform;
}