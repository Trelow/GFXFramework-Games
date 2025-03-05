#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;

out vec3 w_p;
out vec3 w_n;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;


void main()
{
    w_p = (Model * vec4(v_position, 1)).xyz;
	w_n = normalize( mat3(Model) * normalize(v_normal) );

    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
