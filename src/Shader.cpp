#include "s3e/Shader.hpp"
#include <fstream>
#include "s3e/log.hpp"
#include "s3e/Util.hpp"

namespace Tom::s3e {

    void draw_color_array_legacy(const float *vertices,
                                 const float *colors,
                                 size_t n,
                                 GLenum mode) {

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, vertices);
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(3, GL_FLOAT, 0, colors);

        glDrawArrays(mode, 0, n);

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
    }

    void draw_color_array(const std::vector<glm::vec3> & vertices,
                          const std::vector<glm::vec3> & colors,
                          GLenum mode) {

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, &vertices[0].x);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, &colors[0].x);

        glDrawArrays(mode, 0, vertices.size());

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    }

    void draw_tex_array(const std::vector<glm::vec3> & vertices,
                        const std::vector<glm::vec2> & uvs,
                        GLenum mode) {

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, &vertices[0].x);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, &uvs[0].x);

        glDrawArrays(mode, 0, vertices.size());

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    }

    std::string shaderSource(const std::string & path) {
        std::ifstream t(path);
        if (!t) {
            SPDLOG_ERROR("shaderSource(path={}) failed to open file", path);
            return std::string();
        }

        auto start = std::istreambuf_iterator<char>(t);
        auto end = std::istreambuf_iterator<char>();

        return std::string((start), end);
    }

    static bool compileSuccess(GLuint shader) {
        GLint success = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        return success != GL_FALSE;
    }

    static bool linkSuccess(GLuint program) {
        GLint success = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        return success != GL_FALSE;
    }

    static std::string compileError(GLuint shader) {
        GLint logSize = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

        std::vector<GLchar> errorLog(logSize);
        glGetShaderInfoLog(shader, logSize, &logSize, &errorLog[0]);

        return std::string(errorLog.begin(), errorLog.end());
    }

    static std::string linkError(GLuint program) {
        GLint logSize = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);

        std::vector<GLchar> errorLog(logSize);
        glGetProgramInfoLog(program, logSize, &logSize, &errorLog[0]);

        return std::string(errorLog.begin(), errorLog.end());
    }

    static GLuint compileShader(GLuint shaderType,
                                const std::string & shaderSource) {

        GLuint shader = glCreateShader(shaderType);
        const GLchar *src = shaderSource.c_str();

        glShaderSource(shader, 1, &src, NULL);
        glCompileShader(shader);

        return shader;
    }

    Shader::Shader() : program(0) { }

    Shader::~Shader() {
        if (program > 0)
            glDeleteProgram(program);
    }

    bool Shader::loadFromPath(const std::string & vertexPath,
                              const std::string & fragmentPath) {
        SPDLOG_DEBUG("loading Shader from paths vertex = \"{}\" fragment = \"{}\"", vertexPath, fragmentPath);

        std::string vertexSource = shaderSource(vertexPath);
        std::string fragmentSource = shaderSource(fragmentPath);

        if (vertexSource.empty()) {
            SPDLOG_ERROR("vertex shader source could not be loaded from path {}", vertexPath);
            return false;
        }

        if (fragmentSource.empty()) {
            SPDLOG_ERROR("fragment shader source could not be loaded from path {}", fragmentPath);
            return false;
        }

        return loadFromSource(vertexSource, fragmentSource);
    }

    bool Shader::loadFromSource(const std::string & vertexSource,
                                const std::string & fragmentSource) {

        GLuint vShader = compileShader(GL_VERTEX_SHADER, vertexSource);
        if (!compileSuccess(vShader)) {
            SPDLOG_ERROR("failed to compile vertex shader {}: {}", vShader, compileError(vShader));
            glDeleteShader(vShader);
            return false;
        }

        GLuint fShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
        if (!compileSuccess(fShader)) {
            SPDLOG_ERROR("failed to compile fragment shader {}: {}", fShader, compileError(fShader));
            glDeleteShader(vShader);
            glDeleteShader(fShader);
            return false;
        }

        program = glCreateProgram();
        glAttachShader(program, vShader);
        glAttachShader(program, fShader);

        glLinkProgram(program);

        glDetachShader(program, vShader);
        glDetachShader(program, fShader);
        glDeleteShader(vShader);
        glDeleteShader(fShader);

        if (!linkSuccess(program)) {
            SPDLOG_ERROR("failed to link shader program {}: {}", program, linkError(program));
            glDeleteProgram(program);
            return false;
        }

        SPDLOG_DEBUG("shader was successfully compiled and linked as program = {} with vertex shader = {} fragment shader = {}",
                     program, vShader, fShader);
        return true;
    }

    void Shader::bind() {
        SPDLOG_TRACE("bind opengl shader program: {}", program);
        glUseProgram(program);
    }

    void Shader::unbind() {
        SPDLOG_TRACE("bind opengl shader program: 0 (unbind)");
        glUseProgram(0);
    }

    GLuint Shader::uniformLocation(const std::string & name) const {
        return glGetUniformLocation(program, name.c_str());
    }

    void Shader::setBool(const std::string & name, bool value) const {
        SPDLOG_TRACE("glUniform1i(name = \"{}\" value = {}) bool variant", name, value);
        glUniform1i(uniformLocation(name), (int)value);
    }

    void Shader::setInt(const std::string & name, int value) const {
        SPDLOG_TRACE("glUniform1i(name = \"{}\" value = {})", name, value);
        glUniform1i(uniformLocation(name), value);
    }
    void Shader::setUInt(const std::string & name, unsigned int value) const {
        SPDLOG_TRACE("glUniform1ui(name = \"{}\" value = {})", name, value);
        glUniform1ui(uniformLocation(name),  value);
    }

    void Shader::setFloat(const std::string & name, float value) const {
        SPDLOG_TRACE("glUniform1f(name = \"{}\" value = {})", name, value);
        glUniform1f(uniformLocation(name), value);
    }

    void Shader::setVec2(const std::string & name, const glm::vec2 & value) const {
        SPDLOG_TRACE("glUniform2fv(name = \"{}\" value = [{}, {}])", name, value.x, value.y);
        glUniform2fv(uniformLocation(name), 1, &value.x);
    }

    void Shader::setVec3(const std::string & name, const glm::vec3 & value) const {
        SPDLOG_TRACE("glUniform3fv(name = \"{}\" value = [{}, {}, {}])", name, value.x, value.y, value.z);
        glUniform3fv(uniformLocation(name), 1, &value.x);
    }

    void Shader::setVec4(const std::string & name, const glm::vec4 & value) const {
        SPDLOG_TRACE("glUniform4fv(name = \"{}\" value = [{}, {}, {}, {}])", name, value.x, value.y, value.z, value.w);
        glUniform4fv(uniformLocation(name), 1, &value.x);
    }

    void Shader::setMat2(const std::string & name, const glm::mat2 & value) const {
        SPDLOG_TRACE("glUniformMatrix2fv(name = \"{}\" value = {}", name, value);
        glUniformMatrix2fv(uniformLocation(name), 1, GL_FALSE, &value[0][0]);
    }

    void Shader::setMat3(const std::string & name, const glm::mat3 & value) const {
        SPDLOG_TRACE("glUniformMatrix3fv(name = \"{}\" value = {}", name, value);
        glUniformMatrix3fv(uniformLocation(name), 1, GL_FALSE, &value[0][0]);
    }

    void Shader::setMat4(const std::string & name, const glm::mat4 & value) const {
        SPDLOG_TRACE("glUniformMatrix4fv(name = \"{}\" value = {}", name, value);
        glUniformMatrix4fv(uniformLocation(name), 1, GL_FALSE, &value[0][0]);
    }
};

