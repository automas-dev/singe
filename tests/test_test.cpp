#include "test.h"
#include <s3e/Graphics/VBO.hpp>
using namespace Tom::s3e;

int test_Vertex_Vertex() {
    Vertex v;
    ASSERT_EQ(glm::vec3(0), v.pos);
    ASSERT_EQ(glm::vec3(0), v.norm);
    ASSERT_EQ(glm::vec2(0), v.uv);
    return 0;
}

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
TEST(test_Vertex_Vertex);
END_TESTS