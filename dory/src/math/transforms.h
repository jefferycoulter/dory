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
         * @brief resulting matrix corrsponds to Translate * Rz * Ry * Rx * Scale. 
         * simple euler angles rotation for now. rotations correspond to tait-bryan angles of Z(1), Y(2), X(3).
         * https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
         * @return glm::mat4 translation * rotation.y * rotation.x * rotation.z * scale
         */
        glm::mat4 mat4()
        {
            const float c3 = glm::cos(rotation.x);
            const float s3 = glm::sin(rotation.x);
            const float c2 = glm::cos(rotation.y);
            const float s2 = glm::sin(rotation.y);
            const float c1 = glm::cos(rotation.z);
            const float s1 = glm::sin(rotation.z);
            return glm::mat4
            {
                { // first column
                    scale.x * (c1 * c2),
                    scale.x * (c2 * s1),
                    scale.x * (-s2),
                    0.0f,
                },
                { // second column
                    scale.y * (c1 * s2 * s3 - c3 * s1),
                    scale.y * (c1 * c3 + s1 * s2 * s3),
                    scale.y * (c2 * s3),
                    0.0f,
                },
                { // third column
                    scale.z * (s1 * s3 + c1 * c3 * s2),
                    scale.z * (c3 * s1 * s2 - c1 * s3),
                    scale.z * (c2 * c3),
                    0.0f,
                },
                { // fourth column
                    translation.x, 
                    translation.y, 
                    translation.z, 
                    1.0f
                }
            };
        }
    }; // struct TransformObject

} // namespace DORY

#endif // DORY_TRANSFORM_INCL