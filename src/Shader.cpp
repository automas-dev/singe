#include "Shader.hpp"
#include <SFML/Graphics/Image.hpp>
#include <fstream>
#include <iostream>

namespace game {
    static GLuint loadGlTexture(const unsigned char *data, int width, int height, bool srcAlpha = false,
                                GLint magFilter = GL_LINEAR, GLint minFilter = GL_LINEAR_MIPMAP_LINEAR, GLint wrap = GL_REPEAT, bool mipmaps = true) {
        GLuint textureID;
        glGenTextures(1, &textureID);

        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, (srcAlpha ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);

        if (mipmaps)
            glGenerateMipmap(GL_TEXTURE_2D);

        return textureID;
    }

    // GLuint loadTexture(const char *path, bool srcAlpha) {
    //     GLuint texId = 0;
    //     int width, height, nrChannels;
    //     unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    //     if (data) {
    //         texId = loadGlTexture(data, width, height, srcAlpha);
    //         stbi_image_free(data);
    //     }
    //     return texId;
    // }

    Texture::Texture() {

    }

    Texture::~Texture() {
        glDeleteTextures(1, &textureId);
    }

    bool Texture::loadFromPath(const std::string & path) {
        sf::Image img;
        if (!img.loadFromFile(path))
            return false;

        // glGenTextures(1, &textureId);
        // glBindTexture(GL_TEXTURE_2D, textureId);
        // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.getSize().x, img.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());
        textureId = loadGlTexture(img.getPixelsPtr(), img.getSize().x, img.getSize().y, true, GL_NEAREST, GL_NEAREST_MIPMAP_LINEAR);
        return true;
    }

    void Texture::bind() {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureId);
    }

    void Texture::unbind() {
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Texture::Ptr Texture::create(const std::string & path) {
        auto texture = std::make_shared<Texture>();
        if (texture && texture->loadFromPath(path)) {
            return texture;
        }
        return nullptr;
    }
}

namespace game {

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

    GLuint Shader::uniformLocation(const std::string & name) const {
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