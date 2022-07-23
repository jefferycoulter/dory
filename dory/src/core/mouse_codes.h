#ifndef DORY_MOUSE_CODES_INCL
#define DORY_MOUSE_CODES_INCL

#include <cstdint>

namespace DORY
{
    using MouseCode = uint8_t;

	namespace Mouse
	{
		enum : MouseCode
		{
			// these are glfw key codes for now. taken from glfw3.h
			Button0                = 0,
			Button1                = 1,
			Button2                = 2,
			Button3                = 3,
			Button4                = 4,
			Button5                = 5,
			Button6                = 6,
			Button7                = 7,

			ButtonLast             = Button7,
			ButtonLeft             = Button0,
			ButtonRight            = Button1,
			ButtonMiddle           = Button2
		}; // enum
	} // namespace Mouse
} // namespace DORY

#endif // DORY_MOUSE_CODES_INCL