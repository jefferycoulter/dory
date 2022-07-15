#version 450

layout(location = 0) in vec2 a_position;

layout(push_constant) uniform Push
{
    mat2 transform;
    vec2 offset;
    vec3 color;
} push;

void main() {
    gl_Position = vec4(push.transform * a_position + push.offset, 0.0, 1.0);
}