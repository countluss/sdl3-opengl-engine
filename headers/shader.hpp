
#pragma once

#include "external/glm/ext/vector_float3.hpp"
#include "headers/uniform_locations.hpp"
#include <string>
#include <utility>

class Shader {

    private:

        unsigned int vertexShader, fragmentShader;
        unsigned int program;
        bool status = false;
        std::string readFile(std::string filePath);
        const UniformLocations uniformLocations;

        void compileAndLinkShaders(const char* vertexShaderSource, const char* fragmentShaderSource);

      public:

        Shader(const char *vertexShaderPath, const char *fragmentShaderPath);

        Shader(const char *vertexShaderPath, const char *fragmentShaderPath,
        const UniformLocations uniformLocations);

        Shader(Shader&& other) noexcept :
            vertexShader(std::exchange(other.vertexShader, 0)),
            fragmentShader(std::exchange(other.fragmentShader, 0)),
            program(std::exchange(other.program, 0)),
            status(std::exchange(other.status, false)),
            uniformLocations(other.uniformLocations) {};

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;
        
        ~Shader();

        
        void useProgram() const;
        bool getShaderStatus() const;
        unsigned int getProgram() const;
        void setVec3(const char* name, float x, float y, float z);
        void setVec3(const char* name, glm::vec3 vector);
        void setFloat(const char* name, float value);
        void setInt(const char* name, int value);

        const UniformLocations getUniformLocations() const;



};