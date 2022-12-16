include(FetchContent)

FetchContent_Declare(
    Wavefront
    GIT_REPOSITORY  https://github.com/twh2898/wavefront.git
    GIT_TAG         0.1.0
)

set(WAVEFRONT_BUILD_DOCS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(Wavefront)
