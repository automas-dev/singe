include(FetchContent)

FetchContent_Declare(
   fmtlib
   GIT_REPOSITORY https://github.com/fmtlib/fmt.git
   GIT_TAG 9.1.0)

set(FMT_PEDANTIC OFF)
set(FMT_WERROR OFF)
set(FMT_DOC OFF)
set(FMT_INSTALL ON)
set(FMT_TEST OFF)
set(FMT_FUZZ OFF)
set(FMT_CUDA_TEST OFF)
set(FMT_OS ON)
set(FMT_MODULE OFF)
set(FMT_SYSTEM_HEADERS OFF)

FetchContent_MakeAvailable(fmtlib)
