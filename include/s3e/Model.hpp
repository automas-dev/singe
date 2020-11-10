#pragma once

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include "s3e/VBO.hpp"
#include "s3e/Material.hpp"

namespace Tom::s3e {

    class Mesh : public VBO {
        std::string name;
        Material::ConstPtr material;

    public:
        typedef std::shared_ptr<Mesh> Ptr;
        typedef std::shared_ptr<const Mesh> ConstPtr;

        Mesh(void);
        Mesh(const std::string & name, const Material::ConstPtr & material, const std::vector<Vertex> & points);
        virtual ~Mesh();

        const Material::ConstPtr & getMaterial(void) const;

        static Ptr create(const std::string & name, const Material::ConstPtr & material, const std::vector<Vertex> & points);
    };

    class Model {
        glm::vec3 pos;
        glm::vec3 rot;
        glm::vec3 size;

        std::vector<Mesh::Ptr> models;
        std::string name;

    public:
        typedef std::shared_ptr<Model> Ptr;
        typedef std::shared_ptr<const Model> ConstPtr;

        Model(void);
        Model(const std::string & path);
        virtual ~Model();

        bool loadFromPath(const std::string & path);

        void move(glm::vec3 pos);
        void rotate(glm::vec3 rot);
        void scale(glm::vec3 scale);

        const glm::vec3 & getPosition(void) const;
        void setPosition(glm::vec3 pos);

        const glm::vec3 & getRotation(void) const;
        void setRotation(glm::vec3 rot);

        const glm::vec3 & getScale(void) const;
        void setScale(glm::vec3 scale);

        glm::mat4 modelMatrix() const;

        Material::ConstPtr getMaterial(const std::string & material) const;

        void draw(const MaterialShader::Ptr & shader) const;

        static Ptr create(const std::string & path);
    };
};