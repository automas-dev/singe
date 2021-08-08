#pragma once

#include <GL/glew.h>

#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

// https://www.khronos.org/opengl/wiki/Shader_Compilation

namespace Tom::s3e {
    /**
     * Manages a single OpenGL shader.
     */
    class Shader {
        GLuint program;

    public:
        using Ptr = std::shared_ptr<Shader>;
        using ConstPtr = std::shared_ptr<const Shader>;

        Shader();
        virtual ~Shader();

        bool loadFromSource(const std::string & vertexSource,
                            const std::string & fragmentSource);

        bool loadFromSource(const char * vertexSource, const char * fragmentSource);

        bool loadFromPath(const std::string & vertexPath,
                          const std::string & fragmentPath);

        void bind();

        void unbind();

        GLuint uniformLocation(const std::string & name) const;

        void setBool(const std::string & name, bool value) const;

        void setInt(const std::string & name, int value) const;
        void setUInt(const std::string & name, unsigned int value) const;

        void setFloat(const std::string & name, float value) const;

        void setVec2(const std::string & name, const glm::vec2 & value) const;
        void setVec3(const std::string & name, const glm::vec3 & value) const;
        void setVec4(const std::string & name, const glm::vec4 & value) const;

        void setMat2(const std::string & name, const glm::mat2 & value) const;
        void setMat3(const std::string & name, const glm::mat3 & value) const;
        void setMat4(const std::string & name, const glm::mat4 & value) const;

        static Shader::Ptr defaultShader();
    };
}
