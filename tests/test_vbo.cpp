#include <s3e/Core/GameBase.hpp>
#include <s3e/Graphics/VBO.hpp>
using namespace Tom::s3e;

#include <gtest/gtest.h>

namespace {
    class Game : public GameBase {
    public:
        Game() : GameBase() {}
        virtual ~Game() {}

        bool onCreate(void) override {
            return true;
        }
        void onDestroy(void) override {}

        void onUpdate(const sf::Time & delta) override {}
        void onDraw(void) const override {}
    };

    TEST(VBO, VBO) {
        Game game;
        game.Create("");
        VBO vbo;
        EXPECT_GT(vbo.getVAO(), 0);
        EXPECT_GT(vbo.getVBO(), 0);
        EXPECT_EQ(VBO::Triangles, vbo.getMode());
        EXPECT_EQ(VBO::Static, vbo.getUsage());
    }

    TEST(VBO, VBO_mode_usage) {
        Game game;
        game.Create("");
        VBO vbo(VBO::Lines, VBO::Dynamic);
        EXPECT_EQ(VBO::Lines, vbo.getMode());
        EXPECT_EQ(VBO::Dynamic, vbo.getUsage());
    }

    TEST(VBO, setMode) {
        Game game;
        game.Create("");
        VBO vbo;
        EXPECT_EQ(VBO::Triangles, vbo.getMode());
        vbo.setMode(VBO::Lines);
        EXPECT_EQ(VBO::Lines, vbo.getMode());
    }

    TEST(VBO, setUsage) {
        Game game;
        game.Create("");
        VBO vbo;
        EXPECT_EQ(VBO::Static, vbo.getUsage());
        vbo.setUsage(VBO::Dynamic);
        EXPECT_EQ(VBO::Dynamic, vbo.getUsage());
    }

    TEST(VBO, loadFromPoints) {
        Game game;
        game.Create("");
        VBO vbo;
        vbo.loadFromPoints(std::vector<Vertex> {
            {{0, 0, 0}, {0, 0, 0}, {0, 0}},
            {{0, 0, 0}, {0, 0, 0}, {0, 0}},
            {{0, 0, 0}, {0, 0, 0}, {0, 0}},
        });
        EXPECT_EQ(3, vbo.size());
    }
}