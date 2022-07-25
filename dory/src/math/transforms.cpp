#include "math/transforms.h"

namespace DORY
{
    glm::mat4 TransformObject::Matrix()
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

        glm::mat4 TransformObject::NormalMatrix()
    {
        const float c3 = glm::cos(rotation.x);
        const float s3 = glm::sin(rotation.x);
        const float c2 = glm::cos(rotation.y);
        const float s2 = glm::sin(rotation.y);
        const float c1 = glm::cos(rotation.z);
        const float s1 = glm::sin(rotation.z);

        const glm::vec3 scale_inv = 1.0f / scale;

        return glm::mat3
        {
            { // first column
                scale_inv.x * (c1 * c2),
                scale_inv.x * (c2 * s1),
                scale_inv.x * (-s2),
            },
            { // second column
                scale_inv.y * (c1 * s2 * s3 - c3 * s1),
                scale_inv.y * (c1 * c3 + s1 * s2 * s3),
                scale_inv.y * (c2 * s3),
            },
            { // third column
                scale_inv.z * (s1 * s3 + c1 * c3 * s2),
                scale_inv.z * (c3 * s1 * s2 - c1 * s3),
                scale_inv.z * (c2 * c3),
            },
        };
    }
} // namespace DORY