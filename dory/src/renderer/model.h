#ifndef DORY_MODEL_INCL
#define DORY_MODEL_INCL

#include "renderer/device.h"
#include "utils/nocopy.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>
#include <memory>

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
                glm::vec3 a_position{};
                glm::vec3 a_color{};
                glm::vec3 a_normal{};
                glm::vec2 a_tex_coords{};

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

                bool operator==(const Vertex &other) const
                {
                    return a_position == other.a_position &&
                           a_color == other.a_color &&
                           a_normal == other.a_normal &&
                           a_tex_coords == other.a_tex_coords;
                }
            };

            /**
             * @brief the data for a single mesh
             */
            struct Mesh
            {
                std::vector<Vertex> vertices{};
                std::vector<uint32_t> indices{};
            };
            
            /**
             * @brief create a model from a list of vertices
             * 
             * @param device device to create the model on
             * @param data a struct containing the vertices and indices of the model
             */
            Model(Device &device, const Model::Mesh& data);

            /**
             * @brief destroy the Model object
             */
            ~Model();

            /**
             * @brief load a model from a file
             * @param device device to create the model on
             * @param path path to the model file
             * @return std::unique_ptr<Model> 
             */
            static std::unique_ptr<Model> LoadModelFromFile(Device &device, const std::string& path);

            void Bind(VkCommandBuffer command_buffer);
            void Draw(VkCommandBuffer command_buffer);

        private: // methods
            /**
             * @brief create the vertex buffer and its memory
             * 
             * @param vertices list of vertices to create the buffer from
             */
            void CreateVertexBuffers(const std::vector<Vertex> &vertices);

            /**
             * @brief create the index buffer and its memory
             * 
             * @param vertices list of vertices to create the buffer from
             */
            void CreateIndexBuffers(const std::vector<uint32_t> &indices);

        private: // members
            Device &m_device; // device to create the model on
            VkBuffer m_vertex_buffer; // vertex buffer
            VkDeviceMemory m_vertex_buffer_memory; // vertex buffer memory
            uint32_t m_vertex_count; // number of vertices in the buffer

            VkBuffer m_index_buffer; // index buffer
            VkDeviceMemory m_index_buffer_memory; // index buffer memory
            uint32_t m_index_count; // number of indices in the buffer
            bool m_has_indices = false; // whether the model has index buffer
            
    }; // class Model
} // namespace DORY

#endif // DORY_MODEL_INCL