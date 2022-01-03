set(TARGET Graphics)

set(HEADER_LIST
    Geometry.hpp
    Grid.hpp
    Material.hpp
    Mesh.hpp
    Model.hpp
    ResourceManager.hpp
    Scene.hpp)
list(TRANSFORM HEADER_LIST PREPEND "include/${PROJECT_NAME}/${TARGET}/")

set(SOURCE_LIST
    Geometry.cpp
    Grid.cpp
    Material.cpp
    Mesh.cpp
    Model.cpp
    ResourceManager.cpp
    Scene.cpp)
list(TRANSFORM SOURCE_LIST PREPEND "src/")

add_library(${TARGET} ${HEADER_LIST} ${SOURCE_LIST} include/default_font.h)

target_link_libraries(${TARGET}
    PRIVATE
    spdlog::spdlog
    OpenGL::OpenGL
    OpenGL::GLU
    glpp
    PUBLIC
    Transform3d
    Wavefront
    Support
    Core
    GLEW::GLEW
    sfml-graphics
    glm
    fmt::fmt
    DispatchQueue
    )

set_property(TARGET ${TARGET} PROPERTY POSITION_INDEPENDENT_CODE ON)

library_component(${TARGET})
