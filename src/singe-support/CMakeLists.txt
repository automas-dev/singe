set(TARGET Support)

set(HEADER_LIST
    log.hpp
    Util.hpp)
list(TRANSFORM HEADER_LIST PREPEND "include/${PROJECT_NAME}/${TARGET}/")

set(SOURCE_LIST
    log.cpp
    Util.cpp)
list(TRANSFORM SOURCE_LIST PREPEND "src/")

add_library(${TARGET} ${HEADER_LIST} ${SOURCE_LIST})

target_link_libraries(${TARGET}
    PUBLIC
    sfml-window
    spdlog::spdlog
    glm
    fmt::fmt)

set_property(TARGET ${TARGET} PROPERTY POSITION_INDEPENDENT_CODE ON)

library_component(${TARGET})
