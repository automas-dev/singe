
find_package(benchmark REQUIRED)

set(TARGET noise_benchmark)
add_executable(${TARGET}
    main.cpp
)

target_compile_features(${TARGET} PRIVATE cxx_std_17)

target_link_libraries(${TARGET}
PRIVATE
    spdlog::spdlog
    Tom::s3e
    benchmark)
