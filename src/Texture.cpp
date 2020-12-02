#include "s3e/Texture.hpp"
#include <SFML/Graphics/Image.hpp>
#include "s3e/log.hpp"

namespace Tom::s3e {

    void Texture::realloc() {
        SPDLOG_DEBUG("updating opengl texture {} to size {} x {} magFilter = {} minFilter = {} wrap = {} mipmaps = {}",
                     textureId, size.x, size.y, magFilter, minFilter, wrap, mipmaps);

        GLenum target;

        if (multisample) {
            target = GL_TEXTURE_2D_MULTISAMPLE;
            glBindTexture(target, textureId);
            glTexImage2DMultisample(target, samples, internal, size.x, size.y, GL_TRUE);
        }
        else {
            target = GL_TEXTURE_2D;
            glBindTexture(target, textureId);
            glTexImage2D(target, 0, internal, size.x, size.y, 0, format, type, (image ? image->getPixelsPtr() : NULL));
        }

        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);

        glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap);

        if (!multisample && mipmaps)
            glGenerateMipmap(GL_TEXTURE_2D);
    }

    Texture::Texture() : Texture({0, 0}) { }

    Texture::Texture(sf::Vector2u size, GLint internal, GLenum format, GLenum type, GLint magFilter, GLint minFilter,
                     GLint wrap, bool mipmaps, bool multisample, int samples) : textureId(0), size(size),
        internal(internal), format(format), type(type), minFilter(minFilter), magFilter(magFilter), wrap(wrap),
        mipmaps(mipmaps), multisample(multisample), samples(samples) {
        glGenTextures(1, &textureId);
        SPDLOG_DEBUG("generated opengl texture: {}", textureId);
        if (size.x > 0 && size.y > 0)
            realloc();
    }

    Texture::~Texture() {
        if (textureId > 0) {
            SPDLOG_DEBUG("deleting opengl texture: {}", textureId);
            glDeleteTextures(1, &textureId);
        }
    }

    bool Texture::loadFromPath(const std::string & path) {
        SPDLOG_DEBUG("loading texture from path: {}", path);

        image = std::make_unique<sf::Image>();
        if (!image->loadFromFile(path)) {
            SPDLOG_WARN("Texture failed to open file {}", path);
            return false;
        }

        type = GL_UNSIGNED_BYTE;
        setSize(image->getSize());  // Calls realloc

        return true;
    }

    GLuint Texture::getTextureId() const {
        return textureId;
    }

    const sf::Vector2u & Texture::getSize() const {
        return size;
    }

    void Texture::setSize(sf::Vector2u newSize) {
        size = newSize;
        realloc();
    }

    void Texture::setFilter(GLint magFilter, GLint minFilter) {
        this->magFilter = magFilter;
        this->minFilter = minFilter;
        realloc();
    }

    void Texture::setWrap(GLint wrap) {
        this->wrap = wrap;
        realloc();
    }

    void Texture::bind() {
        SPDLOG_TRACE("bind opengl texture: {}", textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
    }

    void Texture::unbind() {
        SPDLOG_TRACE("bind opengl texture: 0 (unbind)");
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

