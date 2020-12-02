#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <memory>

namespace Tom::s3e {

    class Texture {
        GLuint textureId;
        sf::Vector2u size;
        GLint internal;
        GLenum format;
        GLenum type;

        GLint magFilter, minFilter;
        GLint wrap;
        bool mipmaps;
        bool multisample;
        int samples;

        std::unique_ptr<sf::Image> image;

        void realloc();

    public:
        typedef std::shared_ptr<Texture> Ptr;
        typedef std::shared_ptr<const Texture> ConstPtr;

        Texture(void);
        Texture(sf::Vector2u size, GLint internal = GL_RGBA, GLenum format = GL_RGBA, GLenum type = GL_FLOAT,
                GLint magFilter = GL_NEAREST, GLint minFilter = GL_NEAREST, GLint wrap = GL_REPEAT, bool mipmaps = true,
                bool multisample = false, int samples = 4);
        virtual ~Texture();

        bool loadFromPath(const std::string & path);

        GLuint getTextureId(void) const;

        const sf::Vector2u & getSize(void) const;

        void setSize(sf::Vector2u newSize);

        void setFilter(GLint magFilter, GLint minFilter);

        void setWrap(GLint wrap);

        void bind(void);

        void unbind(void);
    };
}

