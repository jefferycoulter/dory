#ifndef DORY_KEYBOARD_EVENT_INCL
#define DORY_KEYBOARD_EVENT_INCL

#include "event.h"
#include <sstream>

namespace DORY
{
    class KeyEvent : public Event
        {
            public:
                inline int GetKeyCode() const { return m_keycode; }

            protected:
                KeyEvent(int keycode)
                    : m_keycode(keycode) {}

                int m_keycode;
        }; // class KeyEvent

        class KeyPressedEvent : public KeyEvent
        {
            public:
                KeyPressedEvent(int keycode, int rep_count)
                    : KeyEvent(keycode), m_rep_count(rep_count) {}

                inline int GetRepeatCount() const { return m_rep_count; }

                std::string toString()
                {
                    std::stringstream ss;
                    ss << "KeyPressed: " << m_keycode << " (" << m_rep_count << "repeats)";
                    return ss.str();
                }

                EVENT_TYPE(KeyPressed)
            private:
                int m_rep_count;
        }; // class KeyPressedEvent

        class KeyReleasedEvent : public KeyEvent
        {
            public:
                KeyReleasedEvent(int keycode)
                    : KeyEvent(keycode) {}

                std::string toString() const override
                {
                    std::stringstream ss;
                    ss << "KeyRelease: " << m_keycode;
                    return ss.str();
                }

                EVENT_TYPE(KeyReleased)

        }; // class KeyReleasedEvent

} // namespace DORY

#endif // DORY_KEYBOARD_EVENT_INCL