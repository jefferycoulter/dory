#ifndef DORY_WINDOW_INCL
#define DORY_WINDOW_INCL

#include "events/event.h"
#include "utils/nocopy.h"

#define GLFW_INCLUDE_VULKAN
#ifdef __APPLE__
    #define VK_ENABLE_BETA_EXTENSIONS
#endif
#include <GLFW/glfw3.h>

#include <functional>

namespace DORY
{
    class Window : public NoCopy
    {
        public:
            /**
             * @brief construct a new Window object
             * @param width window width
             * @param height window height
             * @param title window title
             */
            Window(const unsigned int& width, const unsigned int& height, const char* title);

            /**
             * @brief destroy the Window object
             */
            ~Window();

            /**
             * @brief create a window surface for Vulkan rendering.
             * https://vulkan-tutorial.com/Drawing_a_triangle/Presentation/Window_surface
             * @param instance 
             * @param surface 
             */
            void CreateWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

            /**
             * @brief return the Vulkan Extent (width and height) of the window.
             * @return VkExtent2D 
             */
            VkExtent2D GetExtent() const { return {static_cast<uint32_t>(m_width), static_cast<uint32_t>(m_height)}; }

            /**
             * @brief check if the window has been resized
             * @return true 
             * @return false 
             */
            bool WindowResized() { return m_framebuffer_resized; }

            /**
             * @brief once the window resize event is handled, set the flag to false.
             */
            void ResetWindowResizedFlag() { m_framebuffer_resized = false; }

            /**
             * @brief return the GLFW window handle
             * @return GLFWwindow* 
             */
            GLFWwindow* GetWindow() const { return m_window; }

            /**
             * @brief determine whether the window should close or not
             * @return true
             * @return false
             */
            bool ShouldClose();

            /**
             * @brief set the event callback function for the window. 
             * @see application.cpp
             */
            void SetEventCallback(const std::function<void(Event&)>& callback)
            {
                m_callback = callback;
            }

        private: // methods
            /**
             * @brief initialize the GLFW and create a window.
             */
            void Init();

            /**
             * @brief set the GLFW event callbacks for the window.
             */
            void SetGLFWCallbacks();

        private: // members
            GLFWwindow *m_window; // pointer to the application's window
            unsigned int m_width; // window width
            unsigned int m_height; // window height
            const char* m_title; // window title
            std::function<void(Event&)> m_callback; // callback function for events
            bool m_framebuffer_resized = false; // flag to indicate whether the framebuffer has been resized
    }; // class Window
} // namespace DORY

#endif // DORY_WINDOW_INCL