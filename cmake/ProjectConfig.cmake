include(CMakeFindDependencyMacro)

find_dependency(Threads REQUIRED)
find_dependency(SFML 2.5 REQUIRED)
find_dependency(GLEW REQUIRED)
find_dependency(OpenGL REQUIRED)
find_dependency(glm REQUIRED)
find_dependency(fmt REQUIRED)
find_dependency(spdlog REQUIRED)
find_dependency(Bullet REQUIRED)

include("${CMAKE_CURRENT_LIST_DIR}/@PROJECT_NAME@Targets.cmake")
