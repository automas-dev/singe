#pragma once

#include <SFML/Window.hpp>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

// https://www.khronos.org/opengl/wiki/Shader_Compilation

namespace game {

    void draw_color_array(const float *vertices,
                          const float *colors,
                          size_t n,
                          GLenum mode);

    std::string shaderSource(const std::string & path);

    class Shader : public std::enable_shared_from_this<Shader> {
        GLuint program;
        std::string error;
        bool didFail;

    public:
        typedef std::shared_ptr<Shader> Ptr;
        typedef std::shared_ptr<const Shader> ConstPtr;

        Shader(void);
        Shader(const std::string & vertexSource,
               const std::string & fragmentSource);
        virtual ~Shader();

        bool failed(void) const { return didFail; }
        const std::string &getError() const { return error; }

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
