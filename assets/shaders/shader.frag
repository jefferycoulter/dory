#version 450

layout(location = 0) in vec3 frag_color;

layout(location = 0) out vec4 FragColor;

layout(push_constant) uniform Push
{
    mat4 model_matrix;
    mat4 normal_matrix;
} push;

void main()
{
    FragColor = vec4(frag_color, 1.0);
}