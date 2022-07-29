#include "renderer/data.h"
#include "systems/renderer_system.h"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>

namespace DORY
{
    RendererSystem::RendererSystem(Device& device, VkRenderPass render_pass)
        : m_device(device)
    {
        CreatePipelineLayout();
        CreatePipeline(render_pass);
    }
    
    RendererSystem::~RendererSystem()
    {
        vkDestroyPipelineLayout(m_device.GetDevice(), m_pipeline_layout, nullptr);
    }

    void RendererSystem::CreatePipelineLayout()
    {
        VkPushConstantRange push_constant_range{};
        push_constant_range.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        push_constant_range.offset = 0;
        push_constant_range.size = sizeof(PushConstantData3D);

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

    void RendererSystem::CreatePipeline(VkRenderPass render_pass)
    {
        DASSERT_MSG(m_pipeline_layout != nullptr, "Pipeline layout is null!");

        PipelineConfigInfo pipeline_config{};
        Pipeline::DefaultConfig(pipeline_config);
        pipeline_config._render_pass = render_pass;
        pipeline_config._pipeline_layout = m_pipeline_layout;
        m_pipeline = std::make_unique<Pipeline>(m_device, pipeline_config, "assets/shaders/shader.vert.spv", "assets/shaders/shader.frag.spv");
    }

    void RendererSystem::CreateDescriptorSetLayout()
    {

    }

    void RendererSystem::CreateDescriptorPool(const std::vector<VkDescriptorPoolSize> &pool_sizes)
    {
        m_global_pool = std::make_unique<DescriptorPool>(m_device, SwapChain::MAX_FRAMES_IN_FLIGHT, 0, pool_sizes);
    }

    void RendererSystem::RenderObjects(VkCommandBuffer command_buffer, std::vector<Object>& objects, const Camera& camera)
    {
        m_pipeline->Bind(command_buffer);

        auto projection_view_matrix = camera.GetProjection() * camera.GetView();

        for (auto& object : objects)
        {
            PushConstantData3D push{};
            auto model_matrix = object.transform.Matrix();
            push.transform = projection_view_matrix * model_matrix;
            push.normal_matrix = object.transform.NormalMatrix();

            vkCmdPushConstants(command_buffer, m_pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushConstantData3D), &push);
            object.m_model->Bind(command_buffer);
            object.m_model->Draw(command_buffer);
        }
    }
} // namespace DORY