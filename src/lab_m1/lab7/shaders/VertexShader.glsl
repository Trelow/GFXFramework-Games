#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;

// Output value to fragment shader
out vec3 color;

void main()
{
	vec3 world_position = (Model * vec4(v_position, 1)).xyz;
	vec3 world_normal	= normalize(mat3(Model) * normalize(v_normal));

	vec3 V = normalize(eye_position - world_position);
	vec3 L = normalize(light_position - world_position);
	vec3 H = normalize(L + V);
	vec3 R = normalize(reflect(-L, world_normal));

	float ambient_light = 0.2;

	float diffuse_light = material_kd * max(dot(world_normal, L), 0);

	float specular_light = 0;
	if (diffuse_light > 0)
	{
		specular_light = material_ks * pow(max(dot(V, R), 0), material_shininess);
	}

	float dis = distance(light_position, world_position);
	float att_factor = 1 / (1 + 0.3 * dis + 0.2 * dis * dis);;
	float light	= ambient_light + att_factor * (diffuse_light + specular_light);

	// Send color light output to fragment shader
	color = object_color * light;

	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}