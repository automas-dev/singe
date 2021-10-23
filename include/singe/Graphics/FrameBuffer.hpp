#pragma once

#include <GL/glew.h>

#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <memory>
#include <vector>

#include "Texture.hpp"

namespace singe {

    /**
     * Manages a texture used in a `FrameBufferObject`.
     */
    class FrameBufferTexture : public Texture {
        GLuint attachment;

    public:
        using Ptr = std::shared_ptr<FrameBufferTexture>;
        using ConstPtr = std::shared_ptr<const FrameBufferTexture>;

        /**
         * Construct a new empty FrameBufferTexture.
         */
        FrameBufferTexture();

        /**
         * Construct a new FrameBufferTexture and generate a 2d texture.
         *
         * @param attachment the frame buffer attachment like
         *                   GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT, etc.
         * @param size the image size in pixels
         * @param internal the internal format like GL_RGBA16F, GL_RGBA,
         *                 GL_DEPTH_COMPONENT, etc.
         * @param format the format of pixel data
         * @param type the data type of pixel data
         * @param samples the number of samples to use, 0 to disable
         *                multisampling
         * @param magFilter the magnification filter (default GL_NEAREST)
         * @param minFilter the minification filter (default GL_NEAREST)
         * @param wrap the wrap mode when drawing
         */
        FrameBufferTexture(GLuint attachment,
                           const sf::Vector2u & size,
                           GLint internal = GL_RGBA,
                           GLenum format = GL_RGBA,
                           GLenum type = GL_FLOAT,
                           GLsizei samples = 0,
                           GLint magFilter = GL_NEAREST,
                           GLint minFilter = GL_NEAREST,
                           GLint wrap = GL_CLAMP);

        /**
         * Destruct the FrameBufferTexture.
         */
        virtual ~FrameBufferTexture();

        /**
         * Get the attachment GLenum used in glFramebufferTexture2D.
         *
         * @return the attachment GLenum
         */
        GLuint getAttachment() const;

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
        using Ptr = std::shared_ptr<FrameBuffer>;
        using ConstPtr = std::shared_ptr<const FrameBuffer>;

        /**
         * Construct a new FrameBuffer with the given size.
         *
         * @param size the size in pixels
         * @param attachments a list of texture attachments for this frame buffer
         * @param depthBuffer should a render buffer be crated for depth
         * @param samples the number of samples to use, 0 to disable
         *                multisampling
         */
        FrameBuffer(const sf::Vector2u & size,
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
        GLuint getId() const;

        /**
         * Get the current size in pixels.
         *
         * @return the current size
         */
        const sf::Vector2u & getSize() const;

        /**
         * Set the size in pixels. This will also resize each
         * FrameBufferTexture under this FrameBuffer.
         *
         * @param size the new size in pixels
         */
        void setSize(const sf::Vector2u & size);

        /**
         * Does the FrameBuffer support multisampling. If so, you need to use
         * the FrameBuffer from getResolved() for any sampleing.
         */
        bool isMultisampled() const;

        /**
         * Return the number of samples used for this FrameBuffer.
         *
         * If the number of samples is > 0, the FrameBuffer is considered to be
         * multisamples and must be resolved or handled by the multisample
         * texture bindings.
         *
         * @return the number of samples
         */
        GLsizei getSamples() const;

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
        const std::vector<FrameBufferTexture::Ptr> & getTextures() const;

        /**
         * Call glBlitFramebuffer with the FrameBuffer as the source and dest
         * as the destination. Use bitfield to select color, depth or stencil.
         *
         * @param dest the destination FrameBuffer id or 0 for the default
         * @param bitfield the buffers to copy (GL_COLOR_BUFFER_BIT,
         * GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT)
         */
        void blit(GLint dest, GLbitfield bitfield = GL_COLOR_BUFFER_BIT) const;

        /**
         * Get the resolved FrameBuffer. If this FrameBuffer is multisampled,
         * the resolved FrameBuffer will be an identical FrameBuffer that is
         * not multisampled.
         *
         * @returns the resolved FrameBuffer
         */
        const FrameBuffer::Ptr & getResolved() const;

        /**
         * Bind the FrameBuffer. All draw calls after this will be sent to the
         * FrameBuffer.
         */
        void bind() const;

        /**
         * Unbind this FrameBuffer, effectively binding the default FrameBuffer.
         */
        static void unbind();

        /**
         * Call glClear with Color, Depth and Stencil buffer bits.
         */
        static void clear();
    };
}
