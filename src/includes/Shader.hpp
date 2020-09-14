#pragma once

#include <SFML/Window.hpp>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

// https://www.khronos.org/opengl/wiki/Shader_Compilation

namespace GameLib {

    void draw_color_array(const float *vertices,
                          const float *colors,
                          size_t n,
                          GLenum mode);

    class shader_compile_error : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };

    class shader_link_error : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };

    std::string shaderSource(std::string const& path);

    class Shader {
        GLuint program;

    public:
        Shader(std::string const & vertexSource,
               std::string const & fragmentSource);
        ~Shader() {}

        GLuint uniformLocation(std::string const & name);

        void bind();

        void unbind();
    };

    std::shared_ptr<Shader> getShader(std::string const & vPath, std::string const & fPath);
};
