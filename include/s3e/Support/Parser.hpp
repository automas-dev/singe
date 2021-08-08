#pragma once

#include <istream>
#include <iterator>
#include <string>
#include <vector>

namespace Tom::s3e {
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
        std::istream & is;

    public:
        /**
         * Create a new WavefrontParser that reads from is.
         * 
         * @param is the input stream to read from
         */
        WavefrontParser(std::istream & is);

        /**
         * Parse the open file and return all tokens.
         *
         * @return all tokens from the open file
         */
        std::vector<Token> tokens();
    };
}
