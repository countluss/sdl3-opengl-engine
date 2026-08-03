
#pragma once

#include "headers/mesh.hpp"
#include "headers/shader.hpp"
#include "headers/texture.hpp"
#include "headers/transform.hpp"
#include <optional>
class Entity {

    public:

        Mesh mesh;
        Shader shader;
        std::optional<Texture> texture;
        Transform transform;

        Entity(Mesh&& mesh, Shader&& shader, std::optional<Texture>&& texture);
        ~Entity();
        
        void useEntity();
        void updateTransform(Transform transform);
};