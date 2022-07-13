#include "application.h"

#include <stdexcept>
#include <array>

namespace DORY
{
    Application::Application()
    {
        LoadModels();
        CreatePipelineLayout();
        CreatePipeline();
    }
    
    Application::~Application()
    {
        vkDestroyPipelineLayout(m_device.GetDevice(), m_pipeline_layout, nullptr);
    }
    
    void Application::Run()
    {
        // run the application
        while (!m_window.ShouldClose())
        {
            glfwPollEvents();
            
            // BeginFrame() returns nullptr if the swap chain is not ready (i.e. the window is being resized, etc.)
            if (auto command_buffer = m_renderer.BeginFrame())
            {
                m_renderer.BeginSwapChainRenderPass(command_buffer);
                LoadModels();
                m_renderer.EndSwapChainRenderPass(command_buffer);
                m_renderer.EndFrame();
            }
        }

        vkDeviceWaitIdle(m_device.GetDevice());
    }

    void Application::CreatePipelineLayout()
    {
        VkPushConstantRange push_constant_range{};
        push_constant_range.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        push_constant_range.offset = 0;
        push_constant_range.size = sizeof(PushConstantData);

        VkPipelineLayoutCreateInfo pipeline_layout_info{};
        pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipeline_layout_info.setLayoutCount = 0;
        pipeline_layout_info.pSetLayouts = nullptr;
        pipeline_layout_info.pushConstantRangeCount = 1;
        pipeline_layout_info.pPushConstantRanges = &push_constant_range;

        if (vkCreatePipelineLayout(m_device.GetDevice(), &pipeline_layout_info, nullptr, &m_pipeline_layout) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create pipeline layout!");
        }
    }

    void Application::CreatePipeline()
    {
        DASSERT_MSG(m_pipeline_layout != nullptr, "Pipeline layout is null!");

        PipelineConfigInfo pipeline_config{};
        Pipeline::DefaultConfig(pipeline_config);
        pipeline_config._render_pass = m_renderer.GetSwapChainRenderPass();
        pipeline_config._pipeline_layout = m_pipeline_layout;
        m_pipeline = std::make_unique<Pipeline>(m_device, pipeline_config, "assets/shaders/shader.vert.spv", "assets/shaders/shader.frag.spv");
    }

    void Application::LoadModels()
    {
        std::vector<Model::Vertex> vertices
        {
            {{0.0f, -0.5f}},
            {{0.5f, 0.5f}},
            {{-0.5f, 0.5f}},
        };

        m_model = std::make_unique<Model>(m_device, vertices);
    }

} // namespace DORY