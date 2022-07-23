#ifndef DORY_TIMER_INCL
#define DORY_TIMER_INCL

#include <chrono>

namespace DORY
{
    class Timer
    {
        public:
            Timer()
            {
                Reset();
            }

            void Reset()
            {
                m_start = std::chrono::high_resolution_clock::now();
            }

            float GetElapsedTime()
            {
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - m_start);
                m_start = end;
                return duration.count() / 1000000.0f;
            }

        private:
            std::chrono::time_point<std::chrono::high_resolution_clock> m_start;

    }; // class Timer

} // namespace DORY

#endif // DORY_TIMER_INCL