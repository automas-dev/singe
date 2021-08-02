
set(TARGET Core)

set(HEADER_LIST
    Camera.hpp
    FPSDisplay.hpp
    GameBase.hpp
    ModelLoader.hpp
    Physics.hpp
    ResourceManager.hpp)
list(TRANSFORM HEADER_LIST PREPEND "${${PROJECT_NAME}_SOURCE_DIR}/include/s3e/${TARGET}/")

set(SOURCE_LIST
    Camera.cpp
    FPSDisplay.cpp
    GameBase.cpp
    ModelLoader.cpp
    Physics.cpp
    ResourceManager.cpp)
list(TRANSFORM SOURCE_LIST PREPEND "${${PROJECT_NAME}_SOURCE_DIR}/src/${TARGET}/")

library_component(${TARGET} "${SOURCE_LIST}" "${HEADER_LIST}")

target_link_libraries(${TARGET}
    PRIVATE
    fmt::fmt
    spdlog::spdlog
    sfml-window 
    sfml-graphics 
    sfml-system
    OpenGL::OpenGL 
    OpenGL::GLU
    GLEW::GLEW
    ${BULLET_LIBRARIES}
    Support
    PUBLIC
    DispatchQueue
    Graphics)
