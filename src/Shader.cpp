#include "Shader.hpp"
#include <fstream>
#include <iostream>

namespace game {

    void draw_color_array(const float *vertices,
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

    std::string shaderSource(const std::string & path) {
        std::ifstream t(path);
        if (!t)
            return std::string();

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

    Shader::Shader() { }

    Shader::~Shader() { }

    bool Shader::loadFromPath(const std::string & vertexPath,
                        const std::string & fragmentPath) {

        std::string vertexSource = shaderSource(vertexPath);
        std::string fragmentSource = shaderSource(fragmentPath);

        if (vertexSource.empty() || fragmentSource.empty())
            return false;
        
        return loadFromSource(vertexSource, fragmentSource);
    }

    bool Shader::loadFromSource(const std::string & vertexSource,
                                const std::string & fragmentSource) {
        
        GLuint vShader = compileShader(GL_VERTEX_SHADER, vertexSource);
        if (!compileSuccess(vShader)) {
            std::cerr << "Failed to compile vertex shader: " << compileError(vShader) << std::endl;
            glDeleteShader(vShader);
            return false;
        }

        GLuint fShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
        if (!compileSuccess(fShader)) {
            std::cerr << "Failed to compile fragment shader: " << compileError(fShader) << std::endl;
            glDeleteShader(vShader);
            glDeleteShader(fShader);
            return false;
        }

        program = glCreateProgram();
        glAttachShader(program, vShader);
        glAttachShader(program, fShader);

        // TODO: Pre-Linking Setup with the following
        // TODO: glBindAttribLocation
        // TODO: glBindFragDataLocation
        // TODO: glTransformFeedbackVaryings

        glLinkProgram(program);

        glDetachShader(program, vShader);
        glDetachShader(program, fShader);
        glDeleteShader(vShader);
        glDeleteShader(fShader);

        if (!linkSuccess(program)) {
            std::cerr << "Failed to link shader: " << linkError(program) << std::endl;
            glDeleteProgram(program);
            return false;
        }

        return true;
    }

    GLuint Shader::uniformLocation(const std::string & name) {
        return glGetUniformLocation(program, name.c_str());
    }

    void Shader::bind() {
        glUseProgram(program);
    }

    void Shader::unbind() {
        glUseProgram(0);
    }

    Shader::Ptr Shader::create(const std::string & vertexPath,
                               const std::string & fragmentPath) {
        auto s = std::make_shared<Shader>();
        if (s && s->loadFromPath(vertexPath, fragmentPath)) {
            return s;
        }
        return nullptr;
    }
};