#include "core/logger.h"
#include "events/keyboard_event.h"
#include "events/mouse_event.h"
#include "events/window_event.h"
#include "platform/window.h"

#include <stdexcept>
#include <iostream>

namespace DORY
{
    Window::Window(const unsigned int& width, const unsigned int& height, const char* title)
        : m_width{width}, m_height{height}, m_title{title}
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
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // resizing will be handled differently with vulkan

        m_window = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);
        DINFO("Window created. Width: %d, Height: %d, Title: %s", m_width, m_height, m_title);
        glfwSetWindowUserPointer(m_window, this);

        SetGLFWCallbacks();
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

    void Window::SetGLFWCallbacks()
    {
        glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) 
			{
				// https://www.glfw.org/docs/3.3/group__window.html#gad91b8b047a0c4c6033c38853864c34f8
				auto _window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
				_window->m_framebuffer_resized = true;
                _window->m_width = width;
                _window->m_height = height;

				// declare an instance of window resize event
				WindowResizeEvent event(width, height);

				// call the callback function (which is set in application.cpp)
				_window->m_callback(event);
			});

            glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
			{
				auto _window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
				WindowCloseEvent event;
				_window->m_callback(event);
			});

            glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				auto _window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
				WindowCloseEvent event;

				switch (action)
				{
					case GLFW_PRESS:
					{
						KeyPressedEvent event(key, 0);
						_window->m_callback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleasedEvent event(key);
						_window->m_callback(event);
						break;
					}
					case GLFW_REPEAT:
					{
						KeyPressedEvent event(key, 1);
						_window->m_callback(event);
						break;
					}
				}
			});

            glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
			{
				auto _window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
				switch (action)
				{
					case GLFW_PRESS:
					{
						MousePressedEvent event(button);
						_window->m_callback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseReleasedEvent event(button);
						_window->m_callback(event);
						break;
					}
				}
			});

            glfwSetScrollCallback(m_window, [](GLFWwindow* window, double dx, double dy)
			{
				auto _window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
				MouseScrolledEvent event((float)dx, (float)dy);
				_window->m_callback(event);
			});

		    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
			{
				auto _window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
				MouseMoveEvent event((float)xPos, (float)yPos);
				_window->m_callback(event);
			});
    }
} // namespace DORY