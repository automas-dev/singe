#include "GeometryBuffer.hpp"

static const char * geometryFragmentShader = R"(
#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;
layout (location = 3) out float gSpecular;
uniform sampler2D tex;
uniform float specular = 0.4;
in vec3 FragPos;
in vec3 FragNorm;
in vec2 FragTex;
void main() {
    gPosition = FragPos;
    gNormal = normalize(FragNorm);
    gAlbedo = texture(tex, FragTex).rgb;
    gSpecular = specular;
})";

namespace Tom::s3e {
    static const std::vector<FrameBufferAttachment> attachments {
        FrameBufferAttachment(GL_COLOR_ATTACHMENT0, GL_RGB16F, GL_RGB, GL_FLOAT),
        FrameBufferAttachment(GL_COLOR_ATTACHMENT1, GL_RGB16F, GL_RGB, GL_FLOAT),
        FrameBufferAttachment(GL_COLOR_ATTACHMENT2, GL_RGB, GL_RGB, GL_FLOAT),
        FrameBufferAttachment(GL_COLOR_ATTACHMENT3, GL_R16F, GL_RED, GL_FLOAT),
    };

    GeometryBuffer::GeometryBuffer(const sf::Vector2u & size)
        : shader(Shader::fromFragmentSource(geometryFragmentShader)),
          buffer(std::make_shared<FrameBuffer>(size, attachments, true, 0)) {}

    GeometryBuffer::~GeometryBuffer() {}

    const sf::Vector2u GeometryBuffer::getSize() const {
        return buffer->getSize();
    }

    void GeometryBuffer::setSize(const sf::Vector2u & size) {
        buffer->setSize(size);
    }

    const std::vector<FrameBufferTexture::Ptr> & GeometryBuffer::getTextures() const {
        if (buffer->isMultisampled())
            return buffer->getResolved()->getTextures();
        else
            return buffer->getTextures();
    }

    void GeometryBuffer::draw(const Scene::ConstPtr & scene,
                              const glm::mat4 & mvp) const {
        buffer->bind();
        shader->bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader->setMat4("mvp", mvp);
        scene->draw(shader);
        shader->unbind();
        buffer->unbind();
    }
}
