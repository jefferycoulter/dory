#include "renderer/pipeline.h"

namespace DORY
{
    Pipeline::Pipeline(Device& device, const PipelineConfigInfo& info, const std::string& vertex_path, const std::string& fragment_path)
        : m_device{device}
    {
        CreatePipeline(info, vertex_path, fragment_path);
    }

    Pipeline::~Pipeline()
    {
        vkDestroyShaderModule(m_device.GetDevice(), m_vert_shader_module, nullptr);
        vkDestroyShaderModule(m_device.GetDevice(), m_frag_shader_module, nullptr);
        vkDestroyPipeline(m_device.GetDevice(), m_graphics_pipeline, nullptr);
    }

    void Pipeline::Bind(VkCommandBuffer command_buffer)
    {
        vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphics_pipeline);
    }

    void Pipeline::CreatePipeline(const PipelineConfigInfo& info, const std::string& vertex_path, const std::string& fragment_path)
    {
        assert(
            info._pipeline_layout != VK_NULL_HANDLE && 
            "Cannot create graphics pipeline with a null layout in PipelineConfigInfo");
        assert(
            info._render_pass != VK_NULL_HANDLE && 
            "Cannot create graphics pipeline with a null render pass in PipelineConfigInfo");
        
        auto vertex_code = Utils::ReadFile(vertex_path);
        auto fragment_code = Utils::ReadFile(fragment_path);

        CreateShaderModule(vertex_code, &m_vert_shader_module);
        CreateShaderModule(fragment_code, &m_frag_shader_module);

        VkPipelineShaderStageCreateInfo shader_stages[2];
        // vertex shader stage
        shader_stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        shader_stages[0].module = m_vert_shader_module;
        shader_stages[0].pName = "main";
        shader_stages[0].flags = 0;
        shader_stages[0].pNext = nullptr;
        shader_stages[0].pSpecializationInfo = nullptr;
        // fragment shader stage
        shader_stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        shader_stages[1].module = m_frag_shader_module;
        shader_stages[1].pName = "main";
        shader_stages[1].flags = 0;
        shader_stages[1].pNext = nullptr;
        shader_stages[1].pSpecializationInfo = nullptr;

        // specify how to interpret the vertex buffer data
        auto binding_description = Model::Vertex::GetBindingDescriptions();
        auto attribute_description = Model::Vertex::GetAttributeDescriptions();
        VkPipelineVertexInputStateCreateInfo vertex_input_info{};
        vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertex_input_info.vertexAttributeDescriptionCount = static_cast<uint32_t>(attribute_description.size());
        vertex_input_info.vertexBindingDescriptionCount = static_cast<uint32_t>(binding_description.size());
        vertex_input_info.pVertexAttributeDescriptions = attribute_description.data();
        vertex_input_info.pVertexBindingDescriptions = binding_description.data();

        VkGraphicsPipelineCreateInfo pipeline_info{};
        pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipeline_info.stageCount = 2;
        pipeline_info.pStages = shader_stages;
        pipeline_info.pVertexInputState = &vertex_input_info;
        pipeline_info.pInputAssemblyState = &info._input_assembly_info;
        pipeline_info.pViewportState = &info._viewport_info;
        pipeline_info.pRasterizationState = &info._rasterization_info;
        pipeline_info.pMultisampleState = &info._multisample_info;
        pipeline_info.pColorBlendState = &info._color_blend_info;
        pipeline_info.pDepthStencilState = &info._depth_stencil_info;
        pipeline_info.pDynamicState = &info._dynamic_state_info;
        pipeline_info.layout = info._pipeline_layout;
        pipeline_info.renderPass = info._render_pass;
        pipeline_info.subpass = info._subpass;

        pipeline_info.basePipelineIndex = -1;
        pipeline_info.basePipelineHandle = VK_NULL_HANDLE;

        if (vkCreateGraphicsPipelines(m_device.GetDevice(), VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &m_graphics_pipeline) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create graphics pipeline!");
        }
    }

    void Pipeline::CreateShaderModule(const std::vector<char>& code, VkShaderModule *shader_module)
    {
        VkShaderModuleCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        create_info.codeSize = code.size();
        create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if(vkCreateShaderModule(m_device.GetDevice(), &create_info, NULL, shader_module) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create shader module!");
        }
    }

    void Pipeline::DefaultConfig(PipelineConfigInfo& info)
    {
        info._input_assembly_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        info._input_assembly_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        info._input_assembly_info.primitiveRestartEnable = VK_FALSE;

        // combine viewport and scissor into a viewport state
        info._viewport_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        info._viewport_info.viewportCount = 1;
        info._viewport_info.pViewports = nullptr;
        info._viewport_info.scissorCount = 1;
        info._viewport_info.pScissors = nullptr;

        info._rasterization_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        info._rasterization_info.depthClampEnable = VK_FALSE;
        info._rasterization_info.rasterizerDiscardEnable = VK_FALSE;
        info._rasterization_info.polygonMode = VK_POLYGON_MODE_FILL;
        info._rasterization_info.lineWidth = 1.0f;
        info._rasterization_info.cullMode = VK_CULL_MODE_NONE; // no culling
        info._rasterization_info.frontFace = VK_FRONT_FACE_CLOCKWISE;
        info._rasterization_info.depthBiasEnable = VK_FALSE;
        info._rasterization_info.depthBiasConstantFactor = 0.0f;    // optional
        info._rasterization_info.depthBiasClamp = 0.0f;             // optional
        info._rasterization_info.depthBiasSlopeFactor = 0.0f;       // optional

        info._multisample_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        info._multisample_info.sampleShadingEnable = VK_FALSE;
        info._multisample_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        info._multisample_info.minSampleShading = 1.0f;
        info._multisample_info.pSampleMask = nullptr;
        info._multisample_info.alphaToCoverageEnable = VK_FALSE;
        info._multisample_info.alphaToOneEnable = VK_FALSE;

        info._color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        info._color_blend_attachment.blendEnable = VK_FALSE;
        info._color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
        info._color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
        info._color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
        info._color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        info._color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        info._color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;

        info._color_blend_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        info._color_blend_info.logicOpEnable = VK_FALSE;
        info._color_blend_info.logicOp = VK_LOGIC_OP_COPY;
        info._color_blend_info.attachmentCount = 1;
        info._color_blend_info.pAttachments = &info._color_blend_attachment;
        info._color_blend_info.blendConstants[0] = 0.0f;
        info._color_blend_info.blendConstants[1] = 0.0f;
        info._color_blend_info.blendConstants[2] = 0.0f;
        info._color_blend_info.blendConstants[3] = 0.0f;

        info._depth_stencil_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        info._depth_stencil_info.depthTestEnable = VK_TRUE;
        info._depth_stencil_info.depthWriteEnable = VK_TRUE;
        info._depth_stencil_info.depthCompareOp = VK_COMPARE_OP_LESS;
        info._depth_stencil_info.depthBoundsTestEnable = VK_FALSE;
        info._depth_stencil_info.minDepthBounds = 0.0f;
        info._depth_stencil_info.maxDepthBounds = 1.0f;
        info._depth_stencil_info.stencilTestEnable = VK_FALSE;
        info._depth_stencil_info.front = {};
        info._depth_stencil_info.back = {};

        info._dynamic_states = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
        info._dynamic_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        info._dynamic_state_info.pDynamicStates = info._dynamic_states.data();
        info._dynamic_state_info.dynamicStateCount = static_cast<uint32_t>(info._dynamic_states.size());
        info._dynamic_state_info.flags = 0;
    }
} // namespace DORY