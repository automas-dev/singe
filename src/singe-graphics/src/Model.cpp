#include "singe/Graphics/Model.hpp"

#include <Wavefront.hpp>
#include <memory>
#include <vector>

namespace singe {
    using std::move;
    using std::make_shared;
    using std::vector;

    Model::Model() {}

    Model::Model(Model && other)
        : materials(move(other.materials)),
          geometry(move(other.geometry)),
          transform(move(other.transform)) {}

    Model & Model::operator=(Model && other) {
        materials = move(other.materials);
        geometry = move(other.geometry);
        transform = move(other.transform);
        return *this;
    }

    Model::~Model() {}

    void Model::draw(RenderState state) const {
        state.pushTransform(transform);
        for (auto & geom : geometry) {
            geom.draw(state);
        }
    }

    Model Model::fromPath(const string & path) {
        wavefront::Model wfModel;
        wfModel.loadModelFrom(path);

        Model model;
        for (auto * mat : wfModel.materials) {
            auto & material = model.materials.emplace_back();
            material.name = mat->name;
            material.ambient = mat->colAmbient;
            material.diffuse = mat->colDiffuse;
            material.specular = mat->colSpecular;
            material.specExp = mat->specExp;
            material.alpha = mat->alpha;
            if (!mat->texAlbedo.empty())
                material.texture =
                    make_shared<Texture>(Texture::fromPath(mat->texAlbedo));
            if (!mat->texNormal.empty())
                material.normalTexture =
                    make_shared<Texture>(Texture::fromPath(mat->texNormal));
            if (!mat->texSpecular.empty())
                material.specularTexture =
                    make_shared<Texture>(Texture::fromPath(mat->texSpecular));
        }

        for (auto * obj : wfModel.objects) {
            const Material * mat = &model.materials[obj->matId];
            auto & geom = model.geometry.emplace_back(mat);

            vector<Vertex> data;
            for (int i = 0; i < obj->size(); i++) {
                data.emplace_back(obj->vertices[i],
                                  obj->normals[i],
                                  obj->texcoords[i]);
            }

            geom.getMesh().bufferData(data);
        }

        return model;
    }
}
