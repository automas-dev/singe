cmake_minimum_required(VERSION 3.15...3.20)

project(singe
    VERSION 0.10.2
    DESCRIPTION "Scene In Nodes Game Engine"
    LANGUAGES CXX)

option(SINGE_BUILD_DOCS "Builds the singe documentation" ON)
option(SINGE_BUILD_EXAMPLES "Builds the singe examples" ON)
option(SINGE_BUILD_TESTS "Builds the singe tests" ON)

# Only if this is the top level project (not included with add_subdirectory)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # Use -std=c++xx instead of -std=g++xx
    set(CMAKE_CXX_EXTENSIONS OFF)

    # Support folders in IDEs
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    if(SINGE_BUILD_TESTS)
        # Testing only available for top level projects. It calls enable_testing
        # which must be in the main CMakeLists.
        include(CTest)
    endif()

    if(SINGE_BUILD_DOCS)
        # Generate documentation using Doxygen
        find_package(Doxygen)

        if(Doxygen_FOUND)
            add_subdirectory(docs)
        else()
            message(STATUS "Doxygen not found, not building docs")
        endif()
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

find_package(Bullet REQUIRED)
include_directories(${BULLET_INCLUDE_DIR})

add_subdirectory(external)

add_subdirectory(src)

if(SINGE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

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
