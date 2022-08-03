#ifndef DORY_DATA_INCL
#define DORY_DATA_INCL

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace DORY
{
    struct PushConstantData2D
    {
        glm::mat2 transform{1.0f};
        glm::vec2 offset;
        alignas(16) glm::vec3 color;
    }; // struct PushConstantData2D

    struct PushConstantData3D
    {
        glm::mat4 model_matrix{1.0f};
        glm::mat4 normal_matrix{1.0f};
    };   // struct PushConstantData3D

    struct UniformBufferObject
    {
        glm::mat4 projection{1.0f};
        glm::mat4 view{1.0f};
        glm::vec4 ambient_color{1.0f, 1.0f, 1.0f, 0.1f};
        glm::vec4 light_position{1.0f, 1.0f, 1.0f, -1.0f};
        glm::vec4 light_color{1.0f};
    }; // struct UBO
} // namespace DORY

#endif // DORY_DATA_INCL