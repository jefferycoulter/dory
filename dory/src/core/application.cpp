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

        auto viewer = Object::CreateObject(); // this holds the camera
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
        DTRACE("%s \n", event.ToString().c_str());
    }

    void Application::LoadObjects()
    {   
        std::shared_ptr<Model> model = Model::LoadModelFromFile(m_device, "assets/models/stanford_bunny.obj");
        auto object = Object::CreateObject();
        object.m_model = model;
        object.transform.translation = glm::vec3{0.0f, 0.0f, 2.5f};
        object.transform.scale = glm::vec3{0.5f, 0.5f, 0.5f};
        m_objects.push_back(std::move(object));
    }
} // namespace DORY