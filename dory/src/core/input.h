#ifndef DORY_INPUT_INCL
#define DORY_INPUT_INCL

#include "core/key_codes.h"
#include "core/mouse_codes.h"
#include "platform/window.h"

#include <glm/glm.hpp>

namespace DORY
{
    class Input
	{
		public:
			static bool IsKeyPressed(KeyCode key);

			static bool IsMouseButtonPressed(MouseCode button);
			static glm::vec2 GetMousePosition();
			static float GetMouseX();
			static float GetMouseY();
	}; // class Input
} // namespace DORY

#endif // DORY_INPUT_INCL