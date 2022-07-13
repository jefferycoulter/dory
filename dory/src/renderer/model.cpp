#include "model.h"

#include <cassert>
#include <cstring>

namespace DORY
{
    Model::Model(Device &device, const std::vector<Vertex> &vertices)
    : m_device(device)
    {
        CreateVertexBuffers(vertices);
    }

    Model::~Model()
    {
        vkDestroyBuffer(m_device.GetDevice(), m_vertex_buffer, nullptr);
        vkFreeMemory(m_device.GetDevice(), m_vertex_buffer_memory, nullptr);
    }

    void Model::CreateVertexBuffers(const std::vector<Vertex> &vertices)
    {
        m_vertex_count = static_cast<uint32_t>(vertices.size());
        assert(m_vertex_count >= 3 && "Model must have at least 3 vertices");
        VkDeviceSize buffer_size = sizeof(vertices[0]) * m_vertex_count;

        m_device.CreateBuffer(buffer_size, 
                            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                            m_vertex_buffer, 
                            m_vertex_buffer_memory);

        // take data from host CPU and copy it to the GPU
        void *data;
        vkMapMemory(m_device.GetDevice(), m_vertex_buffer_memory, 0, buffer_size, 0, &data);
        memcpy(data, vertices.data(), static_cast<size_t>(buffer_size));
        vkUnmapMemory(m_device.GetDevice(), m_vertex_buffer_memory);
    }

    void Model::Bind(VkCommandBuffer command_buffer)
    {
        VkBuffer buffers[] = { m_vertex_buffer };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(command_buffer, 0, 1, buffers, offsets);
    }

    void Model::Draw(VkCommandBuffer command_buffer)
    {
        vkCmdDraw(command_buffer, m_vertex_count, 1, 0, 0);
    }

    std::vector<VkVertexInputBindingDescription> Model::Vertex::GetBindingDescriptions()
    {
        std::vector<VkVertexInputBindingDescription> binding_descriptions(1);
        binding_descriptions[0].binding = 0;
        binding_descriptions[0].stride = sizeof(Vertex);
        binding_descriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return binding_descriptions;
    }

    std::vector<VkVertexInputAttributeDescription> Model::Vertex::GetAttributeDescriptions()
    {
        std::vector<VkVertexInputAttributeDescription> attribute_descriptions(1);
        attribute_descriptions[0].binding = 0;
        attribute_descriptions[0].location = 0;
        attribute_descriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attribute_descriptions[0].offset = 0;
        return attribute_descriptions;
    }

} // namespace DORY