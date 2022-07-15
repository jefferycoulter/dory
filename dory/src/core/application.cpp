#include "application.h"
#include "systems/renderer_system.h"

#include <stdexcept>
#include <array>

namespace DORY
{
    Application::Application()
    {
        LoadObjects();
    }
    
    Application::~Application()
    {
    }
    
    void Application::Run()
    {
        RendererSystem renderer_system{m_device, m_renderer.GetSwapChainRenderPass()};
        // run the application
        while (!m_window.ShouldClose())
        {
            glfwPollEvents();
            
            // BeginFrame() returns nullptr if the swap chain is not ready (i.e. the window is being resized, etc.)
            if (auto command_buffer = m_renderer.BeginFrame())
            {
                m_renderer.BeginSwapChainRenderPass(command_buffer);
                renderer_system.RenderObjects(command_buffer, m_objects);
                m_renderer.EndSwapChainRenderPass(command_buffer);
                m_renderer.EndFrame();
            }
        }

        vkDeviceWaitIdle(m_device.GetDevice());
    }

    void Application::LoadObjects()
    {
        std::vector<Model::Vertex> vertices
        {
            {{0.0f, -0.5f}},
            {{0.5f, 0.5f}},
            {{-0.5f, 0.5f}},
        };

        auto model = std::make_shared<Model>(m_device, vertices); // shared pointer so that multiple objects can share the same model
        auto triangle = Object::CreateObject();
        triangle.m_model = model;
        triangle.m_color = {0.1f, 0.8f, 0.1f};
        triangle.transform_2d.translation.x = 0.2f;
        triangle.transform_2d.scale.x = 0.5f;
        triangle.transform_2d.scale.y = 2.0f;
        triangle.transform_2d.rotation = 0.25f * glm::two_pi<float>();
        m_objects.push_back(std::move(triangle));
    }

} // namespace DORY