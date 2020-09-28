#include "Model.hpp"
#include <map>
#include "Util.hpp"
#include "VBO.hpp"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>


namespace game {

    static std::string pathParent(const std::string &path) {
        std::size_t i = path.find_last_of("\\");
        if (i != std::string::npos) {
            return path.substr(0, i);
        }
        return path;
    }

    Model::Model() : VBO(), pos(0), rot(0), size(1) { }

    Model::Model(const std::string & path) : Model() {
        loadFromPath(path);
    }

    Model::Model(const std::vector<Vertex> & points, std::vector<MaterialLibrary::Ptr> materials) : Model() {
        loadFromPoints(points, materials);
    }

    Model::~Model() { }

    bool Model::loadFromPath(const std::string & path) {

        std::string parent = pathParent(path);

        Parser p;
        if (p.open(path)) {
            struct face {
                struct {
                    std::size_t v, t, n;
                } p[3];
                std::string material;
            };

            // TODO: Handle multiple objects in obj file
            // TODO: Handle 4 point faces

            std::vector<std::string> mtlNames;
            std::string usemtl;
            std::string name;
            std::vector<glm::vec3> av;
            std::vector<glm::vec2> avt;
            std::vector<glm::vec3> avn;
            std::vector<face> af;

            for (std::string line = p.readLine(); !p.eof(); line = p.readLine()) {
                std::cout << "Line: " << line << std::endl;

                if (line.length() == 0 || strStartsWithChar('#', line))
                    continue;

                // TODO: Handle o, s
                // TODO: Warn for nRead errors bellow
                // TODO: Handle sign / unsigned read / store overflow

                if (strStartsWithStr("mtllib", line)) {
                    std::string mtlName = line.substr(7);
                    mtlNames.push_back(mtlName);
                }
                else if (strStartsWithStr("usemtl", line)) {
                    usemtl = line.substr(7);
                }
                else if (strStartsWithStr("o", line)) {
                    name = line.substr(2);
                }
                else if (strStartsWithStr("v ", line)) {
                    glm::vec3 v;
                    int nRead = sscanf(line.substr(3).c_str(), "%f %f %f", &v.x, &v.y, &v.z);
                    if (nRead == 3) {
                        av.push_back(v);
                    }
                }
                else if (strStartsWithStr("vt", line)) {
                    glm::vec2 vt;
                    int nRead = sscanf(line.substr(3).c_str(), "%f %f", &vt.x, &vt.y);
                    if (nRead == 2) {
                        avt.push_back(vt);
                    }
                }
                else if (strStartsWithStr("vn", line)) {
                    glm::vec3 vn;
                    int nRead = sscanf(line.substr(3).c_str(), "%f %f %f", &vn.x, &vn.y, &vn.z);
                    if (nRead == 3) {
                        avn.push_back(vn);
                    }
                }
                else if (strStartsWithChar('f', line)) {
                    face f;
                    int nRead = sscanf(line.substr(2).c_str(), "%lu/%lu/%lu %lu/%lu/%lu %lu/%lu/%lu", 
                               &f.p[0].v, &f.p[0].t, &f.p[0].n,
                               &f.p[1].v, &f.p[1].t, &f.p[1].n,
                               &f.p[2].v, &f.p[2].t, &f.p[2].n);
                    if (nRead == 9) {
                        f.material = usemtl;
                        af.push_back(f);
                    }
                }
            }

            std::vector<Vertex> points;

            for (auto &face : af) {
                for (int i = 0; i < 3; i++) {
                    if (face.p[i].v > av.size()
                            || face.p[i].n > avn.size()
                            || face.p[i].t > avt.size()) {
                        // TODO: handle index out of bounds
                        break;
                    }

                    points.push_back({
                        av[face.p[i].v],
                        avn[face.p[i].n],
                        avt[face.p[i].t]
                    });
                }
            }

            std::vector<MaterialLibrary::Ptr> materials;
            for (auto &name : mtlNames) {
                std::string mtlPath = parent + name + ".mtl";
                MaterialLibrary::Ptr matlib = MaterialLibrary::create(mtlPath);
                if (matlib) {
                    matlib->name = name;
                    materials.push_back(matlib);
                }
                else {
                    // TODO: Handle fail to load mtl file
                }
            }

            return loadFromPoints(points, materials);
        }
        return false;
    }

    bool Model::loadFromPoints(const std::vector<Vertex> & points, std::vector<MaterialLibrary::Ptr> materials) {
        loadPoints(points);
        this->materials = materials;
        return true;
    }

    void Model::move(float x, float y, float z) {
        move({x, y, z});
    }

    void Model::move(glm::vec3 pos) {
        this->pos += pos;
    }

    void Model::rotate(float x, float y, float z) {
        rotate({x, y, z});
    }

    void Model::rotate(glm::vec3 rot) {
        this->rot += rot;
    }

    void Model::scale(float x, float y, float z) {
        scale({x, y, z});
    }

    void Model::scale(glm::vec3 scale) {
        this->size *= scale;
    }

    glm::mat4 Model::modelProjection() {
        glm::mat4 mpos = glm::translate(glm::mat4(), pos);
        glm::quat qrot (rot);
        glm::mat4 mrot = glm::toMat4(qrot);
        glm::mat4 msize = glm::scale(glm::mat4(), size);
        return mpos * mrot * msize;
    }

    MaterialLibrary::ConstPtr Model::getMaterialLibrary(const std::string & library) const {
        for (auto &mtllib : materials) {
            if (mtllib->name == library)
                return mtllib;
        }
        return nullptr;
    }
    
    Material::ConstPtr Model::getMaterial(const std::string & library, const std::string & material) const {
        MaterialLibrary::ConstPtr lib = getMaterialLibrary(library);
        if (lib) {
            return lib->getMaterial(material);
        }
        return nullptr;
    }

    void Model::draw() const{
        // TODO: bind textures
        // TODO: uniforms
        VBO::draw();
        // TODO unbind textures
    }

    Model::Ptr Model::create(const std::string & path) {
        auto model = std::make_shared<Model>();
        if (model && model->loadFromPath(path)) {
            return model;
        }
        return nullptr;
    }

    Model::Ptr Model::create(const std::vector<Vertex> & points, std::vector<MaterialLibrary::Ptr> materials) {
        auto model = std::make_shared<Model>();
        if (model && model->loadFromPoints(points, materials)) {
            return model;
        }
        return nullptr;
    }
}