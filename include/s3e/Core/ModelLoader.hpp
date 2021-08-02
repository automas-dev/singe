#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

#include "s3e/Graphics/Model.hpp"
#include "s3e/Support/Util.hpp"

namespace Tom::s3e {

    class ModelLoader {
        struct ObjMesh {
            std::string name;
            std::string usemtl;
            std::vector<Vertex> points;
        };

        std::string path;
        std::string parentPath() const;

        Parser p;

        /**
         * Clear all fields.
         */
        void clear();

        /**
         * Load a single mesh "o" from the current file.
         *
         * @param name the name of the mesh as described by "o"
         *
         * @return a pointer to the mesh or nullptr on failure
         */
        std::unique_ptr<ObjMesh> loadMesh(const std::string & name);

    public:
        /**
         * Create a new ModelLoader.
         */
        ModelLoader();

        /**
         * Load a Wavefront OBJ model file.
         *
         * @param path path to the model file
         *
         * @return a pointer to the Model object or nullptr on failure
         */
        Model::Ptr loadModel(const std::string & path);
    };
};
