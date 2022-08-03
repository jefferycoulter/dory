#version 450

layout(location = 0) in vec2 frag_offset;
layout(location = 0) out vec4 FragColor;

layout(set = 0, binding = 0) uniform UBO
{
    mat4 projection;
    mat4 view;
    vec4 ambient_color;
    vec4 light_position;
    vec4 light_color;
} ubo;

void main() {
    // make the point light circular
    float dist = sqrt(dot(frag_offset, frag_offset));
    if (dist >= 1.0) {
        discard;
    }

    FragColor = ubo.light_color;
}