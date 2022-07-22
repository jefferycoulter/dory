#include "application.h"
#include "systems/renderer_system.h"

#include <stdexcept>
#include <array>

namespace DORY
{
    Application::Application()
    {
        // set the window event callback to be the application's OnEvent method
        m_window.SetEventCallback([this](Event& event) { this->OnEvent(event); });
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

    void Application::OnEvent(Event& event)
    {
        DTRACE("%s \n", event.toString().c_str());
    }

    // temporary helper function, creates a 1x1x1 cube centered at offset
    std::unique_ptr<Model> CreateCube(Device& device, glm::vec3 offset)
    {
        std::vector<Model::Vertex> vertices{

            // left face (white)
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

            // right face (yellow)
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},

            // top face (orange, remember y axis points down)
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

            // bottom face (red)
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},

            // nose face (blue)
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
        };

        for (auto& v : vertices) 
        {
            v.a_position += offset;
        }

        return std::make_unique<Model>(device, vertices);
    }

    void Application::LoadObjects()
    {
        std::shared_ptr<Model> cube_model = CreateCube(m_device, glm::vec3{0.0f, 0.0f, 0.0f});
        auto cube = Object::CreateObject();
        cube.m_model = cube_model;
        cube.transform.translation = glm::vec3{0.0f, 0.0f, 0.5f};
        cube.transform.scale = glm::vec3{0.5f, 0.5f, 0.5f};
        m_objects.push_back(std::move(cube));
    }

} // namespace DORY