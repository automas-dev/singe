set(TARGET tests)

set(SOURCE_LIST
    test_shader.cpp
    test_texture.cpp
    test_vbo_vertex.cpp
    test_vbo.cpp)
list(TRANSFORM SOURCE_LIST PREPEND "${${PROJECT_NAME}_SOURCE_DIR}/tests/")

add_executable(${TARGET} ${SOURCE_LIST})
target_link_libraries(${TARGET} PRIVATE gtest gtest_main Core Graphics Support)

add_test(NAME ${TARGET} COMMAND ${TARGET})