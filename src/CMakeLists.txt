
set(TARGET s3e)

# Find with GLOB if CMake version is at least 3.12, otherwise use set
file(GLOB_RECURSE HEADER_LIST CONFIGURE_DEPENDS "${${PROJECT_NAME}_SOURCE_DIR}/include/*.hpp")
#set(HEADER_LIST "${${PROJECT_NAME}_SOURCE_DIR}/include/${PROJECT_NAME}/foo.hpp")

file(GLOB_RECURSE SOURCE_LIST CONFIGURE_DEPENDS "${${PROJECT_NAME}_SOURCE_DIR}/src/*.cpp")

add_library(${TARGET} ${SOURCE_LIST} ${HEADER_LIST})

target_include_directories(${TARGET}
PRIVATE
    ${GLM_INCLUDE_DIRS}
    ${BULLET_INCLUDE_DIR}
PUBLIC
    $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

#set_property(TARGET ${TARGET} PROPERTY POSITION_INDEPENDENT_CODE ON)
target_compile_features(${TARGET} PUBLIC cxx_std_17)

target_link_libraries(${TARGET}
PRIVATE
    fmt::fmt
    spdlog::spdlog
PUBLIC
    sfml-window 
    sfml-graphics 
    sfml-system
    OpenGL::OpenGL 
    OpenGL::GLU
    GLEW::GLEW
    spdlog::spdlog
    ${BULLET_LIBRARIES})

# Check for Inter Procedural Optimization (IPO)
include(CheckIPOSupported)
check_ipo_supported(RESULT result)
if(result)
    set_target_properties(${TARGET} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

# Add ALIAS target for use in add_subdirectory or build directory
add_library(${PROJECT_NAME}::${TARGET} ALIAS ${TARGET})

# IDE support, show headers in a folder
source_group(TREE "${PROJECT_SOURCE_DIR}/include"
    PREFIX "Header Files"
    FILES ${HEADER_LIST})

# Export targets so build dir can be used by other projects
export(TARGETS ${TARGET}
    NAMESPACE ${PROJECT_NAME}::
    FILE ${PROJECT_NAME}Targets.cmake)

# Add target to Project Targets for install
install(TARGETS ${TARGET}
    EXPORT ${PROJECT_NAME}Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}) # This is for Windows?

## Include the headers in install
#install(DIRECTORY
#    $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
#    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
#    # ../include/
#    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
