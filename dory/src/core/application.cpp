#include "core/application.h"
#include "core/logger.h"
#include "core/timer.h"
#include "renderer/camera.h"
#include "renderer/camera_controller.h"
#include "systems/renderer_system.h"

#include <stdexcept>
#include <array>

namespace DORY
{
    Application* Application::s_instance = nullptr;

    Application::Application()
    {   
        // assign the application instance to the static instance variable
        s_instance = this;
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
        Camera camera{};
        camera.SetViewTarget(glm::vec3(-1.0f, -2.0f, -2.0f), glm::vec3(0.0f, 0.0f, 2.5f));

        auto viewer = Object::CreateObject();
        CameraController camera_controller{};

        Timer timer{};

        // run the application
        while (!m_window.ShouldClose())
        {
            glfwPollEvents();

            float frame_time = timer.GetElapsedTime();
            camera_controller.Move(m_window.GetWindow(), frame_time, viewer);
            camera.SetViewZYX(viewer.transform.translation, viewer.transform.rotation);

            float aspect = m_renderer.GetSwapChainAspectRatio();

            // update the camera in case window was resized
            camera.SetPerspectiveProjection(glm::radians(45.0f), aspect, 0.1f, 100.0f);
            
            // BeginFrame() returns nullptr if the swap chain is not ready (i.e. the window is being resized, etc.)
            if (auto command_buffer = m_renderer.BeginFrame())
            {
                m_renderer.BeginSwapChainRenderPass(command_buffer);
                renderer_system.RenderObjects(command_buffer, m_objects, camera);
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
        cube.transform.translation = glm::vec3{0.0f, 0.0f, 2.5f};
        cube.transform.scale = glm::vec3{0.5f, 0.5f, 0.5f};
        m_objects.push_back(std::move(cube));
    }
} // namespace DORY