#version 330 core

layout(location = 0) in vec3 v_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 in_color;

out vec4 color;

void main() {
    gl_Position = projection * view * model * vec4(v_position, 1.0);
    color = in_color;
}
