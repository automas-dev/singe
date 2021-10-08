#include <gtest/gtest.h>

#include <s3e/Support/Util.hpp>

namespace {
    TEST(Support_Util, SplitStringSingle) {
        std::vector<std::string> expect {
            "one",
        };
        auto actual = splitString("one");
        EXPECT_EQ(expect, actual);
    }

    TEST(Support_Util, SplitStringSpaceDouble) {
        std::vector<std::string> expect {
            "one",
            "two",
        };
        auto actual = splitString("one two");
        EXPECT_EQ(expect, actual);
    }

    TEST(Support_Util, SplitStringSpaceTriple) {
        std::vector<std::string> expect {
            "one",
            "two",
            "three",
        };
        auto actual = splitString("one two three");
        EXPECT_EQ(expect, actual);
    }

    TEST(Support_Util, SplitStringMultiDelimiter) {
        std::vector<std::string> expect {
            "one",
            "",
            "two",
        };
        auto actual = splitString("one  two");
        EXPECT_EQ(expect, actual);
    }

    TEST(Support_Util, SplitStringCommaDelimiter) {
        std::vector<std::string> expect {
            "one",
            "two",
        };
        auto actual = splitString("one,two", ',');
        EXPECT_EQ(expect, actual);
    }
}