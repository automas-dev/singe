#include "singe/Graphics/Shader.hpp"

#include <memory>

namespace singe {
    using std::move;

    Shader::Shader(glpp::Shader && shader) : m_shader(move(shader)) {}

    Shader::~Shader() {}

    const glpp::Shader & Shader::shader() const {
        return m_shader;
    }

    glpp::Uniform Shader::uniform(const string & name) const {
        return m_shader.uniform(name.data());
    }

    void Shader::bind() const {
        m_shader.bind();
    }

    void Shader::bind(RenderState & state) const {
        m_shader.bind();
    }

    void Shader::unbind() const {
        m_shader.unbind();
    }
}

namespace singe {
    MVPShader::MVPShader(glpp::Shader && shader)
        : Shader(move(shader)), m_mvp(m_shader.uniform("mvp")) {}

    const glpp::Uniform & MVPShader::mvp() const {
        return m_mvp;
    }

    void MVPShader::bind(RenderState & state) const {
        Shader::bind(state);
        m_mvp.setMat4(state.getMVP());
    }
}
