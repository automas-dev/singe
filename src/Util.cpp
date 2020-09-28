#include "Util.hpp"
#include <cstring>
#include <cctype>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace game {

    bool strStartsWithChar(char pre, const std::string & str) {
        return str.rfind(pre, 0) == 0;
    }

    bool strStartsWithStr(const std::string & pre, const std::string & str) {
        return str.rfind(pre, 0) == 0;
    }

    void modeProjection(void) {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
    }

    void modeModel(void) {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    void push(glm::vec3 pos, glm::vec2 rot) {
        glPushMatrix();
        glRotatef(-rot.x, 1, 0, 0);
        glRotatef(-rot.y, 0, 1, 0);
        glTranslatef(-pos.x, -pos.y, -pos.z);
    }

    glm::mat4 matFromVecs(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) {
        glm::mat4 mpos = glm::translate(glm::mat4(), pos);
        glm::quat qrot (rot);
        glm::mat4 mrot = glm::toMat4(qrot);
        glm::mat4 msize = glm::scale(glm::mat4(), scale);
        return mpos * mrot * msize;
    }
}

namespace game {

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
        return !is_open() || fin.eof();
    }

    std::string Parser::readToChar(char delim) {
        int len = lengthToDelim(fin, delim);
        if (len < 1)
            return NULL;

        std::string buff(len, ' ');
        fin.read(&buff[0], len);

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