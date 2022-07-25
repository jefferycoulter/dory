#version 450

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_color;
layout(location = 2) in vec3 a_normal;
layout(location = 3) in vec2 a_texcoord;

layout(location = 0) out vec3 frag_color;

layout(push_constant) uniform Push
{
    mat4 transform;
    mat4 normal_matrix;
} push;

const vec3 LIGHT_DIRECTION = normalize(vec3(1.0, 3.0, -1.0));
const float AMBIENCE = 0.3;

void main() {
    gl_Position = push.transform * vec4(a_position, 1.0);

    // convert normals in model space to normals in world space
    vec3 normal_in_world_space = normalize(mat3(push.normal_matrix) * a_normal);

    float light_intensity = AMBIENCE + max(dot(normal_in_world_space, LIGHT_DIRECTION), 0.0);

    frag_color = light_intensity * a_color;
}