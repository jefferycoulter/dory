#ifndef DORY_PIPELINE_INCL
#define DORY_PIPELINE_INCL

#include "utils/utils.h"
#include "device.h"

#include <iostream>
#include <string>

namespace DORY
{
    struct PipelineConfigInfo
    {

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
            ~Pipeline() {};

            /**
             * @brief default configuration settings for the
             * 
             * @param width 
             * @param height 
             * @return PipelineConfigInfo 
             */
            static PipelineConfigInfo DefaultConfig(uint32_t width, uint32_t height);

        private:
            /**
             * @brief helper function for creating the graphics pipeline
             * 
             * @param vertex_path vertex source file path
             * @param fragment_path fragment source file path 
             */
            void CreatePipeline(const PipelineConfigInfo& info ,const std::string& vertex_path, const std::string& fragment_path);

            void CreateShaderModule(const std::vector<char>& code, VkShaderModule *shader_module);

            Device& _device; // reference to device in pipeline
            VkPipeline _graphics_pipeline; // graphics pipeline handle in pipeline
            VkShaderModule _vert_shader_module; // vertex shader module handle in pipeline
            VkShaderModule _frag_shader_module; // fragment shader module handle in pipeline
    }; // class Pipeline

} // namespace DORY

#endif // DORY_PIPELINE_INCL