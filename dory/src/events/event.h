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
            virtual std::string toString() const { return GetName(); }

            bool m_handled = false; // determine whether event has been handled (should not propogate further)

    }; // class Event

    class EventShare
	{
        public:
            EventShare(Event& event)
                : m_event(event)
                {
                }

           /**
            * @brief template function is used to match a desired event (type) with 
            * an event passed to the dispatch function (m_Event). if they match
            * then the event is "handled"
            * @tparam type templated event type to match
            * @tparam func templated function (an event call back function)
            * @param f function to call if event type matches
            * @return true 
            * @return false 
            */
            template<typename type, typename func>
            bool Share(const func& f)
            {
                // check if event passed to function is same as template type.
                // see use in application.cpp (Application::OnEvent) for example
                if (m_event.GetEventType() == type::GetStaticType())
                {
                    m_event.m_handled = f(*(type*)&m_event);
                    return true;
                }
            }

        private:
            Event& m_event;

    }; // class EventShare

} // namespace DORY

#endif // DORY_EVENT_INCL