
set(TARGET UI)

set(HEADER_LIST
    FPSDisplay.hpp
    Menu.hpp
    Mesh.hpp)
list(TRANSFORM HEADER_LIST PREPEND "${${PROJECT_NAME}_SOURCE_DIR}/include/${PROJECT_NAME}/${TARGET}/")

set(SOURCE_LIST
    FPSDisplay.cpp
    Menu.cpp)
list(TRANSFORM SOURCE_LIST PREPEND "${${PROJECT_NAME}_SOURCE_DIR}/src/${TARGET}/")

library_component(${TARGET} "${SOURCE_LIST}" "${HEADER_LIST}")

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
    # Graphics
    GLEW::GLEW
    sfml-graphics
    glm
    fmt::fmt
    ${BULLET_LIBRARIES}
    DispatchQueue
    )
