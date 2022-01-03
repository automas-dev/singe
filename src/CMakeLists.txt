
macro(library_component target)
    target_include_directories(${target}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
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

    #Add target to Project Targets for install
    install(TARGETS ${target}
        EXPORT ${PROJECT_NAME}Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}) # This is for Windows?

    ## Include the headers in install
    install(DIRECTORY
        include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${target})
endmacro()

add_subdirectory(Transform3d)
add_subdirectory(Wavefront)
add_subdirectory(glpp EXCLUDE_FROM_ALL)

add_subdirectory(singe-support)
add_subdirectory(singe-graphics)
add_subdirectory(singe-core)
# add_subdirectory(Support)
# add_subdirectory(Graphics)
# add_subdirectory(Core)
# add_subdirectory(UI)

set(TARGET singe)

add_library(${TARGET} INTERFACE)

target_link_libraries(${TARGET}
    INTERFACE
    OpenGL::OpenGL
    OpenGL::GLU
    GLEW::GLEW
    ${BULLET_LIBRARIES}
    )

#Check for Inter Procedural Optimization(IPO)
include(CheckIPOSupported)
check_ipo_supported(RESULT result)
if(result)
    set_target_properties(${TARGET} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

#Add ALIAS target for use in add_subdirectory or build directory
add_library(${PROJECT_NAME}::${TARGET} ALIAS ${TARGET})

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
