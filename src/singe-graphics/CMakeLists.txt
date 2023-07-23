set(TARGET Graphics)

set(HEADER_LIST
    Material.hpp
    Model.hpp
    RenderState.hpp
    ResourceManager.hpp
    Scene.hpp
    Shader.hpp
    UniformExtra.hpp)
list(TRANSFORM HEADER_LIST PREPEND "include/${PROJECT_NAME}/${TARGET}/")

set(SOURCE_LIST
    Material.cpp
    Model.cpp
    RenderState.cpp
    ResourceManager.cpp
    Scene.cpp
    Shader.cpp
    UniformExtra.cpp)
list(TRANSFORM SOURCE_LIST PREPEND "src/")

add_library(${TARGET} ${HEADER_LIST} ${SOURCE_LIST})

target_link_libraries(${TARGET}
    PRIVATE
    OpenGL::OpenGL
    OpenGL::GLU
    glpp
    PUBLIC
    Wavefront
    Support
    GLEW::GLEW
    glm
    rapidxml
    )

set_property(TARGET ${TARGET} PROPERTY POSITION_INDEPENDENT_CODE ON)

library_component(${TARGET})
