#ifndef DORY_APPLICATION_INCL
#define DORY_APPLICATION_INCL

#include "nocopy.h"
#include "platform/window.h"

namespace DORY
{
    /**
     * @brief core class representing basic properties of an application that will be created
     * by a user
     */
    class Application : NoCopy
    {
        public:

            /**
             * @brief default width of application window
             */
            const unsigned int WIDTH = 800;

            /**
             * @brief default height of application window
             */
            const unsigned int HEIGHT = 600;

            /**
             * @brief construct a new Application object
             */
            Application();

            /**
             * @brief destroy the Application object
             */
            ~Application();

            /**
             * @brief run the application
             */
            void Run();

        private:
            /**
             * @brief window object containing the application 
             */
            Window m_window{WIDTH, HEIGHT, "Dory"};

    }; // class Application

    /**
     * @brief initialize an application.  this is defined externally by the user and is called
     * by the main function in entry.h
     * @return DORY::Application*
     */
    Application* Init();

} // namespace DORY

#endif // DORY_APPLICATION_INCL