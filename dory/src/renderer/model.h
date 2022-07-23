#ifndef DORY_MODEL_INCL
#define DORY_MODEL_INCL

#include "renderer/device.h"
#include "utils/nocopy.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace DORY
{
    class Model : public NoCopy
    {
        public:

            /**
             * @brief struct describing a vertex and its attributes
             */
            struct Vertex
            {
                glm::vec3 a_position;
                glm::vec3 a_color;

                /**
                 * @brief get the binding descriptions of the vertex
                 * 
                 * @return std::vector<VkVertexInputBindingDescription> 
                 */
                static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();

                /**
                 * @brief get the attribute descriptions of the vertex
                 * 
                 * @return std::vector<VkVertexInputAttributeDescription> 
                 */
                static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
            };
            
            /**
             * @brief create a model from a list of vertices
             * 
             * @param device device to create the model on
             * @param vertices list of vertices to create the model from
             */
            Model(Device &device, const std::vector<Vertex> &vertices);

            /**
             * @brief destroy the Model object
             */
            ~Model();

            void Bind(VkCommandBuffer command_buffer);
            void Draw(VkCommandBuffer command_buffer);

        private: // methods
            /**
             * @brief create the vertex buffer and its memory
             * 
             * @param vertices list of vertices to create the buffer from
             */
            void CreateVertexBuffers(const std::vector<Vertex> &vertices);

        private: // members
            Device &m_device;
            VkBuffer m_vertex_buffer;
            VkDeviceMemory m_vertex_buffer_memory;
            uint32_t m_vertex_count;
    }; // class Model
} // namespace DORY

#endif // DORY_MODEL_INCL