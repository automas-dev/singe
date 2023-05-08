include(FetchContent)

FetchContent_Declare(
	glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG		   0.9.8
)

set(USE_SYSTEM_GLM ON)

FetchContent_MakeAvailable(glm)
