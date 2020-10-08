#pragma once

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include "VBO.hpp"
#include "Material.hpp"

namespace game {

    class Model : VBO, public std::enable_shared_from_this<Model> {
        glm::vec3 pos;
        glm::vec3 rot;
        glm::vec3 size;

        std::vector<MaterialLibrary::Ptr> materials;
        std::string name;

    public:
        typedef std::shared_ptr<Model> Ptr;
        typedef std::shared_ptr<const Model> ConstPtr;

        Model(void);
        Model(const std::string & path);
        Model(const std::vector<Vertex> & points, std::vector<MaterialLibrary::Ptr> materials);
        virtual ~Model();

        bool loadFromPath(const std::string & path);

        bool loadFromPoints(const std::vector<Vertex> & points, std::vector<MaterialLibrary::Ptr> materials);

        void move(float x, float y, float z);
        void move(glm::vec3 pos);

        void rotate(float x, float y, float z);
        void rotate(glm::vec3 rot);

        void scale(float x, float y, float z);
        void scale(glm::vec3 scale);

        const glm::vec3 &getPosition(void) const;
        void setPosition(float x, float y, float z);
        void setPosition(glm::vec3 pos);

        const glm::vec3 &getRotation(void) const;
        void setRotation(float x, float y, float z);
        void setRotation(glm::vec3 rot);

        const glm::vec3 &getScale(void) const;
        void setScale(float x, float y, float z);
        void setScale(glm::vec3 scale);

        glm::mat4 modelMatrix() const;

        Material::ConstPtr getFirstMaterial(void) const;

        MaterialLibrary::ConstPtr getMaterialLibrary(const std::string & library) const;

        Material::ConstPtr getMaterial(const std::string & library, const std::string & material) const;

        void draw(void) const;

        static Ptr create(const std::string & path);
        static Ptr create(const std::vector<Vertex> & points, std::vector<MaterialLibrary::Ptr> materials);
    };
};