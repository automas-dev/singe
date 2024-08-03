
set(TARGET workspace)
add_executable(${TARGET}
    main.cpp
    Game.hpp
    Game.cpp
    Collision.cpp
    Collision.hpp
    Diamond.hpp
    Markers.hpp
    Physics.cpp
    Physics.hpp
)

target_compile_features(${TARGET} PRIVATE cxx_std_17)

target_link_libraries(${TARGET}
PRIVATE
    spdlog::spdlog
    Threads::Threads
    Core
    Graphics
)
