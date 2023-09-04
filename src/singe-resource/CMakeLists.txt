set(TARGET Resource)

set(HEADER_LIST
    ResourceManager.hpp)
list(TRANSFORM HEADER_LIST PREPEND "include/${PROJECT_NAME}/${TARGET}/")

set(SOURCE_LIST
    ResourceManager.cpp)
list(TRANSFORM SOURCE_LIST PREPEND "src/")

add_library(${TARGET} ${HEADER_LIST} ${SOURCE_LIST})

target_link_libraries(${TARGET}
    PRIVATE
    OpenGL::OpenGL
    OpenGL::GLU
    Threads::Threads
    glpp
    PUBLIC
    Wavefront
    Support
    Graphics
    GLEW::GLEW
    glm
    )

set_property(TARGET ${TARGET} PROPERTY POSITION_INDEPENDENT_CODE ON)

library_component(${TARGET})
