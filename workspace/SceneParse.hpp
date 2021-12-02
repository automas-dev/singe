#pragma once

#include <singe/Core/Camera.hpp>
#include <singe/Graphics/Grid.hpp>
#include <singe/Core/ResourceManager.hpp>
#include <singe/Core/Scene.hpp>
#include <string>
#include <vector>

namespace singe {
    struct SceneStruct {
        Camera::Ptr camera;
        Grid::Ptr grid;
        std::vector<Scene::Ptr> scenes;
    };

    SceneStruct parseScene(const std::string & file,
                           const ResourceManager & res);
}