
set(TARGET Graphics)

set(HEADER_LIST
    FrameBuffer.hpp
    Shader.hpp
    Texture.hpp
    VBO.hpp)
list(TRANSFORM HEADER_LIST PREPEND "${${PROJECT_NAME}_SOURCE_DIR}/include/s3e/${TARGET}/")

set(SOURCE_LIST
    FrameBuffer.cpp
    Shader.cpp
    Texture.cpp
    VBO.cpp)
list(TRANSFORM SOURCE_LIST PREPEND "${${PROJECT_NAME}_SOURCE_DIR}/src/${TARGET}/")

library_component(${TARGET} "${SOURCE_LIST}" "${HEADER_LIST}")

target_link_libraries(${TARGET}
    PRIVATE
    Support
    OpenGL::OpenGL
    OpenGL::GLU
    PUBLIC
    GLEW::GLEW
    sfml-graphics
    glm)
