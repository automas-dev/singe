set(TARGET Transform3d)

add_library(${TARGET} OBJECT src/Transform3d.cpp include/Transform3d.hpp)

set_property(TARGET ${TARGET} PROPERTY POSITION_INDEPENDENT_CODE ON)

library_component(${TARGET})
