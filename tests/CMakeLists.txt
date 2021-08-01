
macro(test target)
    add_executable(${target} ${CMAKE_CURRENT_SOURCE_DIR}/${target}.cpp)
    target_link_libraries(${target} PRIVATE gtest gtest_main Core Graphics Support)

    add_test(NAME ${target} COMMAND ${target})
endmacro()

test(test_vbo_vertex)
test(test_vbo)