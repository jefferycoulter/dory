#ifndef DORY_TRANSFORM_INCL
#define DORY_TRANSFORM_INCL

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace DORY
{
    /**
     * @brief struct containing 2D object transformation data
     */
    struct TransformObject2D
    {
        glm::vec2 translation{};
        glm::vec2 scale{1.0f, 1.0f};
        float rotation{0.0f};
        glm::mat2 mat2D()
        {
            const float cos_rotation = glm::cos(rotation);
            const float sin_rotation = glm::sin(rotation);
            glm::mat2 rotation_matrix{cos_rotation, -sin_rotation, sin_rotation, cos_rotation};
            glm::mat2 scale_matrix{{scale.x, 0.0f}, {0.0f, scale.y}};

            return rotation_matrix * scale_matrix;
        }
    };
} // namespace DORY

#endif // DORY_TRANSFORM_INCL