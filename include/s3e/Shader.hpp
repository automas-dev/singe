#pragma once

#include <SFML/Window.hpp>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

// https://www.khronos.org/opengl/wiki/Shader_Compilation

namespace Tom::s3e {

    void draw_color_array_legacy(const float *vertices,
                                 const float *colors,
                                 size_t n,
                                 GLenum mode);

    void draw_color_array(const std::vector<glm::vec3> & vertices,
                          const std::vector<glm::vec3> & colors,
                          GLenum mode);

    void draw_tex_array(const std::vector<glm::vec3> & vertices,
                        const std::vector<glm::vec2> & uvs,
                        GLenum mode);

    class Texture {
        GLuint textureId;

    public:
        typedef std::shared_ptr<Texture> Ptr;
        typedef std::shared_ptr<const Texture> ConstPtr;

        Texture(void);
        virtual ~Texture();

        bool loadFromPath(const std::string & path);

        void bind(void);

        void unbind(void);

        static Ptr create(const std::string & path);
    };

    std::string shaderSource(const std::string & path);

    class Shader {
        GLuint program;

    public:
        typedef std::shared_ptr<Shader> Ptr;
        typedef std::shared_ptr<const Shader> ConstPtr;

        Shader(void);
        virtual ~Shader();

        bool loadFromSource(const std::string & vertexSource,
                            const std::string & fragmentSource);

        bool loadFromPath(const std::string & vertexPath,
                          const std::string & fragmentPath);

        void bind(void);

        void unbind(void);

        GLuint uniformLocation(const std::string & name) const;

        void setBool(const std::string &name, bool value) const;
        
        void setInt(const std::string &name, int value) const;
        void setUInt(const std::string &name, unsigned int value) const;
        
        void setFloat(const std::string &name, float value) const;
        
        void setVec2(const std::string &name, const glm::vec2 &value) const;
        void setVec3(const std::string &name, const glm::vec3 &value) const;
        void setVec4(const std::string &name, const glm::vec4 &value) const;

        void setMat2(const std::string &name, const glm::mat2 &value) const;
        void setMat3(const std::string &name, const glm::mat3 &value) const;
        void setMat4(const std::string &name, const glm::mat4 &value) const;

        static Ptr create(const std::string & vertexPath,
                          const std::string & fragmentPath);
    };
};
