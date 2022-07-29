#ifndef DORY_DESCRIPTOR_INCL
#define DORY_DESCRIPTOR_INCL

#include "renderer/device.h"
#include "utils/nocopy.h"

// std
#include <memory>
#include <unordered_map>
#include <vector>

namespace DORY 
{
    class DescriptorSetLayout : public NoCopy
    {
        public:
            /**
             * @brief Construct a new Descriptor Set Layout object
             * @param device 
             * @param bindings 
             */
            DescriptorSetLayout(Device &device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
            
            /**
             * @brief Destroy the Descriptor Set Layout object
             */
            ~DescriptorSetLayout();

            /**
             * @brief Get the Descriptor Set Layout object
             * @return VkDescriptorSetLayout 
             */
            VkDescriptorSetLayout GetDescriptorSetLayout() const { return m_descriptor_set_layout; }

        private:
            Device &m_device;
            VkDescriptorSetLayout m_descriptor_set_layout;
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_bindings;
        
        private:
            friend class DescriptorWriter;
    }; // class DescriptorSetLayout

    class DescriptorPool : public NoCopy
    {
        public:
            /**
             * @brief Construct a new Descriptor Pool object
             * @param device 
             * @param max_sets 
             * @param pool_flags 
             * @param pool_sizes 
             */
            DescriptorPool(Device &device, uint32_t max_sets, VkDescriptorPoolCreateFlags pool_flags, const std::vector<VkDescriptorPoolSize> &pool_sizes);
            
            /**
             * @brief Destroy the Descriptor Pool object
             */
            ~DescriptorPool();

            /**
             * @brief 
             * @param descriptorSetLayout 
             * @param descriptor 
             * @return true 
             * @return false 
             */
            bool AllocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const;

            /**
             * @brief 
             * @param descriptors 
             */
            void FreeDescriptors(std::vector<VkDescriptorSet> &descriptors) const;

            /**
             * @brief 
             */
            void ResetPool();

        private:
            Device &m_device;
            VkDescriptorPool m_descriptor_pool;
        
        private:
            friend class DescriptorWriter;
        }; // class DescriptorPool

    class DescriptorWriter 
    {
        public:
            /**
             * @brief Construct a new Descriptor Writer object
             * @param set_layout 
             * @param pool 
             */
            DescriptorWriter(DescriptorSetLayout &set_layout, DescriptorPool &pool);

            /**
             * @brief 
             * @param binding 
             * @param buffer_info 
             * @return DescriptorWriter& 
             */
            DescriptorWriter &WriteBuffer(uint32_t binding, VkDescriptorBufferInfo *buffer_info);

            /**
             * @brief 
             * @param binding 
             * @param image_info 
             * @return DescriptorWriter& 
             */
            DescriptorWriter &WriteImage(uint32_t binding, VkDescriptorImageInfo *image_info);


            /**
             * @brief 
             * @param set 
             * @return true 
             * @return false 
             */
            bool Build(VkDescriptorSet &set);

            /**
             * @brief 
             * @param set 
             */
            void Overwrite(VkDescriptorSet &set);

        private:
            DescriptorSetLayout &m_set_layout;
            DescriptorPool &m_pool;
            std::vector<VkWriteDescriptorSet> m_writes;
    }; // class DescriptorWriter
}  // namespace DORY

#endif // DORY_DESCRIPTOR_INCL