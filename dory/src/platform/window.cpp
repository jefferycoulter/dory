#include "window.h"

#include <stdexcept>
#include <iostream>

namespace DORY
{
    Window::Window(const unsigned int& width, const unsigned int& height, const char* title)
        :m_width{width}, m_height{height}, m_title{title}
    {
        Init();
    }

    Window::~Window()
    {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

     void Window::Init()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // resizing will be handled differently with vulkan

        m_window = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);
    }

    bool Window::ShouldClose()
    {
        return glfwWindowShouldClose(m_window);
    }

    void Window::CreateWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
    {
        if (glfwCreateWindowSurface(instance, m_window, NULL, surface) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create window surface!");
        }
    }
} // namespace DORY