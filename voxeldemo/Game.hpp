#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <memory>
#include <vector>
#include <map>
#include <algorithm>
#include <thread>
#include <mutex>
#include <random>
#include <s3e.hpp>
using namespace Tom::s3e;

struct ChunkGenerator {
    int seed;
    float scale;
    BlockStyle::Ptr style;

    typedef std::shared_ptr<ChunkGenerator> Ptr;
    typedef std::shared_ptr<const ChunkGenerator> ConstPtr;

    ChunkGenerator(void) : ChunkGenerator(std::rand()) { }

    ChunkGenerator(int seed) : seed(seed), scale(0.05), style(std::make_shared<BlockStyle>()) { }

    Chunk::Ptr loadChunk(int x, int z) {
        auto chunk = std::make_shared<Chunk>(glm::vec3(x, 0, z));
        for (int i = x; i < x + SubChunk::N; i++) {
            for (int j = z; j < z + SubChunk::N; j++) {
                auto height = glm::simplex(glm::vec3(x * scale, seed, z * scale));
                height = 10 + height * 256;
                for (int y = 0; y < height; y++) {
                    chunk->set(i, y, j, style);
                }
            }
        }
        return chunk;
    }
};

struct ChunkLoader {
    std::thread t;
    ChunkGenerator::Ptr gen;
    std::list<std::pair<int, int>> loadQueue;
    std::list<Chunk::Ptr> loaded;
    std::mutex queueMutex;

    typedef std::shared_ptr<ChunkLoader> Ptr;
    typedef std::shared_ptr<const ChunkLoader> ConstPtr;

    ChunkLoader(void) : t(&ChunkLoader::run, this) { }

    ChunkLoader(const ChunkGenerator::Ptr & gen) : gen(gen) { }

    void run(void) {
    }

    void load(int x, int z) {
        auto pair = std::make_pair(x, z);
        std::scoped_lock lock(queueMutex);
        loadQueue.push_back(pair);
    }
};

struct ChunkManager {
    std::map<std::pair<int, int>, Chunk::Ptr> chunks;

    typedef std::shared_ptr<ChunkManager> Ptr;
    typedef std::shared_ptr<const ChunkManager> ConstPtr;

    void set(int x, int y, int z, const BlockStyle::Ptr & style) {
        get(x, y, z)->set(x, y, z, style);
    }

    Chunk::Ptr & get(int x, int y, int z) {
        x = x / SubChunk::N;
        z = z / SubChunk::N;
        auto pair = std::make_pair(x, z);
        if (chunks.count(pair) == 0) {
            glm::vec3 pos(x * SubChunk::N, 0, z * SubChunk::N);
            chunks[pair] = std::make_shared<Chunk>(pos);
        }
        return chunks[pair];
    }

    void update(const sf::Time & delta) {

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
    Model::Ptr model;
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
