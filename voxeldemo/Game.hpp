#pragma once

#include <GL/glew.h>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <random>
#include <s3e.hpp>
#include <thread>
#include <vector>
using namespace Tom::s3e;

#include <chrono>
using namespace std::chrono_literals;

struct ChunkGenerator {
    int seed;
    float scale;
    std::vector<BlockStyle::Ptr> styles;

    typedef std::shared_ptr<ChunkGenerator> Ptr;
    typedef std::shared_ptr<const ChunkGenerator> ConstPtr;

    ChunkGenerator(void) : ChunkGenerator(std::rand()) {}

    UV uvFor(int u, int v) {
        float u1 = u / (float)SubChunk::N;
        float u2 = (u + 1) / (float)SubChunk::N;
        float v1 = v / (float)SubChunk::N;
        float v2 = (v + 1) / (float)SubChunk::N;
        return UV(u1, v1, u2, v2);
    }

    ChunkGenerator(int seed) : seed(seed), scale(0.05) {
        for (int y = 0; y < SubChunk::N; y++) {
            for (int x = 0; x < SubChunk::N; x++) {
                for (int z = 0; z < SubChunk::N; z++) {
                    auto top = uvFor(x, z);
                    auto side1 = uvFor(x, y);
                    auto side2 = uvFor(y, z);
                    styles.push_back(std::make_shared<BlockStyle>(
                        side1, side1, side2, side2, top, top));
                }
            }
        }
    }

    Chunk::Ptr loadChunk(int pos_x, int pos_z) {
        auto chunk = std::make_shared<Chunk>(glm::vec3(pos_x, 0, pos_z));
        float s = 0.02;
        for (int local_x = 0; local_x < SubChunk::N; local_x++) {
            for (int local_z = 0; local_z < SubChunk::N; local_z++) {
                int x = pos_x + local_x;
                int z = pos_z + local_z;
                auto height = 10 + glm::simplex(glm::vec2(x * s, z * s)) * 4;
                for (int y = 0; y < height; y++) {
                    auto style_index = y * SubChunk::N * SubChunk::N;
                    style_index += local_x * SubChunk::N;
                    style_index += local_z;
                    style_index %= styles.size();
                    chunk->set(x, y, z, styles[style_index]);
                }
            }
        }
        std::this_thread::sleep_for(1s);
        return chunk;
    }
};

struct ChunkLoader {
    DispatchQueue taskQueue;
    ChunkGenerator::Ptr gen;
    std::queue<Chunk::Ptr> loaded;
    std::mutex loadedMutex;

    typedef std::shared_ptr<ChunkLoader> Ptr;
    typedef std::shared_ptr<const ChunkLoader> ConstPtr;

    ChunkLoader(const ChunkGenerator::Ptr & gen)
        : taskQueue(DispatchQueue::Concurrent), gen(gen) {}

    bool hasNext() {
        std::scoped_lock lk(loadedMutex);
        return !loaded.empty();
    }

    Chunk::Ptr next() {
        std::scoped_lock lk(loadedMutex);
        auto front = loaded.front();
        loaded.pop();
        return front;
    }

    void stop() {
        taskQueue.stop();
    }

    void join() {
        taskQueue.join();
    }

    void load(int x, int z) {
        std::scoped_lock lk(loadedMutex);
        taskQueue.push([&, x, z]() {
            auto chunk = gen->loadChunk(x, z);
            std::scoped_lock lk(loadedMutex);
            loaded.push(chunk);
        });
    }
};

struct ChunkManager {
    std::map<std::pair<int, int>, Chunk::Ptr> chunks;
    ChunkLoader loader;

    typedef std::shared_ptr<ChunkManager> Ptr;
    typedef std::shared_ptr<const ChunkManager> ConstPtr;

    ChunkManager() : ChunkManager(std::make_shared<ChunkGenerator>()) {}

    ChunkManager(ChunkGenerator::Ptr gen) : loader(gen) {}

    virtual ~ChunkManager() {
        loader.stop();
    }

    // void set(int x, int y, int z, const BlockStyle::Ptr & style) {
    //     get(x, y, z)->set(x, y, z, style);
    // }

    // Chunk::Ptr & get(int x, int y, int z) {
    //     x = x / SubChunk::N;
    //     z = z / SubChunk::N;
    //     auto pair = std::make_pair(x, z);
    //     if (chunks.count(pair) == 0) {
    //         // loader.load(x, z);
    //         glm::vec3 pos(x * SubChunk::N, 0, z * SubChunk::N);
    //         chunks[pair] = std::make_shared<Chunk>(pos);
    //     }
    //     return chunks[pair];
    // }

    void update(const sf::Time & delta) {
        if (loader.hasNext()) {
            auto chunk = loader.next();
            chunks[std::make_pair<int, int>(chunk->pos.x, chunk->pos.z)] = chunk;
            SPDLOG_DEBUG("Got a chunk {}, {} in update", chunk->pos.x,
                         chunk->pos.z);
        }
    }

    std::vector<Vertex> toPoints(void) {
        std::vector<Vertex> points;
        for (auto & e : chunks) {
            auto & chunk = e.second;
            auto p = chunk->toPoints();
            points.insert(points.end(), p.begin(), p.end());
        }
        return points;
    }
};

class Game : public GameBase {
    DefaultResourceManager resManager;
    FPSDisplay::Ptr fps;
    Texture::Ptr devTexture;
    VBO::Ptr model;
    ChunkManager::Ptr chunks;
    std::vector<BlockStyle::Ptr> styles;

public:
    Game(const sf::String & resPath);
    virtual ~Game();

    bool onCreate(void) override;
    void onDestroy(void) override;

    void onKeyPressed(const sf::Event::KeyEvent & e) override;
    void onKeyReleased(const sf::Event::KeyEvent & e) override;
    void onMouseMove(const sf::Event::MouseMoveEvent & e) override;
    void onMouseDown(const sf::Event::MouseButtonEvent & e) override;
    void onMouseUp(const sf::Event::MouseButtonEvent & e) override;
    void onResized(const sf::Event::SizeEvent & e) override;

    void onUpdate(const sf::Time & delta) override;
    void onDraw(void) const override;
};
