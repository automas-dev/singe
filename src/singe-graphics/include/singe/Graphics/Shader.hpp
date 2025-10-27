#pragma once

#include <glpp/Shader.hpp>
#include <memory>
#include <string>
#include <vector>

#include "RenderState.hpp"
#include "UniformExtra.hpp"

namespace singe {
    using std::shared_ptr;
    using std::string;
    using std::vector;

    /**
     * Wrapper for glpp shader which also holds mvp uniform.
     */
    class Shader {
    public:
        using Ptr = shared_ptr<Shader>;
        using ConstPtr = const shared_ptr<Shader>;

    protected:
        glpp::Shader              m_shader;
        vector<UniformExtra::Ptr> m_extras;

    public:
        /**
         * Constructor that takes a glpp::Shader.
         *
         * @param shdaer the glpp::Shader
         */
        Shader(glpp::Shader && shader);

        virtual ~Shader();

        /**
         * Get a reference to the glpp::Shader.
         *
         * @return the glpp::Shader
         */
        const glpp::Shader & shader() const;

        /**
         * Get a glpp::Uniform for name from the glpp::Shader.
         *
         * @param name the uniform name
         *
         * @return the glpp::Uniform
         */
        glpp::Uniform uniform(const string & name) const;

        /**
         * Add an exta uniform to be applied when this shader is bound.
         *
         * @param extra a shared_ptr to a derived class of UniformExtra
         */
        void addExtra(UniformExtra::ConstPtr & extra);

        /**
         * Bind the shader
         */
        void bind() const;

        /**
         * Bind the shader and apply any extra uniforms.
         *
         * @param state the RenderState including transforms
         */
        virtual void bind(RenderState & state) const;

        /**
         * Unbind the shader, effectively binding 0.
         */
        void unbind() const;
    };

    /**
     * Derived Shader which holds a uniform called mvp. The glpp::Shader must
     * have a uniform called mvp of type mat4.
     */
    class MVPShader : public Shader {
    public:
        using Ptr = shared_ptr<MVPShader>;
        using ConstPtr = const shared_ptr<MVPShader>;

    private:
        glpp::Uniform m_mvp;

    public:
        /**
         * Constructor that takes a glpp::Shader. This shader must have a
         * uniform called mvp of type mat4.
         *
         * @param shader the glpp::Shader which has a mat4 uniform called mvp
         */
        MVPShader(glpp::Shader && shader);

        /**
         * Get a reference to the mvp glpp::Uniform.
         *
         * @return the glpp::Uniform called mvp
         */
        const glpp::Uniform & mvp() const;

        /**
         * Bind the shader and apply the mvp uniform.
         *
         * @param state the RenderState including transforms
         */
        void bind(RenderState & state) const override;
    };
}
