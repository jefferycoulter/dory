#include "core/logger.h"
#include "loaders/object_loader.h"
#include "renderer/model.h"

#include <cassert>
#include <cstring>

namespace DORY
{
    Model::Model(Device &device, const Mesh& mesh)
        : m_device(device)
    {
        CreateVertexBuffers(mesh.vertices);
        m_index_count = static_cast<uint32_t>(mesh.indices.size());
        m_has_indices = m_index_count > 0;
        if (m_has_indices)
        {
            CreateIndexBuffers(mesh.indices);
        }
    }

    Model::~Model()
    {
    }

    std::unique_ptr<Model> Model::LoadModelFromFile(Device &device, const std::string& path)
    {
        Mesh mesh{};
        ObjectLoader::Load(mesh, path);
        DINFO("Loaded model from file: %s", path.c_str());
        DINFO("Model has %d vertices and %d indices", mesh.vertices.size(), mesh.indices.size());
        return std::make_unique<Model>(device, mesh);
    }

    void Model::CreateVertexBuffers(const std::vector<Vertex> &vertices)
    {
        m_vertex_count = static_cast<uint32_t>(vertices.size());
        assert(m_vertex_count >= 3 && "Model must have at least 3 vertices");
        VkDeviceSize buffer_size = sizeof(vertices[0]) * m_vertex_count;
        uint32_t vertex_size = sizeof(vertices[0]);

        // stage the vertex data to the device local memory for faster access
        // note: this is declared on the stack and will be destroyed when the function returns
        Buffer staging_buffer(  m_device,
                                vertex_size, 
                                m_vertex_count, 
                                VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        // take data from host CPU and copy it to the stageing buffer on GPU
        staging_buffer.Map();
        staging_buffer.WriteToBuffer((void*)vertices.data());

        m_vertex_buffer = std::make_unique<Buffer>( m_device, 
                                                    vertex_size, 
                                                    m_vertex_count, 
                                                    VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, 
                                                    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        // copy the data from the staging buffer to the vertex buffer
        m_device.CopyBuffer(staging_buffer.GetBuffer(), m_vertex_buffer->GetBuffer(), buffer_size);
    }

    void Model::CreateIndexBuffers(const std::vector<uint32_t> &indices)
    {
        VkDeviceSize buffer_size = sizeof(indices[0]) * m_index_count;
        uint32_t index_size = sizeof(indices[0]);

        // stage the index data to the device local memory for faster access
        Buffer staging_buffer(  m_device,
                                index_size, 
                                m_index_count, 
                                VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        // take data from host CPU and copy it to the stageing buffer on GPU
        staging_buffer.Map();
        staging_buffer.WriteToBuffer((void*)indices.data());

        m_index_buffer = std::make_unique<Buffer>(  m_device, 
                                                    index_size, 
                                                    m_index_count, 
                                                    VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, 
                                                    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        // copy the data from the staging buffer to the index buffer
        m_device.CopyBuffer(staging_buffer.GetBuffer(), m_index_buffer->GetBuffer(), buffer_size);
    }

    void Model::Bind(VkCommandBuffer command_buffer)
    {
        VkBuffer buffers[] = { m_vertex_buffer->GetBuffer() };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(command_buffer, 0, 1, buffers, offsets);

        if (m_has_indices)
        {
            vkCmdBindIndexBuffer(command_buffer, m_index_buffer->GetBuffer(), 0, VK_INDEX_TYPE_UINT32);
        }
    }

    void Model::Draw(VkCommandBuffer command_buffer)
    {
        if (m_has_indices) { vkCmdDrawIndexed(command_buffer, m_index_count, 1, 0, 0, 0); }
        else { vkCmdDraw(command_buffer, m_vertex_count, 1, 0, 0); }
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
        std::vector<VkVertexInputAttributeDescription> attribute_descriptions(4);
        attribute_descriptions[0].binding = 0;
        attribute_descriptions[0].location = 0;
        attribute_descriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attribute_descriptions[0].offset = offsetof(Vertex, a_position);

        attribute_descriptions[1].binding = 0;
        attribute_descriptions[1].location = 1;
        attribute_descriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attribute_descriptions[1].offset = offsetof(Vertex, a_color);

        attribute_descriptions[2].binding = 0;
        attribute_descriptions[2].location = 2;
        attribute_descriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
        attribute_descriptions[2].offset = offsetof(Vertex, a_normal);

        attribute_descriptions[3].binding = 0;
        attribute_descriptions[3].location = 3;
        attribute_descriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
        attribute_descriptions[3].offset = offsetof(Vertex, a_tex_coords);

        return attribute_descriptions;
    }
} // namespace DORY