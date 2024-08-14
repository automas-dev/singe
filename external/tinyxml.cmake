include(FetchContent)

FetchContent_Declare(
    tinyxml
    GIT_REPOSITORY  https://github.com/leethomason/tinyxml2.git
    GIT_TAG         9.0.0
)

FetchContent_MakeAvailable(tinyxml)
