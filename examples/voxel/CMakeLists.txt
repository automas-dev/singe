
set(TARGET example_voxel)
add_executable(${TARGET}
    main.cpp
    Game.hpp
    Game.cpp
)

target_compile_features(${TARGET} PRIVATE cxx_std_17)

target_link_libraries(${TARGET}
PRIVATE
    fmt::fmt
    spdlog::spdlog
    Threads::Threads
    Tom::s3e
)

