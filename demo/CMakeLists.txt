
add_executable(demo main.cpp)

target_compile_features(demo PRIVATE cxx_std_17)

target_link_libraries(demo 
PRIVATE 
    libgame 
    sfml-window 
    sfml-graphics 
    sfml-system 
    OpenGL::OpenGL 
    OpenGL::GLU 
    GLEW::GLEW
)
