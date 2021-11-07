set(TARGET Wavefront)

add_library(${TARGET} OBJECT
    src/Wavefront.cpp
    include/Wavefront.hpp
    src/WavefrontParser.cpp
    include/WavefrontParser.hpp)

target_link_libraries(${TARGET} PUBLIC glm)

target_include_directories(${TARGET}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

set_property(TARGET ${TARGET} PROPERTY POSITION_INDEPENDENT_CODE ON)
target_compile_features(${TARGET} PUBLIC cxx_std_17)

#Check for Inter Procedural Optimization(IPO)
include(CheckIPOSupported)
check_ipo_supported(RESULT result)
if(result)
    set_target_properties(${TARGET} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

#Add target to Project Targets for install
install(TARGETS ${TARGET}
    EXPORT ${PROJECT_NAME}Targets)

## Include the headers in install
install(DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})