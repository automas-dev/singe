
set(TARGET workspace)
add_executable(${TARGET}
    main.cpp
    Game.hpp
    Game.cpp
    Collision.cpp
    Collision.hpp
    Diamond.hpp
)

target_compile_features(${TARGET} PRIVATE cxx_std_17)

target_link_libraries(${TARGET}
PRIVATE
    spdlog::spdlog
    Threads::Threads
    Core
    Graphics
)
