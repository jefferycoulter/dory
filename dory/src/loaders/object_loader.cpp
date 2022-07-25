#define TINYOBJLOADER_IMPLEMENTATION
#include "object_loader.h"
#include "math/hash.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <unordered_map>

namespace std
{
    template<>
    struct hash<DORY::Model::Vertex>
    {
        size_t operator()(const DORY::Model::Vertex& vertex) const
        {
            size_t seed = 0;
            DORY::HashCombine(seed, vertex.a_position, vertex.a_color, vertex.a_normal, vertex.a_tex_coords);
            return seed;
        }
    };
}

namespace DORY
{
    void ObjectLoader::Load(Model::Mesh& dmesh, const std::string& path)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn;
        std::string err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str()))
        {
            throw std::runtime_error(warn + err);
        }

        dmesh.indices.clear();
        dmesh.vertices.clear();

        // https://vulkan-tutorial.com/Loading_models
        std::unordered_map<Model::Vertex, uint32_t> unique_vertices{};
        for (const auto& shape : shapes)
        {
            for (const auto& index : shape.mesh.indices)
            {
                Model::Vertex vertex{};
                // specify the position of the vertex (and color if provided)
                if (index.vertex_index >= 0)
                {
                    vertex.a_position = {
                        attrib.vertices[3 * index.vertex_index + 0], // x
                        attrib.vertices[3 * index.vertex_index + 1], // y
                        attrib.vertices[3 * index.vertex_index + 2]  // z
                    };

                    auto color_index = 3 * index.vertex_index + 2;
                    if (color_index < attrib.colors.size())
                    {
                        vertex.a_color = {
                            attrib.colors[color_index -2], // r
                            attrib.colors[color_index -1], // g
                            attrib.colors[color_index - 0] // b
                        };
                    }
                    else
                    {
                        vertex.a_color = {1.0f, 1.0f, 1.0f};
                    }
                }

                // specify the normal of the vertex
                if (index.normal_index >= 0)
                {
                    vertex.a_normal = {
                        attrib.normals[3 * index.normal_index + 0],
                        attrib.normals[3 * index.normal_index + 1], 
                        attrib.normals[3 * index.normal_index + 2] 
                    };
                }

                // specify the texture coordinates of the vertex
                if (index.texcoord_index >= 0)
                {
                    vertex.a_tex_coords = {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        attrib.texcoords[2 * index.texcoord_index + 1]
                    };
                }

                // check if this vertex already exists
                if (unique_vertices.count(vertex) == 0)
                {
                    unique_vertices[vertex] = static_cast<uint32_t>(dmesh.vertices.size());
                    dmesh.vertices.push_back(vertex);
                }

                // add the index to the indices list
                dmesh.indices.push_back(unique_vertices[vertex]);
            }
        }
    }
} // namespace DORY