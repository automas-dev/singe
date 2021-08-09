
set(TARGET workspace)
add_executable(${TARGET}
    main.cpp
    Game.hpp
    Game.cpp
    GeometryBuffer.hpp
    GeometryBuffer.cpp
)

target_compile_features(${TARGET} PRIVATE cxx_std_17)

target_link_libraries(${TARGET}
PRIVATE
    fmt::fmt
    spdlog::spdlog
    Threads::Threads
    Core
)
