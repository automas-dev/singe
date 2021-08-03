
add_executable(workspace
    main.cpp
    Game.hpp
    Game.cpp
)

target_compile_features(workspace PRIVATE cxx_std_17)

target_link_libraries(workspace 
PRIVATE 
    Threads::Threads
    spdlog::spdlog
    Core
    Support
)
