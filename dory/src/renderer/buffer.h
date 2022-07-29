#ifndef DORY_BUFFER_INCL
#define DORY_BUFFER_INCL

#include "renderer/device.h"
#include "utils/nocopy.h"

namespace DORY
{
    class Buffer : public NoCopy
    {
        public:
            /**
             * @brief construct a new Buffer object
             * @param device the device to create the buffer on
             * @param instance_size the size of an instance in the buffer
             * @param instance_count the number of instances in the buffer
             * @param usage_flags the usage flags for the buffer
             * @param memory_property_flags the memory property flags to use for the buffer
             * @param min_offset_alightment (optional) the minimum offset alignment for the buffer. default value is 1 since
             * vertex buffers are aligned to 4 don't have to worry about this.
             */
            Buffer(
                Device& device,
                VkDeviceSize instance_size,
                uint32_t instance_count,
                VkBufferUsageFlags usage_flags,
                VkMemoryPropertyFlags memory_property_flags,
                VkDeviceSize min_offset_alightment = 1);

            /**
             * @brief destroy the buffer
             */
            ~Buffer();
            
            /**
             * @brief map a memory range of this buffer. If successful, mapped points to the specified buffer range.
             * @param size (optional) size of the memory range to map. default value maps the whole buffer.
             * @param offset (optional) byte offset from beginning
             * @return VkResult 
             */
            VkResult Map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

            /**
             * @brief unmap a mapped memory range
             */
            void Unmap();
            
            /**
             * @brief copies the specified data to the mapped buffer.
             * @param data pointer to the data to copy
             * @param size (optional) size of the data to copy. default value flushes the complete buffer range.
             * @param offset (optional) Byte offset from beginning of mapped region
             *
             */
            void WriteToBuffer(void* data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

            /**
             * @brief flush a memory range of the buffer to make it visible to the device. only required for non-coherent memory
             * @param size (optional) size of the memory range to flush. default value flushes the complete buffer range.
             * @param offset (optional) byte offset from beginning
             * @return VkResult
             */
            VkResult Flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

            /**
             * @brief create a buffer info descriptor
             * @param size (optional) size of the memory range of the descriptor
             * @param offset (optional) byte offset from beginning
             * @return VkDescriptorBufferInfo
             */
            VkDescriptorBufferInfo DescriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

            /**
             * @brief invalidate a memory range of the buffer to make it visible to the host. only required for non-coherent memory
             * @param size (optional) Size of the memory range to invalidate. default value invalidates the complete buffer range.
             * @param offset (optional) Byte offset from beginning
             * @return VkResult
             */
            VkResult Invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
            
            /**
             * @brief copies m_instance_size bytes of data to the mapped buffer at an offset of index * m_alignment_size
             * @param data Pointer to the data to copy
             * @param index Used in offset calculation
             */
            void WriteToIndex(void* data, int index);

            /**
             * @brief flush the memory range at index * m_alignment_size of the buffer to make it visible to the device
             * @param index used in offset calculation
             */
            VkResult FlushIndex(int index);

            /**
             * @brief create a buffer info descriptor
             * @param index Specifies the region given by index * m_alignment_size
             * @return VkDescriptorBufferInfo 
             */
            VkDescriptorBufferInfo DescriptorInfoForIndex(int index);

            /**
             * @brief a memory range of the buffer to make it visible to the host. only required for non-coherent memory
             * @param index specifies the region to invalidate: index * m_alignment_size
             * @return VkResult
             */
            VkResult InvalidateIndex(int index);
            
            VkBuffer GetBuffer() const { return m_buffer; }
            void* GetMappedMemory() const { return m_mapped; }
            uint32_t GetInstanceCount() const { return m_instance_count; }
            VkDeviceSize GetInstanceSize() const { return m_instance_size; }
            VkDeviceSize GetAlignmentSize() const { return m_instance_size; }
            VkBufferUsageFlags GetUsageFlags() const { return m_usage_flags; }
            VkMemoryPropertyFlags GetMemoryPropertyFlags() const { return m_memory_property_flags; }
            VkDeviceSize GetBufferSize() const { return m_buffer_size; }
            
        private: // methods
            /**
             * @brief returns the minimum instance size required to be compatible with the device's min_offset_alignment
             * @param instance_size the size of an instance
             * @param min_offset_alignment the minimum required alignment in bytes for the offset member
             * @return VkResult
             */
            static VkDeviceSize GetAlignment(VkDeviceSize instance_size, VkDeviceSize min_offset_alignment);
        
        private: // members
            Device& m_device; // the device to create the buffer on
            void* m_mapped = nullptr; // pointer to the mapped memory range
            VkBuffer m_buffer = VK_NULL_HANDLE;
            VkDeviceMemory m_memory = VK_NULL_HANDLE;
            
            VkDeviceSize m_buffer_size; // size of the buffer in bytes
            uint32_t m_instance_count; // number of instances in the buffer
            VkDeviceSize m_instance_size; // size of an instance in the buffer
            VkDeviceSize m_alignment_size; // alignment of the buffer in bytes
            VkBufferUsageFlags m_usage_flags; // usage flags for the buffer
            VkMemoryPropertyFlags m_memory_property_flags; // memory property flags for the buffer
    }; // class Buffer
} // namespace DORY

#endif // DORY_BUFFER_INCL