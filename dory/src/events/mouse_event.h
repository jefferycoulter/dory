#ifndef DORY_MOUSE_EVENT_INCL
#define DORY_MOUSE_EVENT_INCL

#include "event.h"
#include <sstream>

namespace DORY
{
    class MouseMoveEvent : public Event
	{
        public:
            MouseMoveEvent(float x, float y)
                : m_X(x), m_Y(y) {}

            inline float getX() const { return m_X; }
            inline float getY() const { return m_Y; }

            std::string toString() const override
            {
                std::stringstream ss;
                ss << "MouseMoveEvent: " << m_X << ", " << m_Y;
                return ss.str();
            }

            EVENT_TYPE(MouseMoved)

        private:
            float m_X, m_Y;

	}; // class MouseMoveEvent

	class MouseScrolledEvent : public Event
	{
        public:
            MouseScrolledEvent(const float xOffset, const float yOffset)
                : m_XOffset(xOffset), m_YOffset(yOffset) {}

            float GetXOffset() const { return m_XOffset; }
            float GetYOffset() const { return m_YOffset; }

            std::string toString() const override
            {
                std::stringstream ss;
                ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
                return ss.str();
            }

            EVENT_TYPE(MouseScrolled);

        private:
            float m_XOffset, m_YOffset;
            
	}; // class MouseScrolledEvent

	class MouseEvent : public Event
	{
        public:
            inline int GetMouseButton() const { return m_Button; }

        protected:
            MouseEvent(int button)
                : m_Button(button) {}

            int m_Button;

	}; // class MouseEvent

	class MousePressedEvent : public MouseEvent
	{
        public:
            MousePressedEvent(int button)
                : MouseEvent(button) {}

            std::string toString() const override
            {
                std::stringstream ss;
                ss << "MouseButtonPressedEvent: " << m_Button;
                return ss.str();
            }

            EVENT_TYPE(MousePressed)

	}; // class MousePressedEvent

	class MouseReleasedEvent : public MouseEvent
	{
        public:
            MouseReleasedEvent(int button)
                : MouseEvent(button) {}

            std::string toString() const override
            {
                std::stringstream ss;
                ss << "MouseReleasedEvent: " << m_Button;
                return ss.str();
            }

            EVENT_TYPE(MouseReleased)

	}; // class MouseReleasedEvent

} // namespace DORY

#endif // DORY_MOUSE_EVENT_INCL