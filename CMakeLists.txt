cmake_minimum_required(VERSION 3.15)

project(sfml-mc VERSION 0.1 LANGUAGES CXX)

find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(src game)

