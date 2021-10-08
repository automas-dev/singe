#include <gtest/gtest.h>

#include <s3e/Support/Util.hpp>

namespace {
    TEST(Util, SplitStringSingle) {
        std::vector<std::string> expect {
            "one",
        };
        auto actual = splitString("one");
        EXPECT_EQ(expect, actual);
    }

    TEST(Util, SplitStringSpaceDouble) {
        std::vector<std::string> expect {
            "one",
            "two",
        };
        auto actual = splitString("one two");
        EXPECT_EQ(expect, actual);
    }

    TEST(Util, SplitStringSpaceTriple) {
        std::vector<std::string> expect {
            "one",
            "two",
            "three",
        };
        auto actual = splitString("one two three");
        EXPECT_EQ(expect, actual);
    }

    TEST(Util, SplitStringMultiDelimiter) {
        std::vector<std::string> expect {
            "one",
            "",
            "two",
        };
        auto actual = splitString("one  two");
        EXPECT_EQ(expect, actual);
    }

    TEST(Util, SplitStringCommaDelimiter) {
        std::vector<std::string> expect {
            "one",
            "two",
        };
        auto actual = splitString("one,two", ',');
        EXPECT_EQ(expect, actual);
    }
}