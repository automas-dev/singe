
set(TARGET Core)

set(HEADER_LIST
    Camera.hpp
    FPSDisplay.hpp
    GameBase.hpp
    Material.hpp
    Menu.hpp
    Mesh.hpp
    Model.hpp
    Physics.hpp
    ResourceManager.hpp
    Scene.hpp)
list(TRANSFORM HEADER_LIST PREPEND "${${PROJECT_NAME}_SOURCE_DIR}/include/${PROJECT_NAME}/${TARGET}/")

set(SOURCE_LIST
    Camera.cpp
    FPSDisplay.cpp
    GameBase.cpp
    Material.cpp
    Menu.cpp
    Mesh.cpp
    Model.cpp
    Physics.cpp
    ResourceManager.cpp
    Scene.cpp)
list(TRANSFORM SOURCE_LIST PREPEND "${${PROJECT_NAME}_SOURCE_DIR}/src/${TARGET}/")

library_component(${TARGET} "${SOURCE_LIST}" "${HEADER_LIST}")

target_link_libraries(${TARGET}
    PRIVATE
    spdlog::spdlog
    OpenGL::OpenGL
    OpenGL::GLU
    PUBLIC
    Support
    Graphics
    GLEW::GLEW
    sfml-graphics
    glm
    fmt::fmt
    ${BULLET_LIBRARIES}
    DispatchQueue
    )
