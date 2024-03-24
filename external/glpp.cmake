include(FetchContent)

FetchContent_Declare(
    glpp
    GIT_REPOSITORY  https://github.com/twh2898/glpp.git
    GIT_TAG         0.8.0
)

set(GLPP_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLPP_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLPP_BUILD_ OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glpp)
