
set(TARGET workspace)
add_executable(${TARGET}
    main.cpp
    Game.hpp
    Game.cpp
    FastScene.hpp
    FastScene.cpp
    Node.hpp
)

target_compile_features(${TARGET} PRIVATE cxx_std_17)

target_link_libraries(${TARGET}
PRIVATE
    spdlog::spdlog
    Threads::Threads
    glpp
    Core
    Graphics
    tinyxml2
)
