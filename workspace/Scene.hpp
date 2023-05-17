#pragma once

#include <memory>
#include <singe/Graphics/ResourceManager.hpp>
#include <singe/Graphics/Scene.hpp>

namespace singe {
    using std::shared_ptr;

    class SceneLoader {
        ResourceManager & res;

    public:
        SceneLoader(ResourceManager & res);

        shared_ptr<Scene> loadScene(const string & name) const;
    };
}