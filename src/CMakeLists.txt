
macro(library_component target source_list header_list)
    add_library(${target} ${source_list} ${header_list})

    target_include_directories(${target}
        PRIVATE
        ${GLM_INCLUDE_DIRS}
        PUBLIC
        $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

    #set_property(TARGET ${TARGET} PROPERTY POSITION_INDEPENDENT_CODE ON)
    target_compile_features(${target} PUBLIC cxx_std_17)

    #Check for Inter Procedural Optimization(IPO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT result)
    if(result)
        set_target_properties(${target} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif()

    #Add ALIAS target for use in add_subdirectory or build directory
    add_library(${PROJECT_NAME}::${target} ALIAS ${target})

    #Export targets so build dir can be used by other projects
    export(TARGETS ${target}
        NAMESPACE ${PROJECT_NAME}::
        FILE ${PROJECT_NAME}${target}Targets.cmake)

    #Add target to Project Targets for install
    install(TARGETS ${target}
        EXPORT ${PROJECT_NAME}Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}) # This is for Windows?

    ## Include the headers in install
    install(DIRECTORY
        ${${PROJECT_NAME}_SOURCE_DIR}/include/${PROJECT_NAME}/${target}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
endmacro()

add_subdirectory(Support)
add_subdirectory(Graphics)
add_subdirectory(Core)

# set(TARGET s3e)

# add_library(${TARGET})

# target_include_directories(${TARGET}
#     PRIVATE
#     ${GLM_INCLUDE_DIRS}
#     ${BULLET_INCLUDE_DIR}
#     PUBLIC
#     $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
#     $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# #set_property(TARGET ${TARGET} PROPERTY POSITION_INDEPENDENT_CODE ON)
# target_compile_features(${TARGET} PUBLIC cxx_std_17)

# target_link_libraries(${TARGET}
#     PRIVATE
#     fmt::fmt
#     spdlog::spdlog
#     sfml-window
#     sfml-graphics
#     sfml-system
#     OpenGL::OpenGL
#     OpenGL::GLU
#     GLEW::GLEW
#     ${BULLET_LIBRARIES}
#     PUBLIC
#     Support
#     Graphics
#     Core)

# #Check for Inter Procedural Optimization(IPO)
# include(CheckIPOSupported)
# check_ipo_supported(RESULT result)
# if(result)
#     set_target_properties(${TARGET} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
# endif()

# #Add ALIAS target for use in add_subdirectory or build directory
# add_library(${PROJECT_NAME}::${TARGET} ALIAS ${TARGET})

#IDE support, show headers in a folder
source_group(TREE "${PROJECT_SOURCE_DIR}/include"
    PREFIX "Header Files"
    FILES ${HEADER_LIST})

#Export targets so build dir can be used by other projects
export(TARGETS ${TARGET}
    NAMESPACE ${PROJECT_NAME}::
    FILE ${PROJECT_NAME}Targets.cmake)

#Add target to Project Targets for install
install(TARGETS ${TARGET}
    EXPORT ${PROJECT_NAME}Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}) # This is for Windows?

## Include the headers in install
install(FILES
    ${${PROJECT_NAME}_SOURCE_DIR}/include/Core.hpp
    ${${PROJECT_NAME}_SOURCE_DIR}/include/Graphics.hpp
    ${${PROJECT_NAME}_SOURCE_DIR}/include/Support.hpp
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
