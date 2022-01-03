set(TARGET Wavefront)

add_library(${TARGET} OBJECT
    src/Wavefront.cpp
    include/Wavefront.hpp
    src/WavefrontParser.cpp
    include/WavefrontParser.hpp)

target_link_libraries(${TARGET} PUBLIC glm)

set_property(TARGET ${TARGET} PROPERTY POSITION_INDEPENDENT_CODE ON)

library_component(${TARGET})
