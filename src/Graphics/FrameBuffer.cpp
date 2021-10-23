#include "singe/Graphics/FrameBuffer.hpp"

#include "singe/Support/log.hpp"

namespace singe {
    FrameBufferTexture::FrameBufferTexture()
        : FrameBufferTexture(GL_COLOR_ATTACHMENT0, {0, 0}) {}

    FrameBufferTexture::FrameBufferTexture(GLuint attachment,
                                           const sf::Vector2u & size,
                                           GLint internal,
                                           GLenum format,
                                           GLenum type,
                                           GLsizei samples,
                                           GLint magFilter,
                                           GLint minFilter,
                                           GLint wrap)
        : Texture(
            size, internal, format, type, samples, magFilter, minFilter, wrap, false),
          attachment(attachment) {

        Logging::Graphics->debug("Created FrameBufferTexture {}: attachment={}",
                                 getTextureId(), attachment);
    }

    FrameBufferTexture::~FrameBufferTexture() {
        Texture::~Texture();
    }

    GLuint FrameBufferTexture::getAttachment() const {
        return attachment;
    }

    void FrameBufferTexture::setAttachment(GLuint attachment) {
        Logging::Graphics->debug("Setting FrameBufferTexture {}: attachment={}",
                                 getTextureId(), attachment);
        this->attachment = attachment;
    }
}

namespace singe {
    FrameBuffer::FrameBuffer(const sf::Vector2u & size,
                             std::vector<FrameBufferAttachment> attachments,
                             bool depthBuffer,
                             GLsizei samples)
        : size(size), rboDepth(0), samples(samples), resolved(nullptr) {

        glGenFramebuffers(1, &fboId);
        Logging::Graphics->debug(
            "Created FrameBuffer {}: size={}x{} attachments={} depthBuffer={} samples={}",
            fboId, size.x, size.y, attachments.size(), depthBuffer, samples);

        bind();

        for (auto & a : attachments) {
            auto texture = std::make_shared<FrameBufferTexture>(
                a.attachment, size, a.internal, a.format, a.type, samples,
                GL_NEAREST, GL_NEAREST, GL_CLAMP);
            GLenum textarget =
                (samples > 0 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D);
            glFramebufferTexture2D(GL_FRAMEBUFFER, a.attachment, textarget,
                                   texture->getTextureId(), 0);
            textures.push_back(texture);
        }

        if (depthBuffer) {
            glGenRenderbuffers(1, &rboDepth);
            glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);

            if (samples > 0)
                glRenderbufferStorageMultisample(
                    GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT, size.x, size.y);
            else
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
                                      size.x, size.y);

            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                      GL_RENDERBUFFER, rboDepth);
        }

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            Logging::Graphics->error("FrameBuffer is not complete");

        std::vector<GLuint> buffers;
        for (int i = 0; i < count(); i++) {
            auto a = textures[i]->getAttachment();
            if (a != GL_DEPTH_ATTACHMENT && a != GL_STENCIL_ATTACHMENT)
                buffers.push_back(a);
        }
        glDrawBuffers(buffers.size(), &buffers[0]);

        unbind();

        if (samples > 0)
            resolved =
                std::make_shared<FrameBuffer>(size, attachments, depthBuffer, 0);
    }

    FrameBuffer::~FrameBuffer() {
        Logging::Graphics->debug("Deleting FrameBuffer {}", fboId);

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

    void FrameBuffer::setSize(const sf::Vector2u & size) {
        Logging::Graphics->debug("FrameBuffer set size to {}x{}", size.x, size.y);

        this->size = size;
        for (auto & texture : textures) {
            texture->resize(size);
        }

        if (rboDepth > 0) {
            glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
            if (samples > 0)
                glRenderbufferStorageMultisample(
                    GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT, size.x, size.y);
            else
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
                                      size.x, size.y);
        }

        if (resolved)
            resolved->setSize(size);
    }

    const std::vector<FrameBufferTexture::Ptr> & FrameBuffer::getTextures() const {
        return textures;
    }

    bool FrameBuffer::isMultisampled() const {
        return samples > 0;
    }

    GLsizei FrameBuffer::getSamples() const {
        return samples;
    }

    size_t FrameBuffer::count() const {
        return textures.size();
    }

    void FrameBuffer::blit(GLint dest, GLbitfield bitfield) const {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, fboId);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dest);
        if (count() > 1) {
            for (int i = 0; i < count(); i++) {
                auto a = textures[i]->getAttachment();
                if (a != GL_DEPTH_ATTACHMENT && a != GL_STENCIL_ATTACHMENT) {
                    glReadBuffer(a);
                    glDrawBuffer(a);
                    glBlitFramebuffer(0, 0, size.x, size.y, 0, 0, size.x,
                                      size.y, bitfield, GL_NEAREST);
                }
            }
        }
        else
            glBlitFramebuffer(0, 0, size.x, size.y, 0, 0, size.x, size.y,
                              bitfield, GL_NEAREST);
    }

    const FrameBuffer::Ptr & FrameBuffer::getResolved() const {
        if (resolved)
            blit(resolved->getId(), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT
                                        | GL_STENCIL_BUFFER_BIT);
        return resolved;
    }

    void FrameBuffer::bind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, fboId);
    }

    void FrameBuffer::unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }
}
