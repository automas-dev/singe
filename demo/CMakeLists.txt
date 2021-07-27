
add_executable(demo 
    main.cpp
    Game.hpp
    Game.cpp
)

target_compile_features(demo PRIVATE cxx_std_17)

target_link_libraries(demo 
PRIVATE 
    spdlog::spdlog
    Tom::s3e
)
