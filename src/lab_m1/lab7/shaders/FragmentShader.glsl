#version 330

// Get color value from vertex shader
in vec3 color;

layout(location = 0) out vec4 out_color;

void main()
{
	// Write pixel out color
	out_color = vec4(color, 1);
}