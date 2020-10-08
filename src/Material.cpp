#include "Material.hpp"
#include "Util.hpp"
#include <iostream>

namespace game {
    void MaterialUniformLocations::loadFromShader(const Shader::ConstPtr &shader) {
        proj = shader->uniformLocation("proj");
        view = shader->uniformLocation("view");
        model = shader->uniformLocation("model");

        lightPos = shader->uniformLocation("lightPos");
        viewPos = shader->uniformLocation("viewPos");
        
        ambient = shader->uniformLocation("ambient");
        diffuse = shader->uniformLocation("diffuse");
        specular = shader->uniformLocation("specular");

        specExp = shader->uniformLocation("specExp");
        alpha = shader->uniformLocation("alpha");
    }
}

namespace game {

    Material::Material() : ambient(0), diffuse(0), specular(0), specularExponent(0), alpha(1) { }

    Material::~Material() { }

    void Material::print(std::ostream & os) const {
        os << "Material " << this->name << ":" << std::endl;
        os << "Ambient: " << this->ambient[0] << " " << this->ambient[1]  << " " << this->ambient[2] << std::endl;
        os << "Diffuse: " << this->diffuse[0] << " " << this->diffuse[1]  << " " << this->diffuse[2] << std::endl;
        os << "Specular: " << this->specular[0] << " " << this->specular[1]  << " " << this->specular[2] << std::endl;
        os << "Specular Exponent: " << this->specularExponent << std::endl;
        os << "Alpha: " << this->alpha << std::endl;
    }

    Material::Ptr Material::create() {
        auto material = std::make_shared<Material>();
        return material;
    }
}

namespace game {

    MaterialLibrary::MaterialLibrary() { }

    MaterialLibrary::MaterialLibrary(const std::string & path) {
        loadFromPath(path);
    }

    MaterialLibrary::~MaterialLibrary() { }

    bool MaterialLibrary::loadFromPath(const std::string & path) {
        Parser p;
        if (p.open(path)) {
            Material::Ptr curr;

            // for (std::string line = p.readLine(); line.empty(); line = p.readLine()) {
            for (std::string line = p.readLine(); !p.eof(); line = p.readLine()) {

                if (line.length() == 0 || strStartsWithChar('#', line))
                    continue;

                if (strStartsWithStr("newmtl", line)) {
                    std::string name = line.substr(7);

                    if (curr)
                        materials.push_back(curr);

                    curr = Material::create();
                    curr->name = name;
                }
                else if (strStartsWithStr("Ns", line)) {
                    float Ns = 0;
                    int nRead = sscanf(line.substr(3).c_str(), "%f", &Ns);
                    if (nRead == 1) {
                        curr->specularExponent = Ns;
                    }
                }
                else if (strStartsWithStr("Ka", line)) {
                    glm::vec3 ambient;
                    int nRead = sscanf(line.substr(3).c_str(), "%f %f %f", &ambient.x, &ambient.y, &ambient.z);
                    if (nRead == 3) {
                        curr->ambient = ambient;
                    }
                }
                else if (strStartsWithStr("Kd", line)) {
                    glm::vec3 diffuse;
                    int nRead = sscanf(line.substr(3).c_str(), "%f %f %f", &diffuse.x, &diffuse.y, &diffuse.z);
                    if (nRead == 3) {
                        curr->diffuse = diffuse;
                    }
                }
                else if (strStartsWithStr("Ks", line)) {
                    glm::vec3 specular;
                    int nRead = sscanf(line.substr(3).c_str(), "%f %f %f", &specular.x, &specular.y, &specular.z);
                    if (nRead == 3) {
                        curr->specular = specular;
                    }
                }
                else if (strStartsWithChar('d', line)) {
                    float alpha;
                    int nRead = sscanf(line.substr(2).c_str(), "%f", &alpha);
                    if (nRead == 1) {
                        curr->alpha = alpha;
                    }
                }
            }

            if (curr)
                materials.push_back(curr);
            
            return true;
        }

        std::cerr << "failed to open " << path << std::endl;

        return false;
    }

    std::size_t MaterialLibrary::size() const {
        return materials.size();
    }

    Material::ConstPtr MaterialLibrary::getMaterial(int index) const {
        if (index < 0 || index >= size())
            return nullptr;
        else
            return materials[index];
    }

    Material::ConstPtr MaterialLibrary::getMaterial(const std::string & name) const {
        for (auto &material : materials) {
            if (material->name == name)
                return material;
        }
        return nullptr;
    }

    MaterialLibrary::Ptr MaterialLibrary::create(const std::string & path) {
        auto matlib = std::make_shared<MaterialLibrary>();
        if (matlib && matlib->loadFromPath(path)) {
            return matlib;
        }
        return nullptr;
    }
}
