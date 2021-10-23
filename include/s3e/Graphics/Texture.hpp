#pragma once

#include <GL/glew.h>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <memory>

namespace Tom::s3e {

    /**
     * Manages a single OpenGL texture.
     */
    class Texture {
        GLuint textureId;
        sf::Vector2u size;
        GLint internal;
        GLenum format;
        GLenum type;
        GLsizei samples;
        GLenum target;

        GLint magFilter, minFilter;
        GLint wrap;
        bool mipmaps;

    public:
        using Ptr = std::shared_ptr<Texture>;
        using ConstPtr = std::shared_ptr<const Texture>;

        /**
         * Create a texture from an image.
         *
         * @param magFilter the magnification filter (default GL_NEAREST)
         * @param minFilter the minification filter (default GL_NEAREST)
         * @param wrap the wrap mode when drawing
         * @param mipmaps should mipmaps be generated
         */
        Texture(const sf::Image & image,
                GLint magFilter = GL_LINEAR,
                GLint minFilter = GL_LINEAR_MIPMAP_LINEAR,
                GLint wrap = GL_REPEAT,
                bool mipmaps = true);

        /**
         * Create an empty texture of the specified size.
         *
         * Note: if samples is > 0, the filter, wrap and mipmaps arguments are
         * ignored.
         *
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
         * @param mipmaps should mipmaps be generated
         */
        Texture(const sf::Vector2u & size,
                GLint internal = GL_RGBA,
                GLenum format = GL_RGBA,
                GLenum type = GL_FLOAT,
                GLsizei samples = 0,
                GLint magFilter = GL_LINEAR,
                GLint minFilter = GL_LINEAR_MIPMAP_LINEAR,
                GLint wrap = GL_REPEAT,
                bool mipmaps = true);

        /// Free all opengl resources
        virtual ~Texture();

        /**
         * Load the texture from an image, setting the size to match
         * image.getSize().
         *
         * @param image the image to load from
         */
        void loadFrom(const sf::Image & image);

        /**
         * Get the OpenGL texture id.
         *
         * @return the texture id
         */
        GLuint getTextureId() const;

        /**
         * Get the texture size
         *
         * @return the texture size
         */
        const sf::Vector2u & getSize() const;

        /**
         * Resize the texture clearing all data.
         *
         * @param size the new texture size
         */
        void resize(const sf::Vector2u & size);

        /**
         * Bind the texture.
         */
        void bind() const;

        /**
         * Unbind the texture, effectively binding 0.
         */
        void unbind() const;
    };
}
