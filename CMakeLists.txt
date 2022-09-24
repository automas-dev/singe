cmake_minimum_required(VERSION 3.15...3.20)

project(singe
    VERSION 0.1.0
    DESCRIPTION "Scene In Nodes Game Engine"
    LANGUAGES CXX)

# Set the default build type if non is specified
set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}' since none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
        STRING "Choose the type of build." FORCE)

    # Set possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Only if this is the top level project (not included with add_subdirectory)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # Use -std=c++xx instead of -std=g++xx
    set(CMAKE_CXX_EXTENSIONS OFF)

    # Support folders in IDEs
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # Testing only available for top level projects. It calls enable_testing
    # which must be in the main CMakeLists.
    include(CTest)

    # Generate documentation using Doxygen
    find_package(Doxygen)
    if(Doxygen_FOUND)
        add_subdirectory(docs)
    else()
        message(STATUS "Doxygen not found, not building docs")
    endif()
endif()

# Use for platform independent install dirs, despite being called GNU
include(GNUInstallDirs)

# Create config.h with project version numbers
configure_File(cmake/config.h.in include/config.h)
include_directories(PRIVATE ${CMAKE_BINARY_DIR}/include)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Threads REQUIRED)
find_package(SFML 2.5 REQUIRED CONFIG COMPONENTS graphics window system)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED CONFIG)
find_package(fmt REQUIRED CONFIG)
find_package(spdlog REQUIRED CONFIG)

find_package(Bullet REQUIRED)
include_directories(${BULLET_INCLUDE_DIR})

include(FetchContent)

FetchContent_Declare(
    rapidxml
    GIT_REPOSITORY  https://gitlab.com/twh2898/rapidxml.git
    GIT_TAG         1.13.0
)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY  https://github.com/google/googletest
    GIT_TAG         release-1.12.1
)

FetchContent_Declare(
    DispatchQueue
    GIT_REPOSITORY  https://github.com/twh2898/DispatchQueue.git
    GIT_TAG         v0.1.4
)

FetchContent_Declare(
    glpp
    GIT_REPOSITORY  https://github.com/twh2898/glpp.git
    GIT_TAG         0.5.4
)

FetchContent_Declare(
    Wavefront
    GIT_REPOSITORY  https://github.com/twh2898/wavefront.git
    GIT_TAG         0.1.0
)

FetchContent_Declare(
    assimp
    GIT_REPOSITORY  https://github.com/assimp/assimp.git
    GIT_TAG         v5.2.5
)

FetchContent_MakeAvailable(rapidxml googletest DispatchQueue glpp Wavefront assimp)

add_subdirectory(src)

# Create Targets file
install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

# Generate ConfigVersion file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion)

# Copy the Config file to the build dir
configure_file(cmake/ProjectConfig.cmake ${PROJECT_NAME}Config.cmake @ONLY)

# Add Config and ConfigVersion files to install
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

# Add examples and tests
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING) AND BUILD_TESTING)
    add_subdirectory(workspace)
    # add_subdirectory(tests)
endif()
