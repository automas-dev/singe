#include <SFML/Graphics.hpp>
#include <s3e/Core/GameBase.hpp>
#include <s3e/Graphics/Shader.hpp>
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

    TEST(Shader, defaultShader) {
        Game game;
        game.Create("");
        auto shader = Shader::defaultShader();
        EXPECT_NE(shader, nullptr);
        EXPECT_GT(shader->getProgram(), 0);
    }
}