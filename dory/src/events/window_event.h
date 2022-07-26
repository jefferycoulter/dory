#ifndef DORY_WINDOW_EVENT_INCL
#define DORY_WINDOW_EVENT_INCL

#include "event.h"
#include <sstream>

namespace DORY
{
    class WindowResizeEvent : public Event
	{
        public:
            WindowResizeEvent(unsigned int width, unsigned int height)
                : m_Width(width), m_Height(height) {}

            unsigned int GetWidth() const { return m_Width; }
            unsigned int GetHeight() const { return m_Height; }

            std::string ToString() const override
            {
                std::stringstream ss;
                //printf("WindowResizeEvent: %d, %d", m_Width, m_Height);
                ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
                return ss.str();
            }

            EVENT_TYPE(WindowResize)

        private:
            unsigned int m_Width, m_Height;
            
	}; // class WindowResizeEvent

	class WindowCloseEvent : public Event
	{
        public:
            WindowCloseEvent() = default;

            EVENT_TYPE(WindowClose)
	}; // class WindowCloseEvent

} // namespace DORY

#endif // DORY_WINDOW_EVENT_INCL