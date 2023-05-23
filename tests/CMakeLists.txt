set(TARGET tests)

macro(define_test name)
    set(SOURCE_LIST
        test_${name}.cpp
    )
    list(TRANSFORM SOURCE_LIST PREPEND "${${PROJECT_NAME}_SOURCE_DIR}/tests/")

    add_executable(${TARGET}_${name} ${SOURCE_LIST})
    target_link_libraries(${TARGET}_${name} PRIVATE
        gtest
        gtest_main
        singe::Core
        singe::Graphics
        singe::Support)

    target_include_directories(${TARGET}_${name} PRIVATE "${${PROJECT_NAME}_SOURCE_DIR}/tests/")

    add_test(NAME ${TARGET}_${name} COMMAND ${TARGET}_${name})
endmacro()

define_test(RenderState)
