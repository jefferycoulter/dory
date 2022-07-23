#include "core/input.h"
#include "renderer/camera_controller.h"

#include <limits>

namespace DORY
{
    void CameraController::Move(GLFWwindow* window, float dt, Object& object)
    {
        // get rotation input
        glm::vec3 rotate{0.0f};
        if (Input::IsKeyPressed(key_map.look_up)) { rotate.x += 1.0f; }
        if (Input::IsKeyPressed(key_map.look_down)) { rotate.x -= 1.0f; }
        if (Input::IsKeyPressed(key_map.look_right)) { rotate.y += 1.0f; }
        if (Input::IsKeyPressed(key_map.look_left)) { rotate.y -= 1.0f; }

        if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
        {
            object.transform.rotation += rotate_speed * dt * glm::normalize(rotate);
        }

        // prevent object from going upside down
        object.transform.rotation.x = glm::clamp(object.transform.rotation.x, -1.5f, 1.5f);
        // prevent repeated spinning of object
        object.transform.rotation.y = glm::mod(object.transform.rotation.y, glm::pi<float>() * 2.0f);

        // get the orthonormal basis of the object to perform movement in
        float yaw = object.transform.rotation.y; // rotation around y axis between previous forward direction and new forward direction
        const glm::vec3 forward_dir{glm::sin(yaw), 0.0f, glm::cos(yaw)};
        const glm::vec3 right_dir{forward_dir.z, 0.0f, -forward_dir.x};
        const glm::vec3 up_dir{0.0f, -1.0f, 0.0f};

        // get movement input
        glm::vec3 move{0.0f};
        if (Input::IsKeyPressed(key_map.move_up)) { move += up_dir; }
        if (Input::IsKeyPressed(key_map.move_down)) { move -= up_dir; }
        if (Input::IsKeyPressed(key_map.move_left)) { move -= right_dir; }
        if (Input::IsKeyPressed(key_map.move_right)) { move += right_dir; }
        if (Input::IsKeyPressed(key_map.move_backward)) { move -= forward_dir; }
        if (Input::IsKeyPressed(key_map.move_forward)) { move += forward_dir; }
        
        if (glm::dot(move, move) > std::numeric_limits<float>::epsilon())
        {
            object.transform.translation += move_speed * dt * glm::normalize(move);
        }
        
    }
} // namespace DORY