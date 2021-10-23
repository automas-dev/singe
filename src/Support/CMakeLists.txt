
set(TARGET Support)

set(HEADER_LIST
    Keymap.hpp
    log.hpp
    Parser.hpp
    Transform3d.hpp
    Util.hpp)
list(TRANSFORM HEADER_LIST PREPEND "${${PROJECT_NAME}_SOURCE_DIR}/include/s3e/${TARGET}/")

set(SOURCE_LIST
    Keymap.cpp
    log.cpp
    Parser.cpp
    Transform3d.cpp
    Util.cpp)
list(TRANSFORM SOURCE_LIST PREPEND "${${PROJECT_NAME}_SOURCE_DIR}/src/${TARGET}/")

library_component(${TARGET} "${SOURCE_LIST}" "${HEADER_LIST}")

target_link_libraries(${TARGET}
    PUBLIC
    sfml-window
    spdlog::spdlog
    glm
    fmt::fmt)
