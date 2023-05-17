#pragma once

#include <istream>
#include <string>
#include <vector>

namespace singe::util {
    using std::istream;
    using std::string;
    using std::vector;

    class Tokenizer {
        char token;
        istream & stream;
        const istream & end;

    public:
        Tokenizer(istream & begin, const istream & end, char token);

        bool hasNext();

        string next();
    };

    /**
     * Split a string based on a delimeter.
     *
     * @param str the string to split
     * @param delim the delimiter
     *
     * @return a vector of strings
     */
    vector<string> splitString(const string & str, char delim = ' ');
}