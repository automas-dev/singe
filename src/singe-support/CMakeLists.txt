set(TARGET Support)

set(HEADER_LIST
    log.hpp
    SceneParser.hpp
    Util.hpp)
list(TRANSFORM HEADER_LIST PREPEND "include/${PROJECT_NAME}/${TARGET}/")

set(SOURCE_LIST
    log.cpp
    SceneParser.cpp
    Util.cpp)
list(TRANSFORM SOURCE_LIST PREPEND "src/")

add_library(${TARGET} ${HEADER_LIST} ${SOURCE_LIST})

target_link_libraries(${TARGET}
    PUBLIC
    sfml-window
    spdlog::spdlog
    glm
    fmt::fmt
    rapidxml
    )

set_property(TARGET ${TARGET} PROPERTY POSITION_INDEPENDENT_CODE ON)

library_component(${TARGET})
