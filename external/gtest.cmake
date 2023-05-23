if(SINGE_BUILD_TESTS)
   include(FetchContent)

   FetchContent_Declare(
      googletest
      GIT_REPOSITORY https://github.com/google/googletest
      GIT_TAG release-1.12.1
   )

   # TODO: flags here
   FetchContent_MakeAvailable(googletest)
endif()
