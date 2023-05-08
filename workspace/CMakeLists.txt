
set(TARGET workspace)
add_executable(${TARGET}
    main.cpp
    Game.hpp
    Game.cpp
    # SceneParse.hpp
    # SceneParse.cpp
)

target_compile_features(${TARGET} PRIVATE cxx_std_17)

target_link_libraries(${TARGET}
PRIVATE
    spdlog::spdlog
    Threads::Threads
    Core
    Graphics
    # rapidxml
)
