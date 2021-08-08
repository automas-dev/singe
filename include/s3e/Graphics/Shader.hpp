#pragma once

#include <GL/glew.h>

#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
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
        bool loadFromSource(const std::string_view & vertexSource,
                            const std::string_view & fragmentSource);

        /**
         * Load and compile the shader from the given paths.
         *
         * @param vertexPath path to the vertex shader source
         * @param fragmentPath path to the fragment shader source
         *
         * @return was the shader compiled successfully
         */
        bool loadFromPath(const std::string_view & vertexPath,
                          const std::string_view & fragmentPath);

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
        GLuint uniformLocation(const std::string_view & name) const;

        /**
         * Get the uniform location for name and set the value to integer 0 or 1.
         *
         * @param name the uniform name
         * @param value the value to set
         */
        void setBool(const std::string_view & name, bool value) const;

        /**
         * Set the value to integer 0 or 1.
         *
         * @param location the uniform location
         * @param value the value to set
         */
        void setBool(GLuint location, bool value) const;

        /**
         * Get the uniform location for name and set the value.
         *
         * @param name the uniform name
         * @param value the value to set
         */
        void setInt(const std::string_view & name, int value) const;

        /**
         * Set the value.
         *
         * @param location the uniform location
         * @param value the value to set
         */
        void setInt(GLuint location, int value) const;

        /**
         * Get the uniform location for name and set the value.
         *
         * @param name the uniform name
         * @param value the value to set
         */
        void setUInt(const std::string_view & name, unsigned int value) const;

        /**
         * Set the value.
         *
         * @param location the uniform location
         * @param value the value to set
         */
        void setUInt(GLuint location, unsigned int value) const;

        /**
         * Get the uniform location for name and set the value.
         *
         * @param name the uniform name
         * @param value the value to set
         */
        void setFloat(const std::string_view & name, float value) const;

        /**
         * Set the value.
         *
         * @param location the uniform location
         * @param value the value to set
         */
        void setFloat(GLuint location, float value) const;

        /**
         * Get the uniform location for name and set the value.
         *
         * @param name the uniform name
         * @param value the value to set
         */
        void setVec2(const std::string_view & name, const glm::vec2 & value) const;

        /**
         * Set the value.
         *
         * @param location the uniform location
         * @param value the value to set
         */
        void setVec2(GLuint location, const glm::vec2 & value) const;

        /**
         * Get the uniform location for name and set the value.
         *
         * @param name the uniform name
         * @param value the value to set
         */
        void setVec3(const std::string_view & name, const glm::vec3 & value) const;

        /**
         * Set the value.
         *
         * @param location the uniform location
         * @param value the value to set
         */
        void setVec3(GLuint location, const glm::vec3 & value) const;

        /**
         * Get the uniform location for name and set the value.
         *
         * @param name the uniform name
         * @param value the value to set
         */
        void setVec4(const std::string_view & name, const glm::vec4 & value) const;

        /**
         * Set the value.
         *
         * @param location the uniform location
         * @param value the value to set
         */
        void setVec4(GLuint location, const glm::vec4 & value) const;

        /**
         * Get the uniform location for name and set the value.
         *
         * @param name the uniform name
         * @param value the value to set
         */
        void setMat2(const std::string_view & name, const glm::mat2 & value) const;

        /**
         * Set the value.
         *
         * @param location the uniform location
         * @param value the value to set
         */
        void setMat2(GLuint location, const glm::mat2 & value) const;

        /**
         * Get the uniform location for name and set the value.
         *
         * @param name the uniform name
         * @param value the value to set
         */
        void setMat3(const std::string_view & name, const glm::mat3 & value) const;

        /**
         * Set the value.
         *
         * @param location the uniform location
         * @param value the value to set
         */
        void setMat3(GLuint location, const glm::mat3 & value) const;

        /**
         * Get the uniform location for name and set the value.
         *
         * @param name the uniform name
         * @param value the value to set
         */
        void setMat4(const std::string_view & name, const glm::mat4 & value) const;

        /**
         * Set the value.
         *
         * @param location the uniform location
         * @param value the value to set
         */
        void setMat4(GLuint location, const glm::mat4 & value) const;

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
