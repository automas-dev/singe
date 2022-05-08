set(TARGET Core)

set(HEADER_LIST
    FPSDisplay.hpp
    GameBase.hpp
    Menu.hpp
    Window.hpp
)
list(TRANSFORM HEADER_LIST PREPEND "include/${PROJECT_NAME}/${TARGET}/")

set(SOURCE_LIST
    FPSDisplay.cpp
    GameBase.cpp
    Menu.cpp
    Window.cpp
)
list(TRANSFORM SOURCE_LIST PREPEND "src/")

add_library(${TARGET} ${HEADER_LIST} ${SOURCE_LIST} include/default_font.h)

target_link_libraries(${TARGET}
    PRIVATE
    spdlog::spdlog
    OpenGL::OpenGL
    OpenGL::GLU
    PUBLIC
    glpp
    Wavefront
    Support
    # Graphics
    GLEW::GLEW
    sfml-graphics
    glm
    fmt::fmt
    DispatchQueue
    )

set_property(TARGET ${TARGET} PROPERTY POSITION_INDEPENDENT_CODE ON)

library_component(${TARGET})
