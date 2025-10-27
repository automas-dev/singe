#pragma once

#include <glm/glm.hpp>
#include <glpp/Shader.hpp>
#include <glpp/extra/Camera.hpp>
#include <glpp/extra/Transform.hpp>

namespace singe {
    using glm::mat4;
    using glpp::extra::Camera;
    using glpp::extra::Transform;

    /**
     * Store current transform state. Pass this by value to create a stack
     * effect of applying transforms in layers.
     *
     * Model is also known as world or global in other software.
     *
     * **note** local should only be the last transform applied to model. model
     * should be the latest local.
     */
    class RenderState {
        mat4 projection;
        mat4 view;
        mat4 model;
        mat4 local;
        bool drawGrid;

    public:
        /**
         * Create a RenderState with identity for each matrix.
         */
        RenderState();

        /**
         * Create a RenderState from all transforms.
         *
         * @param projection the projection matrix
         * @param view the view matrix
         * @param model the model matrix
         * @param local the local matrix
         * @param drawGrid should a grid be drawn if present
         */
        RenderState(const mat4 & projection,
                    const mat4 & view,
                    const mat4 & model,
                    const mat4 & local,
                    bool         drawGrid = false);

        /**
         * Create a RenderState with initial transform.
         *
         * @param camera Camera object used for projection and view matrix
         * @param model the model matrix
         * @param local the local matrix
         */
        RenderState(const Camera & camera,
                    const mat4 &   model = mat4(1),
                    const mat4 &   local = mat4(1),
                    bool           drawGrid = false);

        ~RenderState();

        /**
         * @brief Will a grid be drawn if present.
         *
         * @return is grid draw enabled
         */
        bool getGridEnable() const;

        /**
         * @brief Enable or disable grid draw if a grid is present.
         *
         * @param enabled state of grid draw
         */
        void setGridEnable(bool enabled);

        /**
         * Get the vp transform.
         *
         * VP  = projection * view
         *
         * @return the resulting matrix
         */
        mat4 getVP() const;

        /**
         * Get the mvp transform.
         *
         * MVP = projection * view * model
         *
         * @return the resulting matrix
         */
        mat4 getMVP() const;

        /**
         * Get the model transform.
         *
         * @return the model matrix
         */
        const mat4 & getModel() const;

        /**
         * Get the local transform.
         *
         * @return the local matrix
         */
        const mat4 & getLocal() const;

        /**
         *  Call pushTransform with the Transform matrix.
         *
         * @param transform the Transform to use for the matrix parameter
         */
        void pushTransform(const Transform & transform);

        /**
         * Replace the local transform with this and multiply the model
         * transform with it.
         *
         * @param matrix the matrix used to multiply model and replace local
         */
        void pushTransform(const mat4 & matrix);
    };
}
