include(FetchContent)

# FetchContent_Declare(
#     rapidxml
#     GIT_REPOSITORY  https://gitlab.com/twh2898/rapidxml.git
#     GIT_TAG         1.13.0
# )

FetchContent_Declare(
    googletest
    GIT_REPOSITORY  https://github.com/google/googletest
    GIT_TAG         release-1.12.1
)

FetchContent_Declare(
    DispatchQueue
    GIT_REPOSITORY  https://github.com/twh2898/DispatchQueue.git
    GIT_TAG         0.2.2
)

FetchContent_Declare(
    glpp
    GIT_REPOSITORY  https://github.com/twh2898/glpp.git
    GIT_TAG         0.6.4
)

FetchContent_Declare(
    Wavefront
    GIT_REPOSITORY  https://github.com/twh2898/wavefront.git
    GIT_TAG         0.1.0
)

# FetchContent_Declare(
#     assimp
#     GIT_REPOSITORY  https://github.com/assimp/assimp.git
#     GIT_TAG         v5.2.5
# )

FetchContent_MakeAvailable(
    # rapidxml
    googletest
    DispatchQueue
    glpp
    Wavefront
    # assimp
    )
