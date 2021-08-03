#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

#include "s3e/Graphics/Model.hpp"
#include "s3e/Graphics/Shader.hpp"
#include "s3e/Support/Transform3d.hpp"

namespace Tom::s3e {

    struct Scene : public Transform3d {

        using Ptr = std::shared_ptr<Scene>;
        using ConstPtr = std::shared_ptr<const Scene>;

        std::string name;
        std::vector<Model::Ptr> models;
        std::vector<Scene::Ptr> children;

        Scene(const std::string & name);

        void draw(const Shader::Ptr & shader,
                  glm::mat4 transform = glm::mat4(1)) const;
    };
};