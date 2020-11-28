#pragma once

#include <SFML/Window.hpp>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <memory>
#include <vector>
#include "Texture.hpp"

namespace Tom::s3e {

    class FrameBufferTexture : public Texture {
        GLuint attachment;
    public:
        typedef std::shared_ptr<FrameBufferTexture> Ptr;
        typedef std::shared_ptr<const FrameBufferTexture> ConstPtr;

        FrameBufferTexture(void);
        FrameBufferTexture(GLuint attachment, sf::Vector2u size, GLint internal = GL_RGBA, GLenum format = GL_RGBA, GLenum type = GL_FLOAT,
                GLint magFilter = GL_NEAREST, GLint minFilter = GL_NEAREST, GLint wrap = GL_REPEAT, bool mipmaps = true);
        virtual ~FrameBufferTexture();
    
        GLuint getAttachment(void) const;

        void setAttachment(GLuint attachment);
    };

    class FrameBuffer {
        GLuint fboId;
        sf::Vector2u size;
        std::vector<FrameBufferTexture::Ptr> textures;

    public:
        typedef std::shared_ptr<FrameBuffer> Ptr;
        typedef std::shared_ptr<const FrameBuffer> ConstPtr;

        FrameBuffer(void);
        FrameBuffer(sf::Vector2u size);
        virtual ~FrameBuffer();

        GLuint getId(void) const;

        const sf::Vector2u & getSize(void) const;

        void setSize(sf::Vector2u size);

        void addTexture(GLenum attachment, GLint internal = GL_RGBA, GLenum format = GL_RGBA, GLenum type = GL_FLOAT);

        size_t count(void) const;

        const std::vector<FrameBufferTexture::Ptr> & getTextures(void) const;

        void blit(GLint src, GLint dest, GLbitfield bitfield=GL_COLOR_BUFFER_BIT);

        void bind(void);
        
        void unbind(void);
    };
}

