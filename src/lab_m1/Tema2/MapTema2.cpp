#include "MapTema2.h"

#include "core/gpu/mesh.h"
#include <iostream>
#include "lab_m1/Tema2/transform3d.h"
#include "lab_m1/Tema2/object_3D.h"

#include "components/simple_scene.h"

float fract(float x) {
    return x - std::floor(x);
}

float hash(float n) {
    return fract(std::sin(n) * 21.43f);
}

float mix(float x, float y, float a) {
    return x * (1.0f - a) + y * a;
}

float noise(const glm::vec2& st) {
    glm::vec2 i = glm::floor(st);
    glm::vec2 f = glm::vec2(fract(st.x), fract(st.y));

    float a = hash(i.x + i.y * 57.0);
    float b = hash(i.x + 1.0 + i.y * 57.0);
    float c = hash(i.x + (i.y + 1.0) * 57.0);
    float d = hash(i.x + 1.0 + (i.y + 1.0) * 57.0);

    glm::vec2 u = f * f * (3.0f - 2.0f * f);
    return mix(a, b, u.x) +
        (c - a) * u.y * (1.0 - u.x) +
        (d - b) * u.x * u.y;
}


MapTema2::MapTema2(){
    plane = CreateGrid("grid", 100, 100);
}

Mesh* MapTema2::CreateGrid(const std::string& name, int m, int n) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i <= n; ++i) {
        for (int j = 0; j <= m; ++j) {
            float x = (float)j / m * 100 - 50; 
            float z = (float)i / n * 100 - 50; 
            glm::vec3 position = glm::vec3(x, 0, z);
            glm::vec3 color = glm::vec3(0.1f, 0.7f, 0.3f); 
            vertices.push_back(VertexFormat(position, color));
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            int topLeft = i * (m + 1) + j;
            int topRight = topLeft + 1;
            int bottomLeft = topLeft + m + 1;
            int bottomRight = bottomLeft + 1;

            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    Mesh* grid = new Mesh(name);
    grid->InitFromData(vertices, indices);
    return grid;
}

float MapTema2:: getTerrainHeight(float x, float z) {
    float scale = 0.1f;
    glm::vec2 pos = glm::vec2(x, z) * scale;
    float noiseValue = noise(pos);
    return noiseValue * 4.0f - 1.0f;
}