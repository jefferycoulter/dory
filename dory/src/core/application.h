#ifndef DORY_APPLICATION_INCL
#define DORY_APPLICATION_INCL

#include "core/core.h"
#include "math/push.h"
#include "platform/window.h"
#include "renderer/device.h"
#include "renderer/object.h"
#include "renderer/renderer.h"
#include "renderer/swapchain.h"
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

        private: // methods
            /**
             * @brief load vertex data from a file and create a vertex buffer object
             */
            void LoadObjects();
            
        private: // members
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