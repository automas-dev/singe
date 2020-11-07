cmake_minimum_required(VERSION 3.15)

project(sfml-mc VERSION 0.1.1 LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)

file(GLOB_RECURSE HEADER_LIST CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/includes/*.hpp")

add_library(s3e STATIC
    src/Camera.cpp
    src/GameBase.cpp
    src/Material.cpp
    src/Menu.cpp
    src/Model.cpp
    src/ResourceManager.cpp
    src/Shader.cpp
    src/Util.cpp
    src/VBO.cpp
    ${HEADER_LIST}
)

add_library(Tom::s3e ALIAS s3e)

target_compile_features(s3e PRIVATE cxx_std_17)

target_include_directories(s3e PUBLIC includes
                        PRIVATE ${GLM_INCLUDE_DIRS})

target_link_libraries(s3e
PUBLIC
    sfml-window 
    sfml-graphics 
    sfml-system
    OpenGL::OpenGL 
    OpenGL::GLU
    GLEW::GLEW
)

include(CheckLibraryExists)
CHECK_LIBRARY_EXISTS(m sin "" HAVE_LIB_M)                                                                                                
if (HAVE_LIB_M)                                                                                                                          
  target_link_libraries(s3e PRIVATE m)                                                                                                      
endif (HAVE_LIB_M)

add_subdirectory(demo)

# install(TARGETS s3e
#         EXPORT Tom::s3e
#         LIBRARY DESTINATION lib
#         ARCHIVE DESTINATION lib
#         INCLUDES DESTINATION include)

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
