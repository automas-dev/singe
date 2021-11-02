
set(TARGET Support)

set(HEADER_LIST
    log.hpp
    Parser.hpp
    Util.hpp)
list(TRANSFORM HEADER_LIST PREPEND "${${PROJECT_NAME}_SOURCE_DIR}/include/${PROJECT_NAME}/${TARGET}/")

set(SOURCE_LIST
    log.cpp 
    Parser.cpp
    Util.cpp)
list(TRANSFORM SOURCE_LIST PREPEND "${${PROJECT_NAME}_SOURCE_DIR}/src/${TARGET}/")

library_component(${TARGET} "${SOURCE_LIST}" "${HEADER_LIST}")

target_link_libraries(${TARGET}
    PUBLIC
    sfml-window
    spdlog::spdlog
    glm
    fmt::fmt)
