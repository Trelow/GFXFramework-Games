#include "lab_m1/Tema2/object_3D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

Mesh* object_3D::CreateCube(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices = {
		VertexFormat(corner + glm::vec3(0, 0, length), color),
		VertexFormat(corner + glm::vec3(length, 0, length), color),
		VertexFormat(corner + glm::vec3(0, length, length), color),
		VertexFormat(corner + glm::vec3(length, length, length), color),
		VertexFormat(corner + glm::vec3(0, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color)

    };


    Mesh* cube = new Mesh(name);
    std::vector<unsigned int> indices;
    if (!fill) {
        cube->SetDrawMode(GL_LINE_LOOP);
		indices = {
			0, 1, 3, 2,
			0, 1, 5, 4,
			1, 3, 7, 5,
			2, 3, 7, 6,
			0, 2, 6, 4,
			4, 5, 7, 6
		};
    }
    else {
		indices = {
			0, 1, 2,
			1, 3, 2,
			2, 3, 7,
			2, 7, 6,
			1, 7 , 3,
			1, 5, 7,
			6, 7, 4,
			7, 5, 4,
			0, 4 ,1,
			1, 4, 5,
			2, 6, 4,
			0, 2, 4
		};
      
    }

    cube->InitFromData(vertices, indices);
    return cube;
}

Mesh* object_3D::CreateParallelipiped(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    float width,
    float height,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices = {
        VertexFormat(corner + glm::vec3(0, 0, height), color), 
        VertexFormat(corner + glm::vec3(length, 0, height), color),  
        VertexFormat(corner + glm::vec3(0, width, height), color),  
        VertexFormat(corner + glm::vec3(length, width, height), color), 
        VertexFormat(corner + glm::vec3(0, 0, 0), color),    
        VertexFormat(corner + glm::vec3(length, 0, 0), color),  
        VertexFormat(corner + glm::vec3(0, width, 0), color),
        VertexFormat(corner + glm::vec3(length, width, 0), color) 
    };

    std::vector<unsigned int> indices;
    if (fill) {
        indices = {
            0, 1, 2, 1, 3, 2,
            4, 6, 5, 6, 7, 5,
            0, 1, 4, 1, 5, 4,
            2, 6, 3, 3, 6, 7,
            0, 2, 4, 2, 6, 4,
            1, 3, 5, 3, 7, 5
        };
    }
    else {
        indices = {
            0, 1, 1, 3, 3, 2, 2, 0,  
            4, 5, 5, 7, 7, 6, 6, 4,  
            0, 4, 1, 5, 2, 6, 3, 7   
        };
    }

    Mesh* parallelepiped = new Mesh(name);
    parallelepiped->SetDrawMode(fill ? GL_TRIANGLES : GL_LINE_LOOP);
    parallelepiped->InitFromData(vertices, indices);
    return parallelepiped;
}

Mesh* object_3D::CreateCylinder(const std::string& name, glm::vec3 center, float radius, float height, glm::vec3 color, bool fill) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    int sectors = 36;
    float sectorAngle = 2 * M_PI / sectors;

    vertices.push_back(VertexFormat(center + glm::vec3(0, height / 2, 0), color));
    int topCenterIndex = 0;

    vertices.push_back(VertexFormat(center - glm::vec3(0, height / 2, 0), color));
    int bottomCenterIndex = 1;

    for (int i = 0; i < sectors; ++i) {
        float angle = i * sectorAngle;
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        glm::vec3 unitCirclePos(x, 0, z);

        vertices.push_back(VertexFormat(center + glm::vec3(x, height / 2, z), color));
        vertices.push_back(VertexFormat(center + glm::vec3(x, -height / 2, z), color));
    }

    for (int i = 2; i < sectors * 2; i += 2) {
        indices.push_back(topCenterIndex);
        indices.push_back(i);
        indices.push_back(i + 2);
    }
    indices.push_back(topCenterIndex);
    indices.push_back(sectors * 2);
    indices.push_back(2);

    for (int i = 3; i < sectors * 2; i += 2) {
        indices.push_back(bottomCenterIndex);
        indices.push_back(i + 2);
        indices.push_back(i);
    }
    indices.push_back(bottomCenterIndex);
    indices.push_back(3);
    indices.push_back(sectors * 2 + 1);

    for (int i = 2; i < sectors * 2; i += 2) {
        indices.push_back(i);
        indices.push_back(i + 1);
        indices.push_back(i + 2);

        indices.push_back(i + 1);
        indices.push_back(i + 3);
        indices.push_back(i + 2);
    }
    indices.push_back(sectors * 2);
    indices.push_back(sectors * 2 + 1);
    indices.push_back(2);

    indices.push_back(sectors * 2 + 1);
    indices.push_back(3);
    indices.push_back(2);

    Mesh* cylinder = new Mesh(name);
    cylinder->SetDrawMode(GL_TRIANGLES);
    cylinder->InitFromData(vertices, indices);
    return cylinder;
}



Mesh* object_3D::CreateCone(const std::string& name, glm::vec3 tip, float radius, float height, glm::vec3 color, bool fill) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    int sectors = 36;
    float sectorAngle = 2 * M_PI / sectors;

    vertices.push_back(VertexFormat(tip, color));
    int tipIndex = 0;

    glm::vec3 baseCenter = tip - glm::vec3(0, height, 0);
    vertices.push_back(VertexFormat(baseCenter, color));
    int baseCenterIndex = 1;

    for (int i = 0; i < sectors; ++i) {
        float angle = i * sectorAngle;
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        vertices.push_back(VertexFormat(baseCenter + glm::vec3(x, 0, z), color));
    }

    for (int i = 2; i < sectors + 1; ++i) {
        indices.push_back(baseCenterIndex);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    indices.push_back(baseCenterIndex);
    indices.push_back(sectors + 1);
    indices.push_back(2);

    for (int i = 2; i < sectors + 1; ++i) {
        indices.push_back(tipIndex);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    indices.push_back(tipIndex);
    indices.push_back(sectors + 1);
    indices.push_back(2);

    Mesh* cone = new Mesh(name);
    cone->SetDrawMode(GL_TRIANGLES);
    cone->InitFromData(vertices, indices);
    return cone;
}
