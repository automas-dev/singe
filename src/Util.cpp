#include "s3e/Util.hpp"
#include <cstring>
#include <cctype>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <spdlog/fmt/fmt.h>

std::ostream & operator<<(std::ostream & os, const glm::vec2 & vec) {
    return os << fmt::format("[{}, {}]", vec[0], vec[1]);
}

std::ostream & operator<<(std::ostream & os, const glm::vec3 & vec) {
    return os << fmt::format("[{}, {}, {}]", vec[0], vec[1], vec[2]);
}

std::ostream & operator<<(std::ostream & os, const glm::vec4 & vec) {
    return os << fmt::format("[{}, {}, {}, {}]", vec[0], vec[1], vec[2], vec[3]);
}

std::ostream & operator<<(std::ostream & os, const glm::mat2 & mat) {
    return os << fmt::format("[{}, {}]", mat[0], mat[1]);
}

std::ostream & operator<<(std::ostream & os, const glm::mat3 & mat) {
    return os << fmt::format("[{}, {}, {}]", mat[0], mat[1], mat[2]);
}

std::ostream & operator<<(std::ostream & os, const glm::mat4 & mat) {
    return os << fmt::format("[{}, {}, {}, {}]", mat[0], mat[1], mat[2], mat[3]);
}

namespace Tom::s3e {
    bool strStartsWithChar(char pre, const std::string & str) {
        return !str.empty() && str[0] == pre;
    }

    bool strStartsWithStr(const std::string & pre, const std::string & str) {
        return str.rfind(pre, 0) == 0;
    }

    glm::mat4 matFromVecs(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) {
        glm::mat4 mpos = glm::translate(glm::mat4(1), pos);
        glm::quat qrot (rot);
        glm::mat4 mrot = glm::toMat4(qrot);
        glm::mat4 msize = glm::scale(glm::mat4(1), scale);
        return mpos * mrot * msize;
    }
}

namespace Tom::s3e {

    static int seekChar(std::ifstream & fin, char find) {
        char c;
        while (fin >> std::noskipws >> c && c != find);
        return c;
    }

    static int lengthToDelim(std::ifstream & fin, char delim) {
        long begin = fin.tellg();
        int c = seekChar(fin, '\n');
        long end = fin.tellg();
        fin.seekg(begin);
        return end - begin;
    }

    Parser::Parser() { }

    Parser::Parser(const std::string & path) {
        open(path);
    }

    Parser::~Parser() {
        close();
    }

    bool Parser::open(const std::string & path) {
        fin.open(path);
        return fin.is_open();
    }

    void Parser::close() {
        fin.close();
    }

    bool Parser::is_open() const {
        return fin.is_open();
    }

    bool Parser::eof() const {
        return !fin;
        // return !is_open() || fin.eof();
    }

    std::string Parser::readToChar(char delim) {
        int len = lengthToDelim(fin, delim);
        if (len < 1)
            return std::string();

        std::string buff(len, ' ');
        fin.read(&buff[0], len);
        buff.resize(buff.size() - 1); // Hacky way to remove \n

        if (!fin) {
            return std::string();
        }

        return buff;
    }

    std::string Parser::readLine() {
        return readToChar('\n');
    }

    void Parser::rewind() {
        fin.clear();
        fin.seekg(0);
    }

    Parser::Ptr Parser::create(const std::string & path) {
        auto parser = std::make_shared<Parser>(path);
        return parser;
    }
};
