#include "systems/point_light_system.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>

namespace DORY
{
    PointLightSystem::PointLightSystem(Device& device, VkRenderPass render_pass, VkDescriptorSetLayout descriptor_set_layout)
        : m_device(device)
    {
        CreatePipelineLayout(descriptor_set_layout);
        CreatePipeline(render_pass);
    }
    
    PointLightSystem::~PointLightSystem()
    {
        vkDestroyPipelineLayout(m_device.GetDevice(), m_pipeline_layout, nullptr);
    }

    void PointLightSystem::CreatePipelineLayout(VkDescriptorSetLayout descriptor_set_layout)
    {
        std::vector<VkDescriptorSetLayout> layouts{descriptor_set_layout};

        VkPipelineLayoutCreateInfo pipeline_layout_info{};
        pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipeline_layout_info.setLayoutCount = static_cast<uint32_t>(layouts.size());
        pipeline_layout_info.pSetLayouts = layouts.data();
        pipeline_layout_info.pushConstantRangeCount = 0;
        pipeline_layout_info.pPushConstantRanges = nullptr;

        if (vkCreatePipelineLayout(m_device.GetDevice(), &pipeline_layout_info, nullptr, &m_pipeline_layout) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create pipeline layout!");
        }
    }

    void PointLightSystem::CreatePipeline(VkRenderPass render_pass)
    {
        DASSERT_MSG(m_pipeline_layout != nullptr, "Pipeline layout is null!");

        PipelineConfigInfo pipeline_config{};
        Pipeline::DefaultConfig(pipeline_config);
        pipeline_config.binding_decriptions.clear();
        pipeline_config.attribute_decriptions.clear();
        pipeline_config._render_pass = render_pass;
        pipeline_config._pipeline_layout = m_pipeline_layout;
        m_pipeline = std::make_unique<Pipeline>(m_device, pipeline_config, "assets/shaders/point_light.vert.spv", "assets/shaders/point_light.frag.spv");
    }

    void PointLightSystem::Render(FrameInfo frame_info)
    {
        m_pipeline->Bind(frame_info.command_buffer);
        vkCmdBindDescriptorSets(frame_info.command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline_layout, 0, 1, &frame_info.descriptor_set, 0, nullptr);
        vkCmdDraw(frame_info.command_buffer, 6, 1, 0, 0);
    }
} // namespace DORY