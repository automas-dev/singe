include(FetchContent)

FetchContent_Declare(
	glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG		   1.0.1
)

set(USE_SYSTEM_GLM ON)

FetchContent_MakeAvailable(glm)
