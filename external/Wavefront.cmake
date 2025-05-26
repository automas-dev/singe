include(FetchContent)

FetchContent_Declare(
    Wavefront
    GIT_REPOSITORY  https://github.com/automas-dev/wavefront.git
    GIT_TAG         0.2.0
)

set(WAVEFRONT_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(WAVEFRONT_BUILD_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(Wavefront)
