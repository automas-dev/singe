include(FetchContent)

FetchContent_Declare(
    rapidxml
    GIT_REPOSITORY  https://github.com/automas-dev/rapidxml.git
    GIT_TAG         1.13.0
)

FetchContent_MakeAvailable(rapidxml)
