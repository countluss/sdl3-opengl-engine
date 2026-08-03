
#pragma once

#include "../external/glm/mat4x4.hpp"
#include "shader.hpp"

class Transform {

    private:
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;

    public:
        Transform(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
        ~Transform();

        void applyTransform(const Shader &shader) const;

};