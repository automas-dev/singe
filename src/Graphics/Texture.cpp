#include "s3e/Graphics/Texture.hpp"

#include <SFML/Graphics/Image.hpp>

#include "s3e/Support/log.hpp"

namespace Tom::s3e {

    void Texture::realloc() {
        Logging::Graphics->debug(
            "updating texture {} to size {}x{} intermal={} format={} type={} magFilter={} minFilter={} wrap={} mipmaps={}",
            textureId, size.x, size.y, internal, format, type, magFilter,
            minFilter, wrap, mipmaps);

        if (samples > 0) {
            glBindTexture(target, textureId);
            glTexImage2DMultisample(target, samples, internal, size.x, size.y,
                                    GL_TRUE);
        }
        else {
            glBindTexture(target, textureId);
            glTexImage2D(target, 0, internal, size.x, size.y, 0, format, type,
                         (image ? image->getPixelsPtr() : NULL));

            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);
            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);

            glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap);
            glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap);

            if (mipmaps)
                glGenerateMipmap(target);
        }
        glBindTexture(target, 0);
    }

    Texture::Texture() : Texture({0, 0}) {}

    Texture::Texture(sf::Vector2u size,
                     GLint internal,
                     GLenum format,
                     GLenum type,
                     GLint magFilter,
                     GLint minFilter,
                     GLint wrap,
                     bool mipmaps,
                     GLsizei samples)
        : textureId(0),
          size(size),
          internal(internal),
          format(format),
          type(type),
          minFilter(minFilter),
          magFilter(magFilter),
          wrap(wrap),
          mipmaps(mipmaps),
          samples(samples) {

        target = (samples > 0 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D);

        glGenTextures(1, &textureId);
        Logging::Graphics->debug("generated opengl texture: {} samples = {}",
                             textureId, samples);
        if (size.x > 0 && size.y > 0)
            realloc();
    }

    Texture::~Texture() {
        if (textureId > 0) {
            Logging::Graphics->debug("deleting opengl texture: {}", textureId);
            glDeleteTextures(1, &textureId);
        }
    }

    bool Texture::loadFromPath(const std::string & path) {
        Logging::Graphics->debug("loading texture from path: {}", path);

        image = std::make_unique<sf::Image>();
        if (!image->loadFromFile(path)) {
            Logging::Graphics->warning("Texture failed to open file {}", path);
            return false;
        }

        type = GL_UNSIGNED_BYTE;
        setSize(image->getSize()); // Calls realloc

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
        glBindTexture(target, textureId);
    }

    void Texture::unbind() {
        glBindTexture(target, 0);
    }
}
