#ifndef DORY_APPLICATION_INCL
#define DORY_APPLICATION_INCL

#include "core/core.h"
#include "events/event.h"
#include "events/window_event.h"
#include "platform/window.h"
#include "renderer/device.h"
#include "renderer/object.h"
#include "renderer/renderer.h"
#include "utils/nocopy.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace DORY
{
    /**
     * @brief core class representing basic properties of an application that will be created
     * by a user
     */
    class Application : public NoCopy
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

            /**
             * @brief tell the application to do something in response to a given event
             * @param event the event to handle
             */
            void OnEvent(Event& event);

            /**
             * @brief static function used to get a reference to the application
             * @return Application& 
             */
            static Application& Get() { return *s_instance; }

            /**
             * @brief return the application's window
             * @return GLFWwindow* 
             */
            GLFWwindow* GetApplicationWindow() const { return m_window.GetWindow(); }

        private: // methods
            /**
             * @brief load vertex data from a file and create a vertex buffer object
             */
            void LoadObjects();
            
        private: // members
            static Application* s_instance; // static instance of application
            Window m_window{WIDTH, HEIGHT, "Dory"}; // window object containing the application 
            Device m_device{m_window}; // device running the application
            Renderer m_renderer{m_window, m_device}; // renderer for the application
            std::vector<Object> m_objects; // the application's objects
    }; // class Application

    /**
     * @brief initialize an application.  this is defined externally by the user and is called
     * by the main function in entry.h
     * @return DORY::Application*
     */
    Application* Init(); 
} // namespace DORY

#endif // DORY_APPLICATION_INCL