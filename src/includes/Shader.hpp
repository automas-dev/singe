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

    std::string shaderSource(const std::string & path);

    class Shader {
        GLuint program;

    public:
        typedef std::shared_ptr<Shader> Ptr;
        typedef std::shared_ptr<const Shader> ConstPtr;

        Shader(void);
        Shader(const std::string & vertexSource,
               const std::string & fragmentSource);
        virtual ~Shader();

        bool loadFromSource(const std::string & vertexSource,
                          const std::string & fragmentSource);

        bool loadFromPath(const std::string & vertexPath,
                          const std::string & fragmentPath);

        GLuint uniformLocation(const std::string & name);

        void bind(void);

        void unbind(void);

        static Ptr create(const std::string & vertexPath,
                          const std::string & fragmentPath);
    };
};
