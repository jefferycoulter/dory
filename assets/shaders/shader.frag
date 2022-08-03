#version 450

layout(location = 0) in vec3 frag_color;
layout(location = 1) in vec3 frag_pos_world;
layout(location = 2) in vec3 frag_normal_world;

layout(location = 0) out vec4 FragColor;

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

void main()
{
    // calculate color and lighting
    vec3 light_direction = ubo.light_position.xyz - frag_pos_world;
    float attenuation = 1.0 / dot(light_direction, light_direction);
    vec3 light_color = ubo.light_color.xyz * ubo.light_color.w * attenuation;
    vec3 ambient_light = ubo.ambient_color.xyz * ubo.ambient_color.w;
    vec3 diffuse_light = light_color * max(dot(normalize(frag_normal_world), normalize(light_direction)), 0.0);
    FragColor = vec4((diffuse_light + ambient_light) * frag_color, 1.0);
}