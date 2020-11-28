#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <s3e.hpp>
using namespace Tom::s3e;

class Game : public GameBase {
    std::vector<glm::vec3> gridVerts;
    std::vector<glm::vec3> gridCols;

    DefaultResourceManager resManager;

    sf::Font font;

    Shader::Ptr defaultShader;
    Shader::Ptr debugShader;
    Shader::Ptr geometryShader;
    MaterialShader::Ptr lightingShader;
    MaterialShader::Ptr monoShader;

    Texture::Ptr texture;

    bool doDrawTexture = true;
    bool doDrawShading = true;

    Model::Ptr cubeModel;
    Model::Ptr sphereModel;

    FrameBuffer::Ptr fbuff;
    FrameBuffer::Ptr gbuff;

    float time = 0.0;

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
    void onDraw() const override;

private:
    void drawPass(glm::mat4 vp, const MaterialShader::Ptr & shader) const;
    void drawModel(const Model::ConstPtr & model, glm::mat4 vp, const MaterialShader::Ptr & shader) const;
};
