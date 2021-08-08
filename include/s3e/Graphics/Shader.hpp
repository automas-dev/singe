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

        /**
         * Create a new Shader with no program.
         */
        Shader();

        /// Free OpenGL resources
        virtual ~Shader();

        /**
         * Load and compile the shader from the source code.
         *
         * @param vertexSource the vertex shader source
         * @param fragmentSource the fragment shader source
         *
         * @return was the shader compiled successfully
         */
        bool loadFromSource(const std::string & vertexSource,
                            const std::string & fragmentSource);

        /**
         * Load and compile the shader from the source code.
         *
         * @param vertexSource the vertex shader source
         * @param fragmentSource the fragment shader source
         *
         * @return was the shader compiled successfully
         */
        bool loadFromSource(const char * vertexSource, const char * fragmentSource);

        /**
         * Load and compile the shader from the given paths.
         *
         * @param vertexPath path to the vertex shader source
         * @param fragmentPath path to the fragment shader source
         *
         * @return was the shader compiled successfully
         */
        bool loadFromPath(const std::string & vertexPath,
                          const std::string & fragmentPath);

        /**
         * Get the OpenGL shader program id.
         *
         * @return the shader program id
         */
        GLuint getProgram() const;

        /**
         * Bind the shader.
         */
        void bind() const;

        /**
         * Unbind the shader, effectively binding 0.
         */
        void unbind() const;

        /**
         * Get the uniform location for name in this shader.
         *
         * @param name the uniform name
         *
         * @return the uniform location
         */
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

        /**
         * Load the default shader from the internal source.
         *
         * This shader takes a Vertex from VBO.hpp, two 4x4 matrices mvp and
         * model, and a texture.
         *
         * @return the default shader
         */
        static Shader::Ptr defaultShader();
    };
}
