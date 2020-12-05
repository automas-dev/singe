cmake_minimum_required(VERSION 3.15)

project(sfml-s3e VERSION 0.3.4 LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

configure_file(cmake/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)

add_subdirectory(lib/spdlog EXCLUDE_FROM_ALL)

set(S3E_HEADERS 
    include/s3e.hpp
    include/s3e/Camera.hpp
    include/s3e/FPSDisplay.hpp
    include/s3e/FrameBuffer.hpp
    include/s3e/GameBase.hpp
    include/s3e/log.hpp
    include/s3e/Material.hpp
    include/s3e/Menu.hpp
    include/s3e/Model.hpp
    include/s3e/ResourceManager.hpp
    include/s3e/Shader.hpp
    include/s3e/Texture.hpp
    include/s3e/Util.hpp
    include/s3e/VBO.hpp
    ${CMAKE_CURRENT_BINARY_DIR}/include/config.h
)

set(S3E_SOURCES
    src/Camera.cpp
    src/FPSDisplay.cpp
    src/FrameBuffer.cpp
    src/GameBase.cpp
    src/Material.cpp
    src/Menu.cpp
    src/Model.cpp
    src/ResourceManager.cpp
    src/Shader.cpp
    src/Texture.cpp
    src/Util.cpp
    src/VBO.cpp
)

add_library(s3e STATIC
    ${S3E_SOURCES}
    ${S3E_HEADERS}
)

add_library(Tom::s3e ALIAS s3e)

target_compile_features(s3e PRIVATE cxx_std_17)

target_include_directories(s3e
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${GLM_INCLUDE_DIRS}
)

target_link_libraries(s3e
PUBLIC
    sfml-window 
    sfml-graphics 
    sfml-system
    OpenGL::OpenGL 
    OpenGL::GLU
    GLEW::GLEW
    spdlog::spdlog
)

add_subdirectory(demo)

#export(TARGETS s3e NAMESPACE Tom::
#    FILE ${CMAKE_CURRENT_BINARY_DIR}/s3e-config.cmake)

install(TARGETS s3e DESTINATION lib EXPORT s3e-config PUBLIC_HEADER DESTINATION include)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include)
#install(EXPORT s3e-config DESTINATION include/c NAMESPACE Tom::)
export(PACKAGE s3e)

# Packaging support
set(CPACK_PACKAGE_VENDOR "Thomas Harrison")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "An awesome 3d lib")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
# set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENCE")
# set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

set(CPACK_SOURCE_GENERATOR "TGZ;ZIP")
set(CPACK_SOURCE_IGNORE_FILES
    /.git
    /.vscode
    /.*build.*
)

include(CPack)
