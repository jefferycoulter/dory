#ifndef DORY_EVENT_INCL
#define DORY_EVENT_INCL

#include <string>
#include <functional>

namespace DORY
{

    enum class EventType
	{
		None = 0,
		KeyPressed,
		KeyReleased,
		KeyTyped,
		MousePressed,
		MouseReleased,
		MouseMoved,
		MouseScrolled,
		WindowClose,
		WindowResize,
		WindowMoved
	};

    // fill in these functions with this macro since they will be the same for each event implementation
    #define EVENT_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }


    class Event
    {
        public:
            virtual ~Event() = default;

            virtual EventType GetEventType() const = 0;
            virtual const char* GetName() const = 0;
            virtual std::string ToString() const { return GetName(); }

            bool m_handled = false; // determine whether event has been handled (should not propogate further)

    }; // class Event

} // namespace DORY

#endif // DORY_EVENT_INCL