#include "application.h"

namespace DORY
{
    Application::Application()
    {
    }
    
    Application::~Application()
    {
    }
    
    void Application::Run()
    {
        // run the application
        while (!m_window.ShouldClose())
        {
            glfwPollEvents();
        }
    }
} // namespace DORY