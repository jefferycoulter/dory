#ifndef DORY_PIPELINE_INCL
#define DORY_PIPELINE_INCL

#include "device.h"
#include "model.h"
#include "utils/nocopy.h"
#include "utils/utils.h"

#include <iostream>
#include <string>

namespace DORY
{
    struct PipelineConfigInfo : NoCopy
    {   
        // set in Pipeline::DefaultConfig()
        VkPipelineViewportStateCreateInfo _viewport_info;
        VkPipelineInputAssemblyStateCreateInfo _input_assembly_info; // first stage of graphics pipeline
        VkPipelineRasterizationStateCreateInfo _rasterization_info; // next stage after vertex shader
        VkPipelineMultisampleStateCreateInfo _multisample_info;
        VkPipelineColorBlendAttachmentState _color_blend_attachment; // stage following fragment shader
        VkPipelineColorBlendStateCreateInfo _color_blend_info;
        VkPipelineDepthStencilStateCreateInfo _depth_stencil_info;
        std::vector<VkDynamicState> _dynamic_states;
        VkPipelineDynamicStateCreateInfo _dynamic_state_info;

        // set elsewhere
        VkPipelineLayout _pipeline_layout = nullptr;
        VkRenderPass _render_pass = nullptr;
        uint32_t _subpass = 0;
    };

    /**
     * @brief class implementing a graphics pipeline.  since a pipeline in Vulkan is "immutable", this provies
     * and default configuration as well as a way to create a pipeline from a configuration using the 
     * PipelineConfigInfo struct.
     */
    class Pipeline
    {
        public:
            /**
             * @brief construct a new Pipeline object
             * 
             * @param device reference to the device the pipeline will run on
             * @param info configuration info for the pipeline
             * @param vertex_path vertex source file path
             * @param fragment_path fragment source file path
             */
            Pipeline(Device& device, const PipelineConfigInfo& info, const std::string& vertex_path, const std::string& fragment_path);
            
            /**
             * @brief destroy the Pipeline object
             */
            ~Pipeline();

            /**
             * @brief bind the pipeline to the given command buffer
             */
            void Bind(VkCommandBuffer command_buffer);

            /**
             * @brief default configuration settings for the
             * 
             * @param width 
             * @param height 
             * @return PipelineConfigInfo 
             */
            static void DefaultConfig(PipelineConfigInfo& info);

        private: // methods
            /**
             * @brief helper function for creating the graphics pipeline
             * 
             * @param vertex_path vertex source file path
             * @param fragment_path fragment source file path 
             */
            void CreatePipeline(const PipelineConfigInfo& info ,const std::string& vertex_path, const std::string& fragment_path);

            /**
             * @brief create the shader modules for the pipeline
             * @param code path to shader source code
             * @param shader_module pointer vulkan shader module to be created
             */
            void CreateShaderModule(const std::vector<char>& code, VkShaderModule *shader_module);

        private: // members
            Device& m_device; // reference to device in pipeline
            VkPipeline m_graphics_pipeline; // graphics pipeline handle in pipeline
            VkShaderModule m_vert_shader_module; // vertex shader module handle in pipeline
            VkShaderModule m_frag_shader_module; // fragment shader module handle in pipeline
            
    }; // class Pipeline

} // namespace DORY

#endif // DORY_PIPELINE_INCL