#include <benchmark/benchmark.h>

#include <glm/gtc/noise.hpp>
#include <s3e.hpp>

#include "Game.hpp"

static void BM_Perlin(benchmark::State & state) {
    int x = 0;
    for (auto _ : state) {
        float f = glm::perlin(glm::vec3(x++, 0, 0));
    }
}
BENCHMARK(BM_Perlin);

static void BM_Simplex(benchmark::State & state) {
    int x = 0;
    for (auto _ : state) {
        float f = glm::simplex(glm::vec3(x++, 0, 0));
    }
}
BENCHMARK(BM_Simplex);

static void BM_CubeToPoints(benchmark::State & state) {
    Cube cube;
    glm::vec3 offset(0, 0, 0);
    for (auto _ : state) {
        auto points = cube.toPoints(offset);
    }
}
BENCHMARK(BM_CubeToPoints);

static void BM_SubChunkEmptyToPoints(benchmark::State & state) {
    SubChunk sub;
    for (int x = 0; x < SubChunk::N; x++) {
        for (int y = 0; y < SubChunk::N; y++) {
            for (int z = 0; z < SubChunk::N; z++) {
                sub.get(x, y, z).enabled = false;
            }
        }
    }
    glm::vec3 offset(0, 0, 0);
    for (auto _ : state) {
        auto points = sub.toPoints(offset);
    }
}
BENCHMARK(BM_SubChunkEmptyToPoints);

static void BM_SubChunkToPoints(benchmark::State & state) {
    SubChunk sub;
    for (int x = 0; x < SubChunk::N; x++) {
        for (int y = 0; y < SubChunk::N; y++) {
            for (int z = 0; z < SubChunk::N; z++) {
                sub.get(x, y, z).enabled = true;
            }
        }
    }
    glm::vec3 offset(0, 0, 0);
    for (auto _ : state) {
        auto points = sub.toPoints(offset);
    }
}
BENCHMARK(BM_SubChunkToPoints);

static void BM_ChunkToPoints(benchmark::State & state) {
    Chunk chunk({0, 0, 0}, 10);
    for (int x = 0; x < SubChunk::N; x++) {
        for (int y = 0; y < SubChunk::N * chunk.subchunks.size(); y++) {
            for (int z = 0; z < SubChunk::N; z++) {
                chunk.get(x, y, z).enabled = true;
            }
        }
    }
    for (auto _ : state) {
        auto points = chunk.toPoints();
    }
}
BENCHMARK(BM_ChunkToPoints);

BENCHMARK_MAIN();
