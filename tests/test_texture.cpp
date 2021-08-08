#include <SFML/Graphics.hpp>
#include <s3e/Core/GameBase.hpp>
#include <s3e/Graphics/Texture.hpp>
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

    TEST(Texture, TextureImage) {
        Game game;
        game.Create("");
        sf::Image image;
        image.create(100, 200);
        Texture tex(image);
        EXPECT_GT(tex.getTextureId(), 0);
        EXPECT_EQ(100, tex.getSize().x);
        EXPECT_EQ(200, tex.getSize().y);
    }

    TEST(Texture, TextureSize) {
        Game game;
        game.Create("");
        Texture tex({100, 200});
        EXPECT_GT(tex.getTextureId(), 0);
        EXPECT_EQ(100, tex.getSize().x);
        EXPECT_EQ(200, tex.getSize().y);
    }

    TEST(Texture, resize) {
        Game game;
        game.Create("");
        Texture tex({100, 200});
        tex.resize({300, 400});
        EXPECT_EQ(300, tex.getSize().x);
        EXPECT_EQ(400, tex.getSize().y);
    }
}