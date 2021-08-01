#pragma once

#include <GL/glew.h>

#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <memory>
#include <vector>

#include "Texture.hpp"

namespace Tom::s3e {

    /**
     * Manages a texture used in a `FrameBufferObject`.
     */
    class FrameBufferTexture : public Texture {
        GLuint attachment;

    public:
        /**
         * A shared pointer that manages a FrameBufferTexture.
         */
        typedef std::shared_ptr<FrameBufferTexture> Ptr;

        /**
         * A shared pointer that manages a const FrameBufferTexture.
         */
        typedef std::shared_ptr<const FrameBufferTexture> ConstPtr;

        /**
         * Construct a new empty FrameBufferTexture.
         */
        FrameBufferTexture(void);

        /**
         * Construct a new FrameBufferTexture and generate a 2d texture.
         *
         * @param attachment the frame buffer attachment like
         * GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT, etc.
         * @param size the image size in pixels
         * @param internal the internal format like GL_RGBA16F, GL_RGBA,
         * GL_DEPTH_COMPONENT, etc.
         * @param format the format of pixel data
         * @param type the data type of pixel data
         * @param magFilter the magnification filter (default GL_NEAREST)
         * @param minFilter the minification filter (default GL_NEAREST)
         * @param wrap the wrap mode when drawing
         * @param samples the number of samples to use, 0 to disable
         *                multisampling
         */
        FrameBufferTexture(GLuint attachment,
                           sf::Vector2u size,
                           GLint internal = GL_RGBA,
                           GLenum format = GL_RGBA,
                           GLenum type = GL_FLOAT,
                           GLint magFilter = GL_NEAREST,
                           GLint minFilter = GL_NEAREST,
                           GLint wrap = GL_CLAMP,
                           GLsizei samples = 0);

        /**
         * Destruct the FrameBufferTexture.
         */
        virtual ~FrameBufferTexture();

        /**
         * Get the attachment GLenum used in glFramebufferTexture2D.
         *
         * @return the attachment GLenum
         */
        GLuint getAttachment(void) const;

        /**
         * Set the attachment GLenum used in glFramebufferTexture2D.
         *
         * @param attachment the new attachment GLenum
         */
        void setAttachment(GLuint attachment);
    };

    struct FrameBufferAttachment {
        GLenum attachment;
        GLint internal = GL_RGBA;
        GLenum format = GL_RGBA;
        GLenum type = GL_FLOAT;

        FrameBufferAttachment(GLenum attachment,
                              GLint internal = GL_RGBA,
                              GLenum format = GL_RGBA,
                              GLenum type = GL_FLOAT)
            : attachment(attachment),
              internal(internal),
              format(format),
              type(type) {}
    };

    /**
     * Manage a single frame buffer object.
     */
    class FrameBuffer {
        GLuint fboId;
        sf::Vector2u size;
        std::vector<FrameBufferTexture::Ptr> textures;
        GLuint rboDepth;
        GLsizei samples;

        std::shared_ptr<FrameBuffer> resolved;

    public:
        /**
         * A shared pointer that manages a FrameBuffer.
         */
        typedef std::shared_ptr<FrameBuffer> Ptr;

        /**
         * A shared pointer that manages a const FrameBuffer.
         */
        typedef std::shared_ptr<const FrameBuffer> ConstPtr;

        /**
         * Construct a new FrameBuffer with the given size.
         *
         * @param size the size in pixels
         * @param attachments a list of texture attachments for this frame buffer
         * @param depthBuffer should a render buffer be crated for depth
         * @param samples the number of samples to use, 0 to disable
         *                multisampling
         */
        FrameBuffer(sf::Vector2u size,
                    std::vector<FrameBufferAttachment> attachments,
                    bool depthBuffer = true,
                    GLsizei samples = 0);

        /**
         * Destruct the FrameBuffer.
         */
        virtual ~FrameBuffer();

        /**
         * Get the FrameBuffer id.
         *
         * @return the opengl id
         */
        GLuint getId(void) const;

        /**
         * Get the current size in pixels.
         *
         * @return the current size
         */
        const sf::Vector2u & getSize(void) const;

        /**
         * Set the size in pixels. This will also resize each
         * FrameBufferTexture under this FrameBuffer.
         *
         * @param size the new size in pixels
         */
        void setSize(sf::Vector2u size);

        /**
         * Does the FrameBuffer support multisampling. If so, you need to use
         * the FrameBuffer from getResolved() for any sampleing.
         */
        bool isMultisampled(void);

        /**
         * Get the number of FrameBufferTexture textures in the FrameBuffer.
         *
         * @return the number of FrameBufferTexture textures
         */
        size_t count(void) const;

        /**
         * Get the FrameBufferTexture textures.
         *
         * @return the textures
         */
        const std::vector<FrameBufferTexture::Ptr> & getTextures(void) const;

        /**
         * Call glBlitFramebuffer with the FrameBuffer as the source and dest
         * as the destination. Use bitfield to select color, depth or stencil.
         *
         * @param dest the destination FrameBuffer id or 0 for the default
         * @param bitfield the buffers to copy (GL_COLOR_BUFFER_BIT,
         * GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT)
         */
        void blit(GLint dest, GLbitfield bitfield = GL_COLOR_BUFFER_BIT);

        /**
         * Get the resolved FrameBuffer. If this FrameBuffer is multisampled,
         * the resolved FrameBuffer will be an identical FrameBuffer that is
         * not multisampled.
         *
         * @returns the resolved FrameBuffer
         */
        FrameBuffer::Ptr & getResolved(void);

        /**
         * Bind the FrameBuffer. All draw calls after this will be sent to the
         * FrameBuffer.
         */
        void bind(void);

        /**
         * Bind the default frame buffer, effectively unbinding this
         * FrameBuffer.
         */
        void unbind(void);
    };
}
