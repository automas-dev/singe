#include "singe/Graphics/Shader.hpp"

#include <memory>

namespace singe {
    using std::move;

    Shader::Shader(glpp::Shader && shader)
        : m_shader(move(shader)), m_mvp(m_shader.uniform("mvp")) {}

    Shader::~Shader() {}

    const glpp::Shader & Shader::shader() const {
        return m_shader;
    }

    const glpp::Uniform & Shader::mvp() const {
        return m_mvp;
    }

    glpp::Uniform Shader::uniform(const string & name) const {
        return m_shader.uniform(name.data());
    }

    void Shader::bind() const {
        m_shader.bind();
    }

    void Shader::unbind() const {
        m_shader.unbind();
    }
}
