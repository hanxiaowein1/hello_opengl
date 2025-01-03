# version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_texture;

out vec3 texture;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(a_pos, 1.0f);
    texture = a_texture;
}