#version 450

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_color;

layout(location = 0) out vec3 frag_color;

layout(push_constant) uniform Push
{
    mat4 transform;
    vec3 color;
} push;

void main() {
    // gl_Position = vec4(push.transform * a_position + push.offset, 0.0, 1.0);
    gl_Position = push.transform * vec4(a_position, 1.0);
    frag_color = a_color;
}