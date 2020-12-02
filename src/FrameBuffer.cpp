#include "s3e/FrameBuffer.hpp"
#include "s3e/log.hpp"

namespace Tom::s3e {
    FrameBufferTexture::FrameBufferTexture() : FrameBufferTexture(GL_COLOR_ATTACHMENT0, {
        0, 0
    }) { }

    FrameBufferTexture::FrameBufferTexture(GLuint attachment, sf::Vector2u size, GLint internal, GLenum format, GLenum type,
                                           GLint magFilter, GLint minFilter, GLint wrap, bool multisample) : Texture(size, internal, format, type, magFilter,
                                                       minFilter, wrap, multisample), attachment(attachment) { }

    FrameBufferTexture::~FrameBufferTexture() {
        Texture::~Texture();
    }

    GLuint FrameBufferTexture::getAttachment() const {
        return attachment;
    }

    void FrameBufferTexture::setAttachment(GLuint attachment) {
        this->attachment = attachment;
    }
}

namespace Tom::s3e {
    FrameBuffer::FrameBuffer() : FrameBuffer({0, 0}) { }

    FrameBuffer::FrameBuffer(sf::Vector2u size) : size(size), rboDepth(0) {
        glGenFramebuffers(1, &fboId);
    }

    FrameBuffer::~FrameBuffer() {
        glDeleteFramebuffers(1, &fboId);
        if (rboDepth > 0)
            glDeleteRenderbuffers(1, &rboDepth);
    }

    GLuint FrameBuffer::getId() const {
        return fboId;
    }

    const sf::Vector2u & FrameBuffer::getSize() const {
        return size;
    }

    void FrameBuffer::setSize(sf::Vector2u size) {
        this->size = size;
        for (auto & texture : textures) {
            texture->setSize(size);
        }
    }

    void FrameBuffer::addMultisampleTexture(GLenum attachment, GLint internal, GLenum format, GLenum type) {
        bind();

        auto texture = std::make_shared<FrameBufferTexture>(attachment, size, internal, format, type, GL_NEAREST, GL_NEAREST,
                       GL_CLAMP, true);
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D_MULTISAMPLE, texture->getTextureId(), 0);
        textures.push_back(texture);

        unbind();
    }

    void FrameBuffer::addTexture(GLenum attachment, GLint internal, GLenum format, GLenum type) {
        bind();

        auto texture = std::make_shared<FrameBufferTexture>(attachment, size, internal, format, type, GL_NEAREST, GL_NEAREST,
                       GL_CLAMP, false);
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture->getTextureId(), 0);
        textures.push_back(texture);

        unbind();
    }

    void FrameBuffer::enableDepthBuffer() {
        bind();

        glGenRenderbuffers(1, &rboDepth);
        glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.x, size.y);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

        unbind();
    }

    void FrameBuffer::finalize() {
        bind();
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            SPDLOG_ERROR("FrameBuffer is not complete");

        std::vector<GLuint> buffers;
        for (int i = 0; i < count(); i++) {
            auto a = textures[i]->getAttachment();
            if (a != GL_DEPTH_ATTACHMENT && a != GL_STENCIL_ATTACHMENT)
                buffers.push_back(a);
        }
        glDrawBuffers(buffers.size(), &buffers[0]);

        unbind();
    }

    const std::vector<FrameBufferTexture::Ptr> & FrameBuffer::getTextures() const {
        return textures;
    }

    size_t FrameBuffer::count() const {
        return textures.size();
    }

    void FrameBuffer::blit(GLint dest, GLbitfield bitfield) {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, fboId);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dest);
        glBlitFramebuffer(0, 0, size.x, size.y, 0, 0, size.x, size.y, bitfield, GL_NEAREST);
    }

    void FrameBuffer::bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, fboId);
    }

    void FrameBuffer::unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

