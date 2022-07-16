#ifndef DORY_EVENT_HANDLER_INCL
#define DORY_EVENT_HANDLER_INCL

#include "event.h"

#include <string>
#include <functional>

namespace DORY
{
    class EventHandler
	{
        public:
            EventHandler(Event& event)
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

#endif // DORY_EVENT_HANDLER_INCL