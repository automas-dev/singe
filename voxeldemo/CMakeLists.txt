
add_executable(voxeldemo
    main.cpp
    Game.hpp
    Game.cpp
)

target_compile_features(voxeldemo PRIVATE cxx_std_17)

target_link_libraries(voxeldemo
PRIVATE
    Tom::s3e
    OpenGL::GLU
)

