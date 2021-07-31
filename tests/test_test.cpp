#include "test.h"

int test_First() {
    int a = 1;
    int b = a + 2;
    ASSERT_EQ(a + 2, b);
    ASSERT_NOT_EQ(a + 1, b);
    ASSERT_NOT_EQ(a, b);
    ASSERT_TRUE(a > 0);
    ASSERT_FALSE(a > b);
    return 0;
}

BEGIN_TESTS
TEST(test_First);
END_TESTS