#version 330 core

layout(location = 0) in vec3 aPos; // Position attribute
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out float height;

float hash(float n) {
    return fract(sin(n) * 21.43);
}

float noise(vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    // Four corners in 2D of a tile
    float a = hash(i.x + i.y * 57.0);
    float b = hash(i.x + 1.0 + i.y * 57.0);
    float c = hash(i.x + (i.y + 1.0) * 57.0);
    float d = hash(i.x + 1.0 + (i.y + 1.0) * 57.0);

    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(a, b, u.x) +
           (c - a) * u.y * (1.0 - u.x) +
           (d - b) * u.x * u.y;
}


void main() {
    float noiseValue = noise(aPos.xz * 0.1);
    float y = noiseValue * 4.0 - 1.0;

    // Correctly set up the position vector
    vec3 pos = vec3(aPos.x, y, aPos.z);
    // Set height
    height = y;

    //gl_Position calculation
    gl_Position = projection * view * model * vec4(pos, 1.0);



}
