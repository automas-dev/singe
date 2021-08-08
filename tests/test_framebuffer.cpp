#include <SFML/Graphics.hpp>
#include <s3e/Core/GameBase.hpp>
#include <s3e/Graphics/FrameBuffer.hpp>
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

    TEST(FrameBuffer, FrameBuffer) {
        Game game;
        game.Create("");
        FrameBuffer fb({100, 200},
                       std::vector<FrameBufferAttachment> {
                           FrameBufferAttachment(GL_COLOR_ATTACHMENT0),
                       },
                       true,
                       8);
        EXPECT_GT(fb.getId(), 0);
        EXPECT_EQ(100, fb.getSize().x);
        EXPECT_EQ(200, fb.getSize().y);
        EXPECT_TRUE(fb.isMultisampled());
        EXPECT_EQ(1, fb.count());
    }
}