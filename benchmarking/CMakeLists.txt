
find_package(benchmark REQUIRED)

add_executable(benchmarking
    main.cpp
    Game.hpp
    Game.cpp
)

target_compile_features(benchmarking PRIVATE cxx_std_17)

target_link_libraries(benchmarking
PRIVATE
    Tom::s3e
    OpenGL::GLU
    benchmark
)

