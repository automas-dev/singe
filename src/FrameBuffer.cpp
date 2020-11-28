#include "s3e/FrameBuffer.hpp"
#include "s3e/log.hpp"

namespace Tom::s3e {
    FrameBufferTexture::FrameBufferTexture() : FrameBufferTexture(GL_COLOR_ATTACHMENT0, {
        0, 0
    }) { }

    FrameBufferTexture::FrameBufferTexture(GLuint attachment, sf::Vector2u size, GLint internal, GLenum format, GLenum type,
                                           GLint magFilter, GLint minFilter, GLint wrap, bool mipmaps) : Texture(size, internal, format, type, magFilter,
                                                       minFilter, wrap, mipmaps), attachment(attachment) { }

    GLuint FrameBufferTexture::getAttachment() const {
        return attachment;
    }

    void FrameBufferTexture::setAttachment(GLuint attachment) {
        this->attachment = attachment;
    }
}

namespace Tom::s3e {
    FrameBuffer::FrameBuffer() : FrameBuffer({0, 0}) { }

    FrameBuffer::FrameBuffer(sf::Vector2u size) : size(size) {
        glGenFramebuffers(1, &fboId);
    }

    FrameBuffer::~FrameBuffer() { }

    GLuint FrameBuffer::getId() const {
        return fboId;
    }

    const sf::Vector2u & FrameBuffer::getSize() const {
        return size;
    }

    void FrameBuffer::setSize(sf::Vector2u size) {
        this->size = size;
    }

    void FrameBuffer::addTexture(GLenum attachment, GLint internal, GLenum format, GLenum type) {
        bind();

        auto texture = std::make_shared<FrameBufferTexture>(attachment, size, internal, format, type, GL_NEAREST, GL_NEAREST,
                       GL_REPEAT);
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture->getTextureId(), 0);
        textures.push_back(texture);

        std::vector<GLuint> buffers(count());
        for (int i = 0; i < count(); i++)
            buffers[i] = textures[i]->getAttachment();
        glDrawBuffers(count(), &buffers[0]);

        unbind();
    }
    
    const std::vector<FrameBufferTexture::Ptr> & FrameBuffer::getTextures() const {
        return textures;
    }

    size_t FrameBuffer::count() const {
        return textures.size();
    }

    void FrameBuffer::bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, fboId);
    }

    void FrameBuffer::unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

