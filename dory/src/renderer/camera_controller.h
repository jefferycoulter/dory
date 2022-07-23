#ifndef DORY_CAMERA_CONTROLLER_INCL
#define DORY_CAMERA_CONTROLLER_INCL

#include "core/key_codes.h"
#include "core/mouse_codes.h"
#include "renderer/object.h"
#include "platform/window.h"

namespace DORY
{
    class CameraController
    {
        public:
            struct KeyMap
            {
                KeyCode move_up = Key::Up;
                KeyCode move_down = Key::Down;
                KeyCode move_left = Key::Left;
                KeyCode move_right = Key::Right;
                KeyCode move_backward = Key::Space;
                KeyCode move_forward = Key::RightShift;
                KeyCode look_up = Key::W;
                KeyCode look_down = Key::S;
                KeyCode look_left = Key::A;
                KeyCode look_right = Key::D;
            }; // struct KeyMapping

            void Move(GLFWwindow* window, float dt, Object& object);

            float move_speed = 3.0f;
            float rotate_speed = 1.5f;
        
        private:
            KeyMap key_map{};
    }; // class CameraController
} // namespace DORY

#endif // DORY_CAMERA_CONTROLLER_INCL