#include "singe/Graphics/UniformExtra.hpp"

namespace singe {
    UniformExtra::UniformExtra(glpp::Uniform uniform) : uniform(uniform) {}

    UniformExtra::~UniformExtra() {}

    BoolUniformExtra::BoolUniformExtra(glpp::Uniform uniform)
        : UniformExtra(uniform), value(false) {}

    void BoolUniformExtra::send() const {
        uniform.setBool(value);
    }

    IntUniformExtra::IntUniformExtra(glpp::Uniform uniform)
        : UniformExtra(uniform), value(0) {}

    void IntUniformExtra::send() const {
        uniform.setInt(value);
    }

    UIntUniformExtra::UIntUniformExtra(glpp::Uniform uniform)
        : UniformExtra(uniform), value(0) {}

    void UIntUniformExtra::send() const {
        uniform.setUInt(value);
    }

    FloatUniformExtra::FloatUniformExtra(glpp::Uniform uniform)
        : UniformExtra(uniform), value(0) {}

    void FloatUniformExtra::send() const {
        uniform.setFloat(value);
    }

    Vec2UniformExtra::Vec2UniformExtra(glpp::Uniform uniform)
        : UniformExtra(uniform), value(0) {}

    void Vec2UniformExtra::send() const {
        uniform.setVec2(value);
    }

    Vec3UniformExtra::Vec3UniformExtra(glpp::Uniform uniform)
        : UniformExtra(uniform), value(0) {}

    void Vec3UniformExtra::send() const {
        uniform.setVec3(value);
    }

    Vec4UniformExtra::Vec4UniformExtra(glpp::Uniform uniform)
        : UniformExtra(uniform), value(0) {}

    void Vec4UniformExtra::send() const {
        uniform.setVec4(value);
    }

    Mat2UniformExtra::Mat2UniformExtra(glpp::Uniform uniform)
        : UniformExtra(uniform), value(0) {}

    void Mat2UniformExtra::send() const {
        uniform.setMat2(value);
    }

    Mat3UniformExtra::Mat3UniformExtra(glpp::Uniform uniform)
        : UniformExtra(uniform), value(0) {}

    void Mat3UniformExtra::send() const {
        uniform.setMat3(value);
    }

    Mat4UniformExtra::Mat4UniformExtra(glpp::Uniform uniform)
        : UniformExtra(uniform), value(0) {}

    void Mat4UniformExtra::send() const {
        uniform.setMat4(value);
    }
}
