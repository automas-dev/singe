#pragma once

#include <glm/glm.hpp>
#include <glpp/extra/Transform.hpp>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace fast {
    using std::string;
    using std::string_view;
    using std::vector;
    using std::map;
    using std::shared_ptr;
    using std::weak_ptr;
    using glm::vec3;
    using glm::vec4;
    using glpp::extra::Transform;

    class Node {
    public:
        using Ptr = shared_ptr<Node>;

        virtual string_view getType() const {
            return "Node";
        }

        Node * parent;
        string name;
        Transform transform;
        vector<Node::Ptr> children;

        Node(Node * parent = nullptr, const string_view & name = "")
            : parent(parent), name(name) {}

        Node(const Node & other) = delete;
        Node(Node && other) = delete;

        Node & operator=(const Node & other) = delete;
        Node & operator=(Node && other) = delete;

        virtual ~Node() {}

        void addChild(Node::Ptr child) {
            if (child->parent) {
                child->parent->removeChild(child);
            }
            child->parent = this;
            children.push_back(child);
        }

        bool hasChild(const Node::Ptr & child) {
            for (auto & curr : children) {
                if (curr == child)
                    return true;
            }
            return false;
        }

        void removeChild(const Node::Ptr & child) {
            for (auto curr = children.begin(); curr != children.end(); curr++) {
                if ((*curr) == child) {
                    children.erase(curr);
                    child->parent = nullptr;
                    return;
                }
            }
        }

        Node::Ptr findChild(const string_view & name, bool recursive = true) {
            for (auto & curr : children) {
                if (curr->name == name)
                    return curr;
            }

            if (!recursive) {
                Node::Ptr child;
                for (auto & curr : children) {
                    if (child = curr->findChild(name))
                        return curr;
                }
            }

            return nullptr;
        }
    };

    class CameraNode : public Node {
    public:
        using Ptr = shared_ptr<CameraNode>;

        string_view getType() const override {
            return "Camera";
        }

        enum Control {
            FPV,
            Dolly,
        };

    private:
        Control control = FPV;
        bool active = true;

        singe::Camera::ProjectionMode mode = singe::Camera::Perspective;
        float fov = 73;
        float near = 0.1;
        float far = 1000;

    public:
        using Node::Node;

        Control getControl() const {
            return control;
        }

        void setControl(Control control) {
            this->control = control;
        }

        bool isActive() const {
            return active;
        }

        void setActive(bool active) {
            this->active = active;
        }

        singe::Camera::ProjectionMode getProjectionMode() const {
            return mode;
        }

        void setProjectionMode(singe::Camera::ProjectionMode mode) {
            this->mode = mode;
        }

        float getFov() const {
            return fov;
        }

        void setFov(float fov) {
            this->fov = fov;
        }

        float getNear() const {
            return near;
        }

        void setNear(float near) {
            this->near = near;
        }

        float getFar() const {
            return far;
        }

        void setFar(float far) {
            this->far = far;
        }
    };

    class ImageNode : public Node {
    public:
        using Ptr = shared_ptr<ImageNode>;

        string_view getType() const override {
            return "Image";
        }

    private:
        string_view path;
        glpp::Texture::Format channels = glpp::Texture::RGBA;

    public:
        using Node::Node;

        const string_view & getPath() const {
            return path;
        }

        void setPath(const string_view & path) {
            this->path = path;
        }

        glpp::Texture::Format getChannels() const {
            return channels;
        }

        void setChannels(glpp::Texture::Format channels) {
            this->channels = channels;
        }
    };

    class MaterialNode : public Node {
    public:
        using Ptr = shared_ptr<MaterialNode>;

        string_view getType() const override {
            return "Material";
        }

    private:
        vec4 colorDiffuse = vec4(1);
        ImageNode::Ptr texDiffuse;

    public:
        using Node::Node;

        vec4 getColorDiffuse() const {
            return colorDiffuse;
        }

        void setColorDiffuse(vec4 colorDiffuse) {
            this->colorDiffuse = colorDiffuse;
        }

        ImageNode::Ptr getDiffuseImage() const {
            return texDiffuse;
        }

        void setDiffuseImage(ImageNode::Ptr texDiffuse) {
            this->texDiffuse = texDiffuse;
        }
    };

    // TODO this does not need to inherit from Node, there are no children or transform
    class MeshNode : public Node {
    public:
        using Ptr = shared_ptr<MeshNode>;

        string_view getType() const override {
            return "Mesh";
        }

    private:
        string_view path;
        bool visible = true;
        MaterialNode::Ptr material;

    public:
        MeshNode(Node * parent = nullptr,
                 const string_view & name = "",
                 const string_view & path = "")
            : Node(parent, name), path(path) {}

        const string_view & getPath() const {
            return path;
        }

        void setPath(const string_view & path) {
            this->path = path;
        }

        bool getVisible() const {
            return visible;
        }

        void setVisible(bool visible) {
            this->visible = visible;
        }

        MaterialNode::Ptr getMaterial() const {
            return material;
        }

        void setMaterial(MaterialNode::Ptr material) {
            this->material = material;
        }
    };

    class Root {
    public:
        using Ptr = shared_ptr<Root>;

    private:
        map<string, CameraNode::Ptr::weak_type> cameras;
        map<string, MeshNode::Ptr::weak_type> meshs;
        map<string, MaterialNode::Ptr::weak_type> materials;
        map<string, ImageNode::Ptr::weak_type> images;

    public:
        Node::Ptr rootScene;

        Root() {}

        Root(const Node::Ptr & rootScene)
            : rootScene(rootScene) {
        }

        Root(const Root &) = delete;
        Root(Root && other) = delete;

        Root & operator=(const Root &) = delete;
        Root & operator=(Root && other) = delete;

        bool hasCamera(const string & name) {
            auto it = cameras.find(name);
            if (it == cameras.end())
                return false;
            assert(!it->second.expired());
            return true;
        }

        CameraNode::Ptr::weak_type & getCamera(const string & name) {
            return cameras[name];
        }

        CameraNode::Ptr instanceCamera(const string & name) {
            return cameras[name].lock();
        }

        void addCamera(const string & name, CameraNode::Ptr::weak_type camera) {
            assert(!camera.expired());
            this->cameras[name] = camera;
        }

        void addCamera(CameraNode::Ptr & camera) {
            addCamera(camera->name, camera);
        }

        bool hasMesh(const string & name) {
            auto it = meshs.find(name);
            if (it == meshs.end())
                return false;
            assert(!it->second.expired());
            return true;
        }

        MeshNode::Ptr::weak_type & getMesh(const string & name) {
            return meshs[name];
        }

        MeshNode::Ptr instanceMesh(const string & name) {
            return meshs[name].lock();
        }

        void addMesh(const string & name, MeshNode::Ptr::weak_type mesh) {
            assert(!mesh.expired());
            this->meshs[name] = mesh;
        }

        void addMesh(MeshNode::Ptr & mesh) {
            addMesh(mesh->name, mesh);
        }

        bool hasMaterial(const string & name) {
            auto it = materials.find(name);
            if (it == materials.end())
                return false;
            assert(!it->second.expired());
            return true;
        }

        MaterialNode::Ptr::weak_type & getMaterial(const string & name) {
            return materials[name];
        }

        MaterialNode::Ptr instanceMaterial(const string & name) {
            return materials[name].lock();
        }

        void addMaterial(const string & name, MaterialNode::Ptr::weak_type material) {
            assert(!material.expired());
            this->materials[name] = material;
        }

        void addMaterial(MaterialNode::Ptr & material) {
            addMaterial(material->name, material);
        }

        bool hasImage(const string & name) {
            auto it = images.find(name);
            if (it == images.end())
                return false;
            assert(!it->second.expired());
            return true;
        }

        ImageNode::Ptr::weak_type & getImage(const string & name) {
            return images[name];
        }

        ImageNode::Ptr instanceImage(const string & name) {
            return images[name].lock();
        }

        void addImage(const string & name, ImageNode::Ptr::weak_type image) {
            assert(!image.expired());
            this->images[name] = image;
        }

        void addImage(ImageNode::Ptr & image) {
            addImage(image->name, image);
        }
    };
}
