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

            class Builder 
            {
                /**
                 * @brief nested class to help with creating descriptor pools
                 */
                public:
                    Builder(Device &device) 
                    : m_device{device} 
                    {}

                    /**
                     * @brief add a binding to the descriptor set layout
                     * 
                     * @param binding binding index
                     * @param descriptor_type 
                     * @param stage_flags 
                     * @param count 
                     * @return Builder& 
                     */
                    Builder &AddBinding(uint32_t binding, VkDescriptorType descriptor_type, VkShaderStageFlags stage_flags, uint32_t count = 1);
                    
                    /**
                     * @brief create the descriptor set layout
                     * @return std::unique_ptr<DescriptorSetLayout> 
                     */
                    std::unique_ptr<DescriptorSetLayout> Build() const;

                private:
                    Device &m_device;
                    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_bindings{};
            }; // class Builder

            /**
             * @brief Construct a new Descriptor Set Layout object
             * @param device device to create the descriptor set layout on
             * @param bindings bindings to create the descriptor set layout with
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
            
            /**
             * @brief get the bindings
             * @return std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> 
             */
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding>  GetBindings() const { return m_bindings; }

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
             * @brief nested class to help with creating descriptor pools
             */
            class Builder
            {
                public:
                    Builder(Device &device) 
                    : m_device{device} 
                    {}

                    Builder &AddPoolSize(VkDescriptorType descriptor_type, uint32_t count);
                    Builder &SetPoolFlags(VkDescriptorPoolCreateFlags flags);
                    Builder &SetMaxSets(uint32_t count);
                    std::unique_ptr<DescriptorPool> Build() const;

                private:
                    Device &m_device;
                    std::vector<VkDescriptorPoolSize> m_pool_sizes{};
                    uint32_t m_max_sets = 1000;
                    VkDescriptorPoolCreateFlags m_pool_flags = 0;
            }; // class Builder

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
             * @brief allocate space for a descriptor set
             * @param descriptor_set_layout layout of the descriptor set to allocate
             * @param descriptor handle to the descriptor set to be allocated
             * @return true 
             * @return false 
             */
            bool AllocateDescriptor(const VkDescriptorSetLayout descriptor_set_layout, VkDescriptorSet &descriptor) const;

            /**
             * @brief clear the descriptors
             * @param descriptors 
             */
            void FreeDescriptors(std::vector<VkDescriptorSet> &descriptors) const;

            /**
             * @brief clear the pool
             */
            void ResetPool();

        private:
            Device &m_device; // the device this pool is created on
            VkDescriptorPool m_descriptor_pool; // the descriptor pool
        
        private:
            friend class DescriptorWriter;
        }; // class DescriptorPool

    class DescriptorWriter 
    {
        public:
            /**
             * @brief Construct a new Descriptor Writer object
             * @param set_layout descriptor set layout
             * @param pool descriptor pool
             */
            DescriptorWriter(DescriptorSetLayout &set_layout, DescriptorPool &pool);

            /**
             * @brief write data to a buffer
             * @param binding binding index
             * @param buffer_info info to be written
             * @return DescriptorWriter& 
             */
            DescriptorWriter &WriteBuffer(uint32_t binding, VkDescriptorBufferInfo *buffer_info);

            /**
             * @brief write an image to a buffer
             * @param binding binding index
             * @param image_info info to be written
             * @return DescriptorWriter& 
             */
            DescriptorWriter &WriteImage(uint32_t binding, VkDescriptorImageInfo *image_info);


            /**
             * @brief create the descriptor set
             * @param set reference to the descriptor set to be created
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
            DescriptorSetLayout &m_set_layout; // descriptor set layout
            DescriptorPool &m_pool; // descriptor pool
            std::vector<VkWriteDescriptorSet> m_writes; // writes to be performed
    }; // class DescriptorWriter
}  // namespace DORY

#endif // DORY_DESCRIPTOR_INCL