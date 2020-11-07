cmake_minimum_required(VERSION 3.15)

project(sfml-mc VERSION 0.1 LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)

file(GLOB_RECURSE HEADER_LIST CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/includes/*.hpp")

add_library(libgame STATIC
    src/Camera.cpp
    src/Material.cpp
    src/Menu.cpp
    src/Model.cpp
    src/ResourceManager.cpp
    src/Shader.cpp
    src/Util.cpp
    src/VBO.cpp
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

add_subdirectory(demo)
