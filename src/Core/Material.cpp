#include "s3e/Core/Material.hpp"

#include "s3e/Support/Util.hpp"
#include "s3e/Support/log.hpp"

namespace Tom::s3e {

    Material::Material() : Material("default") {}

    Material::Material(const std::string & name)
        : ambient(0.8),
          diffuse(0.8),
          specular(0.8),
          specularExponent(500),
          alpha(1),
          name(name) {}

    Material::~Material() {}

    void Material::print(std::ostream & os) const {
        os << "Material " << this->name << ":" << std::endl;
        os << "Ambient: " << this->ambient[0] << " " << this->ambient[1] << " "
           << this->ambient[2] << std::endl;
        os << "Diffuse: " << this->diffuse[0] << " " << this->diffuse[1] << " "
           << this->diffuse[2] << std::endl;
        os << "Specular: " << this->specular[0] << " " << this->specular[1]
           << " " << this->specular[2] << std::endl;
        os << "Specular Exponent: " << this->specularExponent << std::endl;
        os << "Alpha: " << this->alpha << std::endl;
    }
}

namespace Tom::s3e {
    void MaterialShader::setMaterial(const Material::ConstPtr & m) const {
        setVec3("material.ambient", m->ambient);
        setVec3("material.diffuse", m->diffuse);
        setVec3("material.specular", m->specular);
        setFloat("material.shininess", m->specularExponent);
        setFloat("material.alpha", m->alpha);
        setInt("material.texture", 0);
    }
}

namespace Tom::s3e {

    MaterialLibrary::MaterialLibrary() {}

    MaterialLibrary::~MaterialLibrary() {}

    bool MaterialLibrary::loadFromPath(const std::string & mtlPath) {
        path = mtlPath;

        Parser p;
        if (!p.open(path)) {
            Logging::Core->warning("Parser failed to open file {}", path);
            return false;
        }

        Material::Ptr curr;

#define PARSE_ERROR(TAG)                                                     \
    Logging::Core->error("could not parse {} tag while loading mtl file {}", \
                         (TAG), path)

        // TODO: Return line number from Parser
        for (std::string line = p.readLine(); !p.eof(); line = p.readLine()) {

            if (line.length() == 0 || strStartsWithChar('#', line))
                continue;

            if (strStartsWithStr("newmtl", line)) {
                if (line.size() < 8) {
                    PARSE_ERROR("newmtl");
                    return false;
                }

                std::string name = line.substr(7);

                if (curr)
                    addMaterial(curr);

                curr = std::make_shared<Material>();
                curr->name = name;
            }
            else if (strStartsWithStr("Ns", line)) {
                float Ns = 0;
                int nRead = sscanf(line.substr(3).c_str(), "%f", &Ns);
                if (nRead < 1) {
                    PARSE_ERROR("Ns");
                    return false;
                }
                else
                    curr->specularExponent = Ns;
            }
            else if (strStartsWithStr("Ka", line)) {
                glm::vec3 ambient;
                int nRead = sscanf(line.substr(3).c_str(), "%f %f %f",
                                   &ambient.x, &ambient.y, &ambient.z);
                if (nRead < 3) {
                    PARSE_ERROR("Ka");
                    return false;
                }
                else
                    curr->ambient = ambient;
            }
            else if (strStartsWithStr("Kd", line)) {
                glm::vec3 diffuse;
                int nRead = sscanf(line.substr(3).c_str(), "%f %f %f",
                                   &diffuse.x, &diffuse.y, &diffuse.z);
                if (nRead < 3) {
                    PARSE_ERROR("Kd");
                    return false;
                }
                else
                    curr->diffuse = diffuse;
            }
            else if (strStartsWithStr("Ks", line)) {
                glm::vec3 specular;
                int nRead = sscanf(line.substr(3).c_str(), "%f %f %f",
                                   &specular.x, &specular.y, &specular.z);
                if (nRead < 3) {
                    PARSE_ERROR("Ks");
                    return false;
                }
                else
                    curr->specular = specular;
            }
            else if (strStartsWithChar('d', line)) {
                float alpha;
                int nRead = sscanf(line.substr(2).c_str(), "%f", &alpha);
                if (nRead < 1) {
                    PARSE_ERROR("d");
                    return false;
                }
                else
                    curr->alpha = alpha;
            }
        }

#undef PARSE_ERROR

        if (curr)
            addMaterial(curr);

        return true;
    }

    void MaterialLibrary::addMaterial(Material::Ptr & material) {
        materials.push_back(material);
    }

    std::size_t MaterialLibrary::size() const {
        return materials.size();
    }

    Material::ConstPtr MaterialLibrary::getMaterial(int index) const {
        if (index < 0 || index >= size()) {
            Logging::Core->error(
                "MaterialLibrary does not contain a Material at index {} size is {}",
                index, size());
            return nullptr;
        }
        else
            return materials[index];
    }

    Material::ConstPtr MaterialLibrary::getMaterial(const std::string & name) const {
        for (auto & material : materials) {
            if (material->name == name)
                return material;
        }
        Logging::Core->error(
            "MaterialLibrary does not contain a Material named {}", name);
        return nullptr;
    }
}
