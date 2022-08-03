#version 450

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_color;
layout(location = 2) in vec3 a_normal;
layout(location = 3) in vec2 a_texcoord;

layout(location = 0) out vec3 frag_color;
layout(location = 1) out vec3 frag_pos_world;
layout(location = 2) out vec3 frag_normal_world;

layout(set = 0, binding = 0) uniform UBO
{
    mat4 projection;
    mat4 view;
    vec4 ambient_color;
    vec4 light_position;
    vec4 light_color;
} ubo;

layout(push_constant) uniform Push
{
    mat4 model_matrix;
    mat4 normal_matrix;
} push;

void main() {
    vec4 world_position = push.model_matrix * vec4(a_position, 1.0);
    gl_Position = ubo.projection * ubo.view * push.model_matrix * vec4(a_position, 1.0);

    // convert normals in model space to normals in world space
    frag_normal_world = normalize(mat3(push.normal_matrix) * a_normal);
    frag_pos_world = world_position.xyz;
    frag_color = a_color;
}