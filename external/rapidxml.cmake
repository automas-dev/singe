include(FetchContent)

FetchContent_Declare(
    rapidxml
    GIT_REPOSITORY  https://gitlab.com/twh2898/rapidxml.git
    GIT_TAG         1.13.0
)

FetchContent_MakeAvailable(rapidxml)
