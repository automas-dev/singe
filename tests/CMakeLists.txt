set(TARGET tests)

set(SOURCE_LIST
    test_graphics_framebuffer.cpp
    test_graphics_shader.cpp
    test_graphics_texture.cpp
    test_support_parser.cpp
    test_support_transform3d.cpp
    test_support_util.cpp
    test_graphics_vbo_vertex.cpp
    test_graphics_vbo.cpp)
list(TRANSFORM SOURCE_LIST PREPEND "${${PROJECT_NAME}_SOURCE_DIR}/tests/")

add_executable(${TARGET} ${SOURCE_LIST})
target_link_libraries(${TARGET} PRIVATE gtest gtest_main Core Graphics Support)

add_test(NAME ${TARGET} COMMAND ${TARGET})