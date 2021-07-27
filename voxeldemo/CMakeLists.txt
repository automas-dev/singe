
add_executable(voxeldemo
    main.cpp
    Game.hpp
    Game.cpp
)

target_compile_features(voxeldemo PRIVATE cxx_std_17)

target_link_libraries(voxeldemo
PRIVATE
    spdlog::spdlog
    Tom::s3e
)

