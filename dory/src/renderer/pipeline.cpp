#include "pipeline.h"

namespace DORY
{
    Pipeline::Pipeline(Device& device, const PipelineConfigInfo& info, const std::string& vertex_path, const std::string& fragment_path)
        :_device(device)
    {
        CreatePipeline(info, vertex_path, fragment_path);
    }

    void Pipeline::CreatePipeline(const PipelineConfigInfo& info, const std::string& vertex_path, const std::string& fragment_path)
    {
        auto vertex_code = Utils::ReadFile(vertex_path);
        auto fragment_code = Utils::ReadFile(fragment_path);

        std::cout << "vertex code size: " << vertex_code.size() << "\n";
        std::cout << "fragment code size: " << fragment_code.size() << "\n";
    }

    void Pipeline::CreateShaderModule(const std::vector<char>& code, VkShaderModule *shader_module)
    {
        VkShaderModuleCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        create_info.codeSize = code.size();
        create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if(vkCreateShaderModule(_device.device(), &create_info, NULL, shader_module) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create shader module!");
        }
    }

    PipelineConfigInfo Pipeline::DefaultConfig(uint32_t width, uint32_t height)
    {
        PipelineConfigInfo info{};

        return info;
    }

} // namespace DORY