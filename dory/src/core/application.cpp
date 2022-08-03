#include "core/application.h"
#include "core/logger.h"
#include "core/timer.h"
#include "renderer/buffer.h"
#include "renderer/camera.h"
#include "renderer/data.h"
#include "renderer/camera_controller.h"
#include "systems/point_light_system.h"
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

        m_descriptor_pool = DescriptorPool::Builder(m_device)
                            .SetMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT)
                            .AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT)
                            .Build();
        LoadObjects();
    }
    
    Application::~Application()
    {
    }
    
    void Application::Run()
    {
        std::vector<std::unique_ptr<Buffer>> ubo_buffers(SwapChain::MAX_FRAMES_IN_FLIGHT);
        for (size_t i = 0; i < ubo_buffers.size(); i++)
        {
            ubo_buffers[i] = std::make_unique<Buffer>(  m_device, 
                                                        sizeof(UniformBufferObject), 
                                                        1,
                                                        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                                        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            ubo_buffers[i]->Map();
        }

        auto descriptor_set_layout = DescriptorSetLayout::Builder(m_device)
                                        .AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT)
                                        .Build();
        std::vector<VkDescriptorSet> descriptor_sets(SwapChain::MAX_FRAMES_IN_FLIGHT);
        for (size_t i = 0; i < descriptor_sets.size(); i++)
        {
            auto buffer_info = ubo_buffers[i]->DescriptorInfo();
            DescriptorWriter(*descriptor_set_layout, *m_descriptor_pool)
                            .WriteBuffer(0, &buffer_info)
                            .Build(descriptor_sets[i]);
        }

        RendererSystem renderer_system{m_device, m_renderer.GetSwapChainRenderPass(), descriptor_set_layout->GetDescriptorSetLayout()};
        PointLightSystem point_light_system{m_device, m_renderer.GetSwapChainRenderPass(), descriptor_set_layout->GetDescriptorSetLayout()};
        Camera camera{};
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
                int frame_index = m_renderer.GetCurrentFrameIndex();
                FrameInfo frame_info{frame_index, frame_time, command_buffer, camera, descriptor_sets[frame_index], m_objects};
                // update the uniform buffer object
                UniformBufferObject ubo{};
                ubo.projection = camera.GetProjection();
                ubo.view = camera.GetView();
                ubo_buffers[frame_index]->WriteToBuffer(&ubo);
                ubo_buffers[frame_index]->Flush();

                // render the objects
                m_renderer.BeginSwapChainRenderPass(command_buffer);
                renderer_system.RenderObjects(frame_info);
                point_light_system.Render(frame_info);
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
        auto bunny_object_1 = Object::CreateObject();
        bunny_object_1.m_model = model;
        bunny_object_1.transform.translation = glm::vec3{-0.5f, 0.5f, 2.5f};
        bunny_object_1.transform.scale = glm::vec3{0.5f, 0.5f, 0.5f};
        bunny_object_1.transform.rotation = glm::vec3{0.0f, glm::pi<float>(), 0.0f};
        m_objects.emplace(bunny_object_1.GetObjectId(), std::move(bunny_object_1));

        model = Model::LoadModelFromFile(m_device, "assets/models/stanford_bunny.obj");
        auto bunny_object_2 = Object::CreateObject();
        bunny_object_2.m_model = model;
        bunny_object_2.transform.translation = glm::vec3{0.5f, 0.5f, 2.5f};
        bunny_object_2.transform.scale = glm::vec3{0.5f, 0.5f, 0.5f};
        bunny_object_2.transform.rotation = glm::vec3{0.0f, glm::pi<float>(), 0.0f};
        m_objects.emplace(bunny_object_2.GetObjectId(), std::move(bunny_object_2));

        model = Model::LoadModelFromFile(m_device, "assets/models/floor.obj");
        auto floor_object = Object::CreateObject();
        floor_object.m_model = model;
        floor_object.transform.translation = glm::vec3{0.0f, 0.5f, 2.5f};
        floor_object.transform.scale = glm::vec3{1.0f, 1.0f, 1.0f};
        m_objects.emplace(floor_object.GetObjectId(), std::move(floor_object));
    }
} // namespace DORY