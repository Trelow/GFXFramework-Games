#version 330 core

in float height;
out vec4 FragColor;

void main() {
    vec3 color = mix(vec3(0.3, 0.45, 0.1), vec3(0.42 ,0.5, 0.05), (height + 1.0) * 0.5);
    FragColor = vec4(color, 1.0);
}
