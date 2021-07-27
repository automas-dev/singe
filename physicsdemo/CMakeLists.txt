
add_executable(physicsdemo 
    main.cpp
    Game.hpp
    Game.cpp
)

target_compile_features(physicsdemo PRIVATE cxx_std_17)

target_link_libraries(physicsdemo 
PRIVATE
    spdlog::spdlog
    Tom::s3e)
