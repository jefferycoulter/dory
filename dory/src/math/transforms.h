#ifndef DORY_TRANSFORM_INCL
#define DORY_TRANSFORM_INCL

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace DORY
{
    /**
     * @brief struct containing object transformation data. by default this is 3D, but
     * it can be used for 2D objects as well by setting one component to zero.
     */
    struct TransformObject
    {
        glm::vec3 translation{};
        glm::vec3 scale{1.0f, 1.0f, 1.0f};
        glm::vec3 rotation{0.0f}; // rotation angles for each axis in radians

        /**
         * @brief resulting transformation matrix corrsponds to Translate * Rz * Ry * Rx * Scale. 
         * simple euler angles rotation for now. rotations correspond to tait-bryan angles of Z(1), Y(2), X(3).
         * https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
         * @return glm::mat4 translation * rotation.y * rotation.x * rotation.z * scale
         */
        glm::mat4 Matrix();

        /**
         * @brief compute the normal matrix for the object. this is used for lighting
         * when the object is transformed/scaled.
         * @return glm::mat4 
         */
        glm::mat4 NormalMatrix();
    }; // struct TransformObject
} // namespace DORY

#endif // DORY_TRANSFORM_INCL