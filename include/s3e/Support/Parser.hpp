#pragma once

#include <fstream>
#include <string>
#include <vector>

namespace Tom::s3e {
    /**
     * Split a string based on a delimeter.
     *
     * @param str the string to split
     * @param delim the delimiter
     *
     * @return a vector of strings
     */
    std::vector<std::string> splitString(const std::string & str, char delim = ' ');

    /**
     * Parser for Wavefront formatted .obj and .mtl files.
     */
    class WavefrontParser {
    public:
        /**
         * A single token with key and value.
         */
        struct Token {
            /// The token command / key
            std::string key;
            /// Everything after the first space
            std::string value;

            /**
             * Split value on spaces and return the results.
             *
             * @return value split by spaces
             */
            std::vector<std::string> params() const;
        };

    private:
        std::ifstream is;

    public:
        /**
         * Open a file at path for parsing.
         *
         * @param path the file to open
         */
        void open(const std::string & path);

        /**
         * Check if a file is currently open.
         *
         * @return is a file open
         */
        bool is_open() const;

        /**
         * Parse the open file and return all tokens.
         *
         * @return all tokens from the open file
         */
        std::vector<Token> tokens();
    };
}
