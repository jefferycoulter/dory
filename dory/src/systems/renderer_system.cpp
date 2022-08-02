#include "renderer/data.h"
#include "systems/renderer_system.h"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>

namespace DORY
{
    RendererSystem::RendererSystem(Device& device, VkRenderPass render_pass, VkDescriptorSetLayout descriptor_set_layout)
        : m_device(device)
    {
        CreatePipelineLayout(descriptor_set_layout);
        CreatePipeline(render_pass);
    }
    
    RendererSystem::~RendererSystem()
    {
        vkDestroyPipelineLayout(m_device.GetDevice(), m_pipeline_layout, nullptr);
    }

    void RendererSystem::CreatePipelineLayout(VkDescriptorSetLayout descriptor_set_layout)
    {
        VkPushConstantRange push_constant_range{};
        push_constant_range.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        push_constant_range.offset = 0;
        push_constant_range.size = sizeof(PushConstantData3D);

        std::vector<VkDescriptorSetLayout> layouts{descriptor_set_layout};

        VkPipelineLayoutCreateInfo pipeline_layout_info{};
        pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipeline_layout_info.setLayoutCount = static_cast<uint32_t>(layouts.size());
        pipeline_layout_info.pSetLayouts = layouts.data();
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

    void RendererSystem::RenderObjects(FrameInfo frame_info, std::vector<Object>& objects)
    {
        m_pipeline->Bind(frame_info.command_buffer);
        vkCmdBindDescriptorSets(frame_info.command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline_layout, 0, 1, &frame_info.descriptor_set, 0, nullptr);

        for (auto& object : objects)
        {
            PushConstantData3D push{};
            push.model_matrix = object.transform.Matrix();
            push.normal_matrix = object.transform.NormalMatrix();

            vkCmdPushConstants(frame_info.command_buffer, m_pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushConstantData3D), &push);
            object.m_model->Bind(frame_info.command_buffer);
            object.m_model->Draw(frame_info.command_buffer);
        }
    }
} // namespace DORY