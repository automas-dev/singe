#include "s3e/Model.hpp"
#include <map>
#include "s3e/Util.hpp"
#include "s3e/VBO.hpp"
#include <iostream>


namespace Tom::s3e {
    Mesh::Mesh() : VBO() { }

    Mesh::Mesh(const std::string & name, const Material::ConstPtr & material, const std::vector<Vertex> & points)
        : name(name), material(material), VBO(points) { }

    Mesh::~Mesh() {
        VBO::~VBO();
    }

    const Material::ConstPtr & Mesh::getMaterial() const {
        return material;
    }

    Mesh::Ptr Mesh::create(const std::string & name, const Material::ConstPtr & material, const std::vector<Vertex> & points) {
        auto mesh = std::make_shared<Mesh>(name, material, points);
        return mesh;
    }
}

namespace Tom::s3e {
    static std::string pathParent(const std::string & path) {
        std::size_t i = path.find_last_of("/");
        if (i != std::string::npos) {
            return path.substr(0, i + 1);
        }
        return path;
    }

    Model::Model() : pos(0), rot(0), size(1) { }

    Model::Model(const std::string & path) : Model() {
        loadFromPath(path);
    }

    Model::~Model() { }

    bool Model::loadFromPath(const std::string & path) {

        std::string parent = pathParent(path);

        Parser p;
        if (p.open(path)) {
            struct Face {
                struct {
                    std::size_t v, t, n;
                } p[3];
            };

            struct ObjMesh {
                std::string name;
                std::string usemtl;
                std::vector<glm::vec3> av;
                std::vector<glm::vec2> avt;
                std::vector<glm::vec3> avn;
                std::vector<Face> af;
            };

            // TODO: Handle 4 point faces

            std::string mtllib;
            std::vector<std::unique_ptr<ObjMesh>> meshs;
            std::unique_ptr<ObjMesh> mesh (nullptr);

            for (std::string line = p.readLine(); !p.eof(); line = p.readLine()) {

                if (line.length() == 0 || strStartsWithChar('#', line))
                    continue;

                // TODO: Handle s
                // TODO: Warn for nRead errors bellow
                // TODO: Handle sign / unsigned read / store overflow

                if (strStartsWithStr("mtllib", line)) {
                    mtllib = line.substr(7);
                }
                else if (strStartsWithStr("o", line)) {
                    if (mesh) {
                        meshs.push_back(std::move(mesh));
                        mesh.reset();
                    }
                    mesh = std::make_unique<ObjMesh>();
                    mesh->name = line.substr(2);
                }
                else if (strStartsWithStr("usemtl", line)) {
                    mesh->usemtl = line.substr(7);
                }
                else if (strStartsWithStr("v ", line)) {
                    glm::vec3 v;
                    int nRead = sscanf(line.substr(2).c_str(), "%f %f %f", &v.x, &v.y, &v.z);
                    if (nRead == 3) {
                        mesh->av.push_back(v);
                    }
                }
                else if (strStartsWithStr("vt ", line)) {
                    glm::vec2 vt;
                    int nRead = sscanf(line.substr(3).c_str(), "%f %f", &vt.x, &vt.y);
                    if (nRead == 2) {
                        mesh->avt.push_back(vt);
                    }
                }
                else if (strStartsWithStr("vn ", line)) {
                    glm::vec3 vn;
                    int nRead = sscanf(line.substr(3).c_str(), "%f %f %f", &vn.x, &vn.y, &vn.z);
                    if (nRead == 3) {
                        mesh->avn.push_back(vn);
                    }
                }
                else if (strStartsWithChar('f', line)) {
                    Face f;
                    int nRead = sscanf(line.substr(2).c_str(), "%lu/%lu/%lu %lu/%lu/%lu %lu/%lu/%lu",
                                       &f.p[0].v, &f.p[0].t, &f.p[0].n,
                                       &f.p[1].v, &f.p[1].t, &f.p[1].n,
                                       &f.p[2].v, &f.p[2].t, &f.p[2].n);
                    if (nRead == 9) {
                        mesh->af.push_back(f);
                    }
                }
            }

            std::string mtlPath = parent + mtllib;
            MaterialLibrary::Ptr materials = MaterialLibrary::create(mtlPath);
            if (materials) {
                materials->name = mtllib;
            }
            else {
                std::cerr << "material fail load " << mtllib << std::endl;
                return false;
            }

            for (auto & mesh : meshs) {
                std::vector<Vertex> points;

                for (auto & face : mesh->af) {
                    for (int i = 0; i < 3; i++) {
                        if (face.p[i].v > mesh->av.size()
                                || face.p[i].n > mesh->avn.size()
                                || face.p[i].t > mesh->avt.size()) {
                            std::cerr << "Failed to load mesh " << mesh->name << " face index out of bounds" << std::endl;
                            break;
                        }

                        points.push_back({
                            mesh->av[face.p[i].v - 1],
                            mesh->avn[face.p[i].n - 1],
                            mesh->avt[face.p[i].t - 1]
                        });
                    }
                }

                Mesh::Ptr model = Mesh::create(mesh->name, materials->getMaterial(mesh->usemtl), points);
                models.push_back(model);
            }

            return true;
        }
        else {
            return false;
        }
    }

    void Model::move(glm::vec3 pos) {
        this->pos += pos;
    }

    void Model::rotate(glm::vec3 rot) {
        this->rot += rot;
    }

    void Model::scale(glm::vec3 scale) {
        this->size *= scale;
    }

    const glm::vec3 & Model::getPosition() const {
        return pos;
    }

    void Model::setPosition(glm::vec3 pos) {
        this->pos = pos;
    }

    const glm::vec3 & Model::getRotation() const {
        return rot;
    }

    void Model::setRotation(glm::vec3 rot) {
        this->rot = rot;
    }

    const glm::vec3 & Model::getScale() const {
        return size;
    }

    void Model::setScale(glm::vec3 scale) {
        this->size = scale;
    }


    glm::mat4 Model::modelMatrix() const {
        return matFromVecs(pos, rot, size);
    }

    Material::ConstPtr Model::getMaterial(const std::string & material) const {
        MaterialLibrary::ConstPtr lib = getMaterialLibrary(library);
        if (lib) {
            return lib->getMaterial(material);
        }
        return nullptr;
    }

    void Model::draw() const {
        // TODO: bind textures
        // TODO: uniforms
        for (auto & mesh : models) {
            auto & material = mesh->getMaterial();
            mesh->draw();
        }
        // TODO unbind textures
    }

    Model::Ptr Model::create(const std::string & path) {
        auto model = std::make_shared<Model>();
        if (model && model->loadFromPath(path)) {
            return model;
        }
        return nullptr;
    }
}
