#ifndef DORY_PUSH_CONSTANTS_INCL
#define DORY_PUSH_CONSTANTS_INCL

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace DORY
{
    struct PushConstantData
    {
        glm::mat2 transform{1.0f};
        glm::vec2 offset;
        alignas(16) glm::vec3 color;
    };

} // namespace DORY

#endif // DORY_PUSH_CONSTANTS_INCL