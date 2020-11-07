
file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/includes/*.hpp")

add_library(libgame STATIC
    Camera.cpp
    Game.cpp
    Material.cpp
    Menu.cpp
    Model.cpp
    ResourceManager.cpp
    Shader.cpp
    Util.cpp
    VBO.cpp
    ${HEADER_LIST}
)

target_compile_features(libgame PRIVATE cxx_std_17)

target_include_directories(libgame PUBLIC includes
                        PRIVATE ${GLM_INCLUDE_DIRS})

target_link_libraries(libgame
PUBLIC
    sfml-window 
    sfml-graphics 
    sfml-system
PRIVATE 
    OpenGL::OpenGL 
    GLEW::GLEW
)

add_executable(game main.cpp)

target_compile_features(game PRIVATE cxx_std_17)

target_link_libraries(game 
PRIVATE 
    libgame 
    sfml-window 
    sfml-graphics 
    sfml-system 
    OpenGL::OpenGL 
    OpenGL::GLU 
    GLEW::GLEW
)
