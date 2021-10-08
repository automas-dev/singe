
set(TARGET voxel)
add_executable(${TARGET}
    main.cpp
    Game.hpp
    Game.cpp
    Voxel.hpp
    Voxel.cpp
)

target_compile_features(${TARGET} PRIVATE cxx_std_17)

target_link_libraries(${TARGET}
PRIVATE
    spdlog::spdlog
    Threads::Threads
    Core
)

