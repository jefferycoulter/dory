#include "core/core.h"
#include "renderer/buffer.h"

namespace DORY
{
    VkDeviceSize Buffer::GetAlignment(VkDeviceSize instance_size, VkDeviceSize min_offset_alignment)
    {
        if (min_offset_alignment > 0)
        {
            return (instance_size + min_offset_alignment - 1) & ~(min_offset_alignment - 1);
        }
        return instance_size;
    }

    Buffer::Buffer(Device &device, VkDeviceSize instance_size, uint32_t instance_count, VkBufferUsageFlags usage_flags, VkMemoryPropertyFlags memory_property_flags, VkDeviceSize min_offset_alignment)
        : m_device{device}, m_instance_size{instance_size}, m_instance_count{instance_count}, m_usage_flags{usage_flags}, m_memory_property_flags{memory_property_flags}
        {
            m_alignment_size = GetAlignment(instance_size, min_offset_alignment);
            m_buffer_size = m_alignment_size * instance_count;
            device.CreateBuffer(m_buffer_size, usage_flags, memory_property_flags, m_buffer, m_memory);
        }

    Buffer::~Buffer()
    {
        Unmap();
        vkDestroyBuffer(m_device.GetDevice(), m_buffer, nullptr);
        vkFreeMemory(m_device.GetDevice(), m_memory, nullptr);
    }

    VkResult Buffer::Map(VkDeviceSize size, VkDeviceSize offset)
    {
        DASSERT_MSG(m_buffer && m_memory, "Cannot map buffer before it has been created");
        return vkMapMemory(m_device.GetDevice(), m_memory, offset, size, 0, &m_mapped);
    }

    void Buffer::Unmap() 
    {
        if (m_mapped) 
        {
            vkUnmapMemory(m_device.GetDevice(), m_memory);
            m_mapped = nullptr;
        }
    }

    void Buffer::WriteToBuffer(void *data, VkDeviceSize size, VkDeviceSize offset)
    {
        DASSERT_MSG(m_mapped, "Cannot copy to unmapped buffer");

        if (size == VK_WHOLE_SIZE) 
        {
            memcpy(m_mapped, data, m_buffer_size);
        } 
        else
        {
            char *mem_offset = (char *)m_mapped;
            mem_offset += offset;
            memcpy(mem_offset, data, size);
        }
    }

    VkResult Buffer::Flush(VkDeviceSize size, VkDeviceSize offset) 
    {
        VkMappedMemoryRange mapped_range = {};
        mapped_range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        mapped_range.memory = m_memory;
        mapped_range.offset = offset;
        mapped_range.size = size;
        return vkFlushMappedMemoryRanges(m_device.GetDevice(), 1, &mapped_range);
    }

    VkResult Buffer::Invalidate(VkDeviceSize size, VkDeviceSize offset)
    {
        VkMappedMemoryRange mapped_range = {};
        mapped_range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        mapped_range.memory = m_memory;
        mapped_range.offset = offset;
        mapped_range.size = size;
        return vkInvalidateMappedMemoryRanges(m_device.GetDevice(), 1, &mapped_range);
    }

    VkDescriptorBufferInfo Buffer::DescriptorInfo(VkDeviceSize size, VkDeviceSize offset)
    {
        return VkDescriptorBufferInfo{m_buffer, offset, size};
    }

    void Buffer::WriteToIndex(void *data, int index)
    {
        WriteToBuffer(data, m_instance_size, index * m_alignment_size);
    }

    VkResult Buffer::FlushIndex(int index)
    {
        return Flush(m_alignment_size, index * m_alignment_size); 
    }

    VkDescriptorBufferInfo Buffer::DescriptorInfoForIndex(int index) 
    {
        return DescriptorInfo(m_alignment_size, index * m_alignment_size);
    }

    VkResult Buffer::InvalidateIndex(int index) 
    {
        return Invalidate(m_alignment_size, index * m_alignment_size);
    }
}  // namespace DORY
