#pragma once

#include <singe/Core/FPSDisplay.hpp>
#include <singe/Core/GameBase.hpp>
#include <singe/Core/Window.hpp>
#include <singe/Graphics/Mesh.hpp>
#include <singe/Graphics/ResourceManager.hpp>
#include <singe/Graphics/Scene.hpp>
#include <singe/Graphics/Shader.hpp>
#include <singe/Support/log.hpp>
using namespace singe;

#include <glpp/extra/Grid.hpp>
using glpp::extra::Grid;

#include <memory>

class Game : public GameBase {
    ResourceManager res;
    FPSDisplay fps;
    std::shared_ptr<singe::Shader> shader;
    std::shared_ptr<singe::Shader> gridShader;
    // Grid grid;
    Scene scene;

public:
    Game(Window & window);
    virtual ~Game();

    void onUpdate(const sf::Time & delta) override;
    void onDraw(void) const override;
};
