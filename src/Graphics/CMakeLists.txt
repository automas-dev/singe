
set(TARGET Graphics)

set(HEADER_LIST
    Grid.hpp
    RenderState.hpp
    )
list(TRANSFORM HEADER_LIST PREPEND "${${PROJECT_NAME}_SOURCE_DIR}/include/${PROJECT_NAME}/${TARGET}/")

set(SOURCE_LIST
    Grid.cpp
    RenderState.cpp
    )
list(TRANSFORM SOURCE_LIST PREPEND "${${PROJECT_NAME}_SOURCE_DIR}/src/${TARGET}/")

library_component(${TARGET} "${SOURCE_LIST}" "${HEADER_LIST}")

target_link_libraries(${TARGET}
    PRIVATE
    Support
    OpenGL::OpenGL
    OpenGL::GLU
    PUBLIC
    glpp
    Transform3d
    GLEW::GLEW
    sfml-graphics
    glm)
