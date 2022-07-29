#include "core/core.h"
#include "renderer/descriptor.h"

namespace DORY
{
    DescriptorSetLayout::DescriptorSetLayout(Device &device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings)
    : m_device{device}, m_bindings{bindings} 
    {
        std::vector<VkDescriptorSetLayoutBinding> set_layout_bindings{};
        for (auto kv : m_bindings) 
        {
            set_layout_bindings.push_back(kv.second);
        }

        VkDescriptorSetLayoutCreateInfo descriptor_set_layout_info{};
        descriptor_set_layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        descriptor_set_layout_info.bindingCount = static_cast<uint32_t>(set_layout_bindings.size());
        descriptor_set_layout_info.pBindings = set_layout_bindings.data();

            if (vkCreateDescriptorSetLayout(m_device.GetDevice(), &descriptor_set_layout_info, nullptr, &m_descriptor_set_layout) != VK_SUCCESS) 
            {
                throw std::runtime_error("failed to create descriptor set layout!");
            }
    }

    DescriptorSetLayout::~DescriptorSetLayout() 
    {
        vkDestroyDescriptorSetLayout(m_device.GetDevice(), m_descriptor_set_layout, nullptr);
    }

    DescriptorPool::DescriptorPool(Device &device, uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags, const std::vector<VkDescriptorPoolSize> &poolSizes)
    : m_device{device} 
    {
        VkDescriptorPoolCreateInfo descriptorPoolInfo{};
        descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        descriptorPoolInfo.pPoolSizes = poolSizes.data();
        descriptorPoolInfo.maxSets = maxSets;
        descriptorPoolInfo.flags = poolFlags;

        if (vkCreateDescriptorPool(m_device.GetDevice(), &descriptorPoolInfo, nullptr, &m_descriptor_pool) != VK_SUCCESS) 
        {
            throw std::runtime_error("failed to create descriptor pool!");
        }
    }

    DescriptorPool::~DescriptorPool() 
    {
        vkDestroyDescriptorPool(m_device.GetDevice(), m_descriptor_pool, nullptr);
    }

    bool DescriptorPool::AllocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const 
    {
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = m_descriptor_pool;
        allocInfo.pSetLayouts = &descriptorSetLayout;
        allocInfo.descriptorSetCount = 1;

        if (vkAllocateDescriptorSets(m_device.GetDevice(), &allocInfo, &descriptor) != VK_SUCCESS) 
        {
            return false;
        }

        return true;
    }

    void DescriptorPool::FreeDescriptors(std::vector<VkDescriptorSet> &descriptors) const 
    {
        vkFreeDescriptorSets(m_device.GetDevice(), m_descriptor_pool, static_cast<uint32_t>(descriptors.size()), descriptors.data());
    }

    void DescriptorPool::ResetPool() 
    {
        vkResetDescriptorPool(m_device.GetDevice(), m_descriptor_pool, 0);
    }
    
    DescriptorWriter::DescriptorWriter(DescriptorSetLayout &set_layout, DescriptorPool &pool)
    : m_set_layout{set_layout}, m_pool{pool} 
    { }

    DescriptorWriter &DescriptorWriter::WriteBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo) 
    {
        DASSERT_MSG(m_set_layout.m_bindings.count(binding) == 1, "Layout does not contain specified binding");

        auto &binding_description = m_set_layout.m_bindings[binding];
        DASSERT_MSG(binding_description.descriptorCount == 1, "Binding single descriptor info, but binding expects multiple");

        VkWriteDescriptorSet write{};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.descriptorType = binding_description.descriptorType;
        write.dstBinding = binding;
        write.pBufferInfo = bufferInfo;
        write.descriptorCount = 1;

        m_writes.push_back(write);
        return *this;
    }

    DescriptorWriter &DescriptorWriter::WriteImage(uint32_t binding, VkDescriptorImageInfo *imageInfo) 
    {
        DASSERT_MSG(m_set_layout.m_bindings.count(binding) == 1, "Layout does not contain specified binding");

        auto &binding_description = m_set_layout.m_bindings[binding];
        DASSERT_MSG(binding_description.descriptorCount == 1, "Binding single descriptor info, but binding expects multiple");

        VkWriteDescriptorSet write{};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.descriptorType = binding_description.descriptorType;
        write.dstBinding = binding;
        write.pImageInfo = imageInfo;
        write.descriptorCount = 1;

        m_writes.push_back(write);
        return *this;
    }

    bool DescriptorWriter::Build(VkDescriptorSet &set) 
    {
        bool success = m_pool.AllocateDescriptor(m_set_layout.GetDescriptorSetLayout(), set);
        if (!success) 
        {
            return false;
        }

        Overwrite(set);
        return true;
    }

    void DescriptorWriter::Overwrite(VkDescriptorSet &set) 
    {
        for (auto &write : m_writes) 
        {
            write.dstSet = set;
        }
        vkUpdateDescriptorSets(m_pool.m_device.GetDevice(), m_writes.size(), m_writes.data(), 0, nullptr);
    }

} // namespace DORY
