#pragma once

#include <SFML/System.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <s3e/Core/Camera.hpp>
#include <s3e/Core/Scene.hpp>
#include <s3e/Graphics/FrameBuffer.hpp>
#include <s3e/Graphics/Shader.hpp>
#include <vector>

namespace Tom::s3e {
    class GeometryBuffer {
        Shader::Ptr shader;
        FrameBuffer::Ptr buffer;

    public:
        using Ptr = std::shared_ptr<GeometryBuffer>;
        using ConstPtr = std::shared_ptr<const GeometryBuffer>;

        GeometryBuffer(const sf::Vector2u & size);

        virtual ~GeometryBuffer();

        const sf::Vector2u getSize() const;

        void setSize(const sf::Vector2u & size);

        const std::vector<FrameBufferTexture::Ptr> & getTextures() const;

        void draw(const Scene::ConstPtr & scene, const glm::mat4 & mvp) const;
    };

    class LightingShader {
        Shader::Ptr shader;
        bool hdr;

    public:
        using Ptr = std::shared_ptr<LightingShader>;
        using ConstPtr = std::shared_ptr<const LightingShader>;

        LightingShader(const Shader::Ptr & shader);

        virtual ~LightingShader();

        void draw(const GeometryBuffer::ConstPtr & buff,
                  const Camera::ConstPtr & camera) const;
    };
}
