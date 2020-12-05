#include "s3e/Model.hpp"
#include <map>
#include "s3e/Util.hpp"
#include "s3e/VBO.hpp"
#include "s3e/log.hpp"

namespace Tom::s3e {
    Mesh::Mesh() : VBO() { }

    Mesh::Mesh(const std::string & name, const Material::ConstPtr & material) : name(name), material(material), VBO() { }

    Mesh::~Mesh() {
        VBO::~VBO();
    }

    const Material::ConstPtr & Mesh::getMaterial() const {
        return material;
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

    Model::Model() : pos(0), rot(0), size(1), materials(std::make_shared<MaterialLibrary>()) { }

    Model::~Model() { }

    bool Model::loadFromPath(const std::string & objPath) {

        path = objPath;
        std::string parent = pathParent(path);

        Parser p;
        if (!p.open(path)) {
            SPDLOG_WARN("Parser failed to open file {}", path);
            return false;
        }

        struct Face {
            struct {
                std::size_t v, t, n;
            } p[3];
        };

        std::vector<glm::vec3> av;
        std::vector<glm::vec2> avt;
        std::vector<glm::vec3> avn;

        struct ObjMesh {
            std::string name;
            std::string usemtl;
            std::vector<Face> af;
        };

        // TODO: Handle 4 point faces

        std::string mtllib;
        std::vector<std::unique_ptr<ObjMesh>> meshs;
        std::unique_ptr<ObjMesh> mesh (nullptr);

#define PARSE_ERROR(TAG) SPDLOG_ERROR("could not parse {} tag while loading mtl file {}", (TAG), path)

        for (std::string line = p.readLine(); !p.eof(); line = p.readLine()) {

            if (line.length() == 0 || strStartsWithChar('#', line))
                continue;

            // TODO: Handle s
            // TODO: Warn for nRead errors bellow
            // TODO: Handle sign / unsigned read / store overflow

            if (strStartsWithStr("mtllib", line)) {
                if (line.size() < 8) {
                    PARSE_ERROR("mtllib");
                    return false;
                }
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
                if (line.size() < 8) {
                    PARSE_ERROR("usemtl");
                    return false;
                }
                mesh->usemtl = line.substr(7);
            }
            else if (strStartsWithStr("v ", line)) {
                glm::vec3 v;
                int nRead = sscanf(line.substr(2).c_str(), "%f %f %f", &v.x, &v.y, &v.z);
                if (nRead < 3) {
                    PARSE_ERROR("v");
                    return false;
                }
                av.push_back(v);
            }
            else if (strStartsWithStr("vt ", line)) {
                glm::vec2 vt;
                int nRead = sscanf(line.substr(3).c_str(), "%f %f", &vt.x, &vt.y);
                if (nRead < 2) {
                    PARSE_ERROR("vt");
                    return false;
                }
                avt.push_back(vt);
            }
            else if (strStartsWithStr("vn ", line)) {
                glm::vec3 vn;
                int nRead = sscanf(line.substr(3).c_str(), "%f %f %f", &vn.x, &vn.y, &vn.z);
                if (nRead < 3) {
                    PARSE_ERROR("vn");
                    return false;
                }
                avn.push_back(vn);
            }
            else if (strStartsWithChar('f', line)) {
                Face f;
                int nRead = sscanf(line.substr(2).c_str(), "%lu/%lu/%lu %lu/%lu/%lu %lu/%lu/%lu",
                                   &f.p[0].v, &f.p[0].t, &f.p[0].n,
                                   &f.p[1].v, &f.p[1].t, &f.p[1].n,
                                   &f.p[2].v, &f.p[2].t, &f.p[2].n);
                if (nRead < 9) {
                    PARSE_ERROR("f");
                    return false;
                }
                mesh->af.push_back(f);
            }
        }

        if (mesh) {
            meshs.push_back(std::move(mesh));
            mesh.reset();
        }

#undef PARSE_ERROR

        SPDLOG_DEBUG("Loaded {} meshes", meshs.size());

        std::string mtlPath = parent + mtllib;

        if (materials->loadFromPath(mtlPath)) {
            materials->name = mtllib;
        }
        else {
            SPDLOG_ERROR("failed to load material {}", mtllib);
            return false;
        }

        for (auto & mesh : meshs) {
            std::vector<Vertex> points;

            for (auto & face : mesh->af) {
                for (int i = 0; i < 3; i++) {
                    if (face.p[i].v > av.size()) {
                        SPDLOG_ERROR("failed to load mesh {} vertex index {} is out of bounds {}", mesh->name, face.p[i].v, av.size());
                        return false;
                    }
                    else if (face.p[i].n > avn.size()) {
                        SPDLOG_ERROR("failed to load mesh {} normal index {} is out of bounds {}", mesh->name, face.p[i].n, avn.size());
                        return false;
                    }
                    else if (face.p[i].t > avt.size()) {
                        SPDLOG_ERROR("failed to load mesh {} texture coordinate index {} is out of bounds {}", mesh->name, face.p[i].t,
                                     avt.size());
                        return false;
                    }
                    else {
                        points.push_back({
                            av[face.p[i].v - 1],
                            avn[face.p[i].n - 1],
                            avt[face.p[i].t - 1]
                        });
                    }
                }
            }

            auto model = std::make_shared<Mesh>(mesh->name, materials->getMaterial(mesh->usemtl));
            if (!model->loadFromPoints(points)) {
                SPDLOG_ERROR("failed in call to Mesh::loadPoints while loading Model {}", path);
                return false;
            }
            models.push_back(model);
        }

        return true;
    }

    bool Model::loadFromPoints(const std::vector<Vertex> & points) {
        auto material = std::make_shared<Material>();
        return loadFromPoints(points, material);
    }

    bool Model::loadFromPoints(const std::vector<Vertex> & points, Material::Ptr & material) {
        materials->addMaterial(material);

        auto mesh = std::make_shared<Mesh>("mesh", material);
        if (!mesh->loadFromPoints(points)) {
            SPDLOG_ERROR("failed in call to Mesh::loadFromPoints()");
            return false;
        }

        models.push_back(mesh);
        return true;
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

    Material::ConstPtr Model::getMaterial(const std::string & name) const {
        auto material = materials->getMaterial(name);
        if (!material)
            SPDLOG_ERROR("failed in call to MaterialLibrary::getMaterial(name={})", name);
        return material;
    }

    void Model::draw() const {
        // TODO: bind textures
        for (auto & mesh : models) {
            mesh->draw();
        }
        // TODO unbind textures
    }

    void Model::draw(const MaterialShader::Ptr & shader) const {
        // TODO: bind textures
        // TODO: uniforms
        for (auto & mesh : models) {
            auto & material = mesh->getMaterial();
            shader->setMaterial(material);
            mesh->draw();
        }
        // TODO unbind textures
    }
}

