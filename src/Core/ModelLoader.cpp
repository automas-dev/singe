#include "s3e/Core/ModelLoader.hpp"

#include "s3e/Support/log.hpp"

namespace Tom::s3e::Logging {
    static Logger::Ptr ModelLoader = std::make_shared<Logger>("ModelLoader");
}

#define PARSE_ERROR(TAG)                                              \
    {                                                                 \
        Logging::ModelLoader->error("Failed to parse tag {}", (TAG)); \
        return nullptr;                                               \
    }

#define ASSERT_NREAD(N, TAG) \
    if (nRead != (N))        \
        PARSE_ERROR(TAG);

#define ASSERT_LINE_SIZE(N, TAG) \
    if (line.size() < (N))       \
        PARSE_ERROR(TAG);

namespace Tom::s3e {
    struct Face {
        struct {
            std::size_t v, t, n;
        } p[3];
    };

    std::string ModelLoader::parentPath() const {
        std::size_t i = path.find_last_of("/");
        if (i != std::string::npos) {
            return path.substr(0, i + 1);
        }
        return path;
    }

    void ModelLoader::clear() {
        path.clear();
        p.close();
    }

    std::unique_ptr<ModelLoader::ObjMesh> ModelLoader::loadMesh(
        const std::string & name) {
        Logging::ModelLoader->debug("Loading sub-model {}", name);

        auto mesh = std::make_unique<ObjMesh>();
        mesh->name = name;

        std::vector<glm::vec3> av;
        std::vector<glm::vec2> avt;
        std::vector<glm::vec3> avn;
        std::vector<Face> af;

        for (std::string line = p.readLine(); !p.eof(); line = p.readLine()) {
            if (line.length() == 0 || strStartsWithChar('#', line))
                continue;

            if (strStartsWithStr("o", line)) {
                p.rewind(line.size());
                break;
            }

            if (strStartsWithStr("usemtl", line)) {
                ASSERT_LINE_SIZE(8, "usemtl");
                mesh->usemtl = line.substr(7);
            }
            else if (strStartsWithStr("v ", line)) {
                glm::vec3 v;
                int nRead =
                    sscanf(line.substr(2).c_str(), "%f %f %f", &v.x, &v.y, &v.z);
                ASSERT_NREAD(3, "v");
                av.push_back(v);
            }
            else if (strStartsWithStr("vt ", line)) {
                glm::vec2 vt;
                int nRead = sscanf(line.substr(3).c_str(), "%f %f", &vt.x, &vt.y);
                ASSERT_NREAD(2, "vT");
                avt.push_back(vt);
            }
            else if (strStartsWithStr("vn ", line)) {
                glm::vec3 vn;
                int nRead = sscanf(line.substr(3).c_str(), "%f %f %f", &vn.x,
                                   &vn.y, &vn.z);
                ASSERT_NREAD(3, "vn");
                avn.push_back(vn);
            }
            else if (strStartsWithChar('f', line)) {
                Face f;
                int nRead = sscanf(
                    line.substr(2).c_str(), "%lu/%lu/%lu %lu/%lu/%lu %lu/%lu/%lu",
                    &f.p[0].v, &f.p[0].t, &f.p[0].n, &f.p[1].v, &f.p[1].t,
                    &f.p[1].n, &f.p[2].v, &f.p[2].t, &f.p[2].n);
                ASSERT_NREAD(9, "f");
                af.push_back(f);
            }
        }

        for (auto & face : af) {
            for (int i = 0; i < 3; i++) {
                auto & p = face.p[i];

                if (p.v > av.size()) {
                    Logging::ModelLoader->error(
                        "Failed to load mesh {} vertex index {} is out of bounds {}",
                        mesh->name, p.v, av.size());
                    return nullptr;
                }

                if (p.n > avn.size()) {
                    Logging::ModelLoader->error(
                        "Failed to load mesh {} normal index {} is out of bounds {}",
                        mesh->name, p.n, avn.size());
                    return nullptr;
                }

                if (p.t > avt.size()) {
                    Logging::ModelLoader->error(
                        "Failed to load mesh {} texture coordinate index {} is out of bounds {}",
                        mesh->name, p.t, avt.size());
                    return nullptr;
                }

                mesh->points.push_back({av[face.p[i].v - 1], avn[face.p[i].n - 1],
                                        avt[face.p[i].t - 1]});
            }
        }

        return mesh;
    }

    ModelLoader::ModelLoader() {}

    Model::Ptr ModelLoader::loadModel(const std::string & path) {
        Logging::ModelLoader->debug("Loading {}", path);
        clear();

        this->path = path;
        if (!p.open(path)) {
            Logging::ModelLoader->error("Failed to open file {}", path);
            return nullptr;
        }

        // TODO: Handle 4 point faces

        auto model = std::make_shared<Model>();

        for (std::string line = p.readLine(); !p.eof(); line = p.readLine()) {
            if (line.length() == 0 || strStartsWithChar('#', line))
                continue;

            // TODO: Handle s
            // TODO: Warn for nRead errors bellow
            // TODO: Handle sign / unsigned read / store overflow

            if (strStartsWithStr("o", line)) {
                Logging::ModelLoader->trace("Got object {}", line.substr(2));
                auto mesh = loadMesh(line.substr(2));
                if (!mesh)
                    return nullptr;
                model->appendPoints(mesh->points);
            }
        }

        model->send();
        Logging::ModelLoader->debug("Loaded {} points", model->points.size());

        return model;
    }
};