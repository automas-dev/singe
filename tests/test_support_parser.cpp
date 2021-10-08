#include <gtest/gtest.h>

#include <s3e/Support/Parser.hpp>

namespace {
    // TODO: Test WavefrontParser
    /*
    Test Plan:

    - Token splitting value into params
    - Parsing Tokens
      - Empty input
      - Ignores comments
      - Ignores empty lines
      - Key with single value
      - Key with multiple values
    - Iterator
      - Starts at begining of parser
      - Returns tokens in the correct order
      - Ends when parser is out of data
    */
}