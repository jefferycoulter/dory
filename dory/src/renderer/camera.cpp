#include "camera.h"
#include "core/core.h"

#include <limits>   

namespace DORY
{
    void Camera::SetOrthographicProjection(float left, float right, float bottom, float top, float near, float far) 
    {
        m_projection_matrix = glm::mat4{1.0f};
        m_projection_matrix[0][0] = 2.f / (right - left);
        m_projection_matrix[1][1] = 2.f / (bottom - top);
        m_projection_matrix[2][2] = 1.f / (far - near);
        m_projection_matrix[3][0] = -(right + left) / (right - left);
        m_projection_matrix[3][1] = -(bottom + top) / (bottom - top);
        m_projection_matrix[3][2] = -near / (far - near);
    }

    void Camera::SetPerspectiveProjection(float fov, float aspect, float near, float far)
    {
        // check that the aspect ratio is larger than the machine epsilon
        DASSERT(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);

        const float tan_half_fov = tan(fov / 2.f);
        m_projection_matrix = glm::mat4{0.0f};
        m_projection_matrix[0][0] = 1.f / (aspect * tan_half_fov);
        m_projection_matrix[1][1] = 1.f / (tan_half_fov);
        m_projection_matrix[2][2] = far / (far - near);
        m_projection_matrix[2][3] = 1.f;
        m_projection_matrix[3][2] = -(far * near) / (far - near);
    }

    void Camera::SetViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up)
    {
        // orthonormal basis vectors for the camera view based on the direction
        const glm::vec3 w{glm::normalize(direction)};
        const glm::vec3 u{glm::normalize(glm::cross(w, up))};
        const glm::vec3 v{glm::cross(w, u)};

        m_view_matrix = glm::mat4{1.0f};
        // rotation components
        m_view_matrix[0][0] = u.x;
        m_view_matrix[1][0] = u.y;
        m_view_matrix[2][0] = u.z;
        m_view_matrix[0][1] = v.x;
        m_view_matrix[1][1] = v.y;
        m_view_matrix[2][1] = v.z;
        m_view_matrix[0][2] = w.x;
        m_view_matrix[1][2] = w.y;
        m_view_matrix[2][2] = w.z;
        // translation components
        m_view_matrix[3][0] = -glm::dot(u, position);
        m_view_matrix[3][1] = -glm::dot(v, position);
        m_view_matrix[3][2] = -glm::dot(w, position);
    }

    void Camera::SetViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up)
    {
        // check taht direction is not zero
        DASSERT(glm::length(position - target) > 0.0f);
        // the direction of the camera is the vector from the camera position to the target
        SetViewDirection(position, target - position, up);
    }

    void Camera::SetViewZYX(glm::vec3 position, glm::vec3 rotation)
    {
        const float c3 = glm::cos(rotation.x);
        const float s3 = glm::sin(rotation.x);
        const float c2 = glm::cos(rotation.y);
        const float s2 = glm::sin(rotation.y);
        const float c1 = glm::cos(rotation.z);
        const float s1 = glm::sin(rotation.z);
        const glm::vec3 u{(c1 * c2), (c2 * s1), (-s2)};
        const glm::vec3 v{(c1 * s2 * s3 - c3 * s1), (c1 * c3 + s1 * s2 * s3), (c2 * s3)};
        const glm::vec3 w{(s1 * s3 + c1 * c3 * s2), (c3 * s1 * s2 - c1 * s3), (c2 * c3)};
        m_view_matrix = glm::mat4{1.f};
        // rotation components
        m_view_matrix[0][0] = u.x;
        m_view_matrix[1][0] = u.y;
        m_view_matrix[2][0] = u.z;
        m_view_matrix[0][1] = v.x;
        m_view_matrix[1][1] = v.y;
        m_view_matrix[2][1] = v.z;
        m_view_matrix[0][2] = w.x;
        m_view_matrix[1][2] = w.y;
        m_view_matrix[2][2] = w.z;
        // translation components
        m_view_matrix[3][0] = -glm::dot(u, position);
        m_view_matrix[3][1] = -glm::dot(v, position);
        m_view_matrix[3][2] = -glm::dot(w, position);
    }

} // namespace DORY