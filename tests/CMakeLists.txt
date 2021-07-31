
macro(test target source)
    add_executable(${target} ${CMAKE_CURRENT_SOURCE_DIR}/${source})
    target_link_libraries(${target} PRIVATE Core)

    add_test(NAME ${target} COMMAND ${target})
endmacro()

test(test_test test_test.cpp)