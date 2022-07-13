#ifndef DORY_WINDOW_INCL
#define DORY_WINDOW_INCL

#include "utils/nocopy.h"

#define GLFW_INCLUDE_VULKAN
#define VK_ENABLE_BETA_EXTENSIONS
#include <GLFW/glfw3.h>

namespace DORY
{
    class Window : NoCopy
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

            bool WindowResized() { return m_framebuffer_resized; }

            void ResetWindowResizedFlag() { m_framebuffer_resized = false; }

            /**
             * @brief determine whether the window should close or not
             * @return true
             * @return false
             */
            bool ShouldClose();

        private: // methods
            /**
             * @brief initialize the GLFW and create a window.  @see _window
             */
            void Init();

            static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);

        private: // members
            GLFWwindow *m_window; // pointer to the application's window
            unsigned int m_width; // window width
            unsigned int m_height; // window height
            const char* m_title; // window title
            bool m_framebuffer_resized = false; // flag to indicate whether the framebuffer has been resized

    }; // class Window

} // namespace DORY

#endif // DORY_WINDOW_INCL