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

    std::string shaderSource(const std::string &path);

    class Shader {
        GLuint program;

    public:
        Shader(const std::string &vertexSource,
               const std::string &fragmentSource);
        ~Shader() {}

        GLuint uniformLocation(const char *name);

        void bind();

        void unbind();
    };

    std::shared_ptr<Shader> getShader(const char *vPath, const char *fPath);
};
