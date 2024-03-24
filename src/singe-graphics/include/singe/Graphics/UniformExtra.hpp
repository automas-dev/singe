#pragma once

#include <glm/glm.hpp>
#include <glpp/Shader.hpp>
#include <memory>

namespace singe {
    using std::shared_ptr;
    using glm::mat2;
    using glm::mat3;
    using glm::mat4;
    using glm::vec2;
    using glm::vec3;
    using glm::vec4;

    /**
     * Wrapper for a uniform added to a Shader.
     *
     * This is an abstract base class with virtual send() method. This method is
     * implemented by the derived class to send their value to the appropriate
     * uniform type.
     */
    class UniformExtra {
    public:
        using Ptr = shared_ptr<UniformExtra>;
        using ConstPtr = const shared_ptr<UniformExtra>;

    protected:
        glpp::Uniform uniform;

    public:
        /**
         * Create a new UniformExtra.
         *
         * @param uniform the glpp::Uniform from the shader
         */
        UniformExtra(glpp::Uniform uniform);

        /**
         * Virtual destructor.
         */
        virtual ~UniformExtra();

        /**
         * Virtual method to be implemented by derived classes. This method
         * will be called when a shader is bound to update it's uniforms.
         */
        virtual void send() const = 0;
    };

    /**
     * UniformExtra of type bool
     */
    class BoolUniformExtra : public UniformExtra {
    public:
        using Ptr = shared_ptr<BoolUniformExtra>;
        using ConstPtr = const shared_ptr<BoolUniformExtra>;

        /// The uniform value
        bool value;

        /**
         * Create a new BoolUniformExtra.
         *
         * @param uniform the glpp::Uniform from the shader
         */
        BoolUniformExtra(glpp::Uniform uniform);

        /**
         * Send the uniform to the shader.
         */
        void send() const override;
    };

    /**
     * UniformExtra of type int
     */
    class IntUniformExtra : public UniformExtra {
    public:
        using Ptr = shared_ptr<IntUniformExtra>;
        using ConstPtr = const shared_ptr<IntUniformExtra>;

        /// The uniform value
        int value;

        /**
         * Create a new IntUniformExtra.
         *
         * @param uniform the glpp::Uniform from the shader
         */
        IntUniformExtra(glpp::Uniform uniform);

        /**
         * Send the uniform to the shader.
         */
        void send() const override;
    };

    /**
     * UniformExtra of type uint
     */
    class UIntUniformExtra : public UniformExtra {
    public:
        using Ptr = shared_ptr<UIntUniformExtra>;
        using ConstPtr = const shared_ptr<UIntUniformExtra>;

        /// The uniform value
        unsigned int value;

        /**
         * Create a new UIntUniformExtra.
         *
         * @param uniform the glpp::Uniform from the shader
         */
        UIntUniformExtra(glpp::Uniform uniform);

        /**
         * Send the uniform to the shader.
         */
        void send() const override;
    };

    /**
     * UniformExtra of type float
     */
    class FloatUniformExtra : public UniformExtra {
    public:
        using Ptr = shared_ptr<FloatUniformExtra>;
        using ConstPtr = const shared_ptr<FloatUniformExtra>;

        /// The uniform value
        float value;

        /**
         * Create a new FloatUniformExtra.
         *
         * @param uniform the glpp::Uniform from the shader
         */
        FloatUniformExtra(glpp::Uniform uniform);

        /**
         * Send the uniform to the shader.
         */
        void send() const override;
    };

    /**
     * UniformExtra of type vec2
     */
    class Vec2UniformExtra : public UniformExtra {
    public:
        using Ptr = shared_ptr<Vec2UniformExtra>;
        using ConstPtr = const shared_ptr<Vec2UniformExtra>;

        /// The uniform value
        vec2 value;

        /**
         * Create a new Vec2UniformExtra.
         *
         * @param uniform the glpp::Uniform from the shader
         */
        Vec2UniformExtra(glpp::Uniform uniform);

        /**
         * Send the uniform to the shader.
         */
        void send() const override;
    };

    /**
     * UniformExtra of type vec3
     */
    class Vec3UniformExtra : public UniformExtra {
    public:
        using Ptr = shared_ptr<Vec3UniformExtra>;
        using ConstPtr = const shared_ptr<Vec3UniformExtra>;

        /// The uniform value
        vec3 value;

        /**
         * Create a new Vec3UniformExtra.
         *
         * @param uniform the glpp::Uniform from the shader
         */
        Vec3UniformExtra(glpp::Uniform uniform);

        /**
         * Send the uniform to the shader.
         */
        void send() const override;
    };

    /**
     * UniformExtra of type vec4
     */
    class Vec4UniformExtra : public UniformExtra {
    public:
        using Ptr = shared_ptr<Vec4UniformExtra>;
        using ConstPtr = const shared_ptr<Vec4UniformExtra>;

        /// The uniform value
        vec4 value;

        /**
         * Create a new Vec4UniformExtra.
         *
         * @param uniform the glpp::Uniform from the shader
         */
        Vec4UniformExtra(glpp::Uniform uniform);

        /**
         * Send the uniform to the shader.
         */
        void send() const override;
    };

    /**
     * UniformExtra of type mat2
     */
    class Mat2UniformExtra : public UniformExtra {
    public:
        using Ptr = shared_ptr<Mat2UniformExtra>;
        using ConstPtr = const shared_ptr<Mat2UniformExtra>;

        /// The uniform value
        mat2 value;

        /**
         * Create a new Mat2UniformExtra.
         *
         * @param uniform the glpp::Uniform from the shader
         */
        Mat2UniformExtra(glpp::Uniform uniform);

        /**
         * Send the uniform to the shader.
         */
        void send() const override;
    };

    /**
     * UniformExtra of type mat3
     */
    class Mat3UniformExtra : public UniformExtra {
    public:
        using Ptr = shared_ptr<Mat3UniformExtra>;
        using ConstPtr = const shared_ptr<Mat3UniformExtra>;

        /// The uniform value
        mat3 value;

        /**
         * Create a new Mat3UniformExtra.
         *
         * @param uniform the glpp::Uniform from the shader
         */
        Mat3UniformExtra(glpp::Uniform uniform);

        /**
         * Send the uniform to the shader.
         */
        void send() const override;
    };

    /**
     * UniformExtra of type mat4
     */
    class Mat4UniformExtra : public UniformExtra {
    public:
        using Ptr = shared_ptr<Mat4UniformExtra>;
        using ConstPtr = const shared_ptr<Mat4UniformExtra>;

        /// The uniform value
        mat4 value;

        /**
         * Create a new Mat4UniformExtra.
         *
         * @param uniform the glpp::Uniform from the shader
         */
        Mat4UniformExtra(glpp::Uniform uniform);

        /**
         * Send the uniform to the shader.
         */
        void send() const override;
    };
}
