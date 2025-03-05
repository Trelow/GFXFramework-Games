#version 330

in vec3 w_p;
in vec3 w_n;

// Uniforms for light properties

uniform vec3 new_light_position;
uniform vec3 new_light_color;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform vec3 object_color;
uniform int spot;

// Output
layout(location = 0) out vec4 out_color;


void main()
{

    vec3 V = normalize(eye_position - w_p);
	vec3 L = normalize(light_position - w_p);
	vec3 H = normalize(L + V);
	vec3 R = normalize(reflect(-L, w_n));

    float ambient_light = 0.25 * material_kd;
    float diffuse_light = material_kd * max(dot(L, w_n), 0);;
    float specular_light = 0;


    if (diffuse_light > 0)
    {
        specular_light = material_ks * pow(max(dot(V, R), 0), material_shininess);
    }


    float cut_off = radians(30.0f);
    float spot_light = dot(-L, light_direction);
    float spot_light_limit = cos(cut_off);
 
    // Quadratic attenuation
    float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
    float light_att_factor = pow(linear_att, 2);
    float dist = distance(light_position, w_p);
    float atten = 1 / (dist * dist);

    // TODO(student): Compute the total light. You can just add the components
    // together, but if you're feeling extra fancy, you can add individual
    // colors to the light components. To do that, pick some vec3 colors that
    // you like, and multiply them with the respective light components.
    vec3 light;
    if (spot == 0) {
		light = object_color * (ambient_light + atten * (diffuse_light + specular_light));
	}
    else{
    if (spot_light > cos(cut_off))
	{
		light = object_color * (ambient_light + (light_att_factor + atten) * (diffuse_light + specular_light));
	} 
    else 
    {
		light = object_color * ambient_light;
	}
    }

    vec3 L2 = normalize(new_light_position - w_p);
    vec3 R2 = normalize(reflect(-L2, w_n));
    float diffuse_light2 = material_kd * max(dot(L2, w_n), 0);
    float specular_light2 = 0;
    if (diffuse_light2 > 0) {
        specular_light2 = material_ks * pow(max(dot(V, R2), 0), material_shininess);
    }

    float dist2 = distance(new_light_position, w_p);
    float atten2 = 1 / (dist2 * dist2);

    vec3 new_light = new_light_color * atten2 * (diffuse_light2 + specular_light2);

    vec3 total_light = object_color * (ambient_light + light) + new_light;

    // TODO(student): Write pixel out color
    out_color = vec4(total_light, 1);

}
