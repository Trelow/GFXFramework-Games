#include "lab_m1/Tema1/object2Dt.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2Dt::CreateSquare(
	const std::string& name,
	glm::vec3 leftBottomCorner,
	float length,
	glm::vec3 color,
	bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned int> indices = { 0, 1, 2, 3 };

	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* object2Dt::CreateRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    float width,
    glm::vec3 color,
    bool fill)
{
    float halfLength = length / 2.0f;
    float halfWidth = width / 2.0f;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(leftBottomCorner + glm::vec3(-halfLength, -halfWidth, 0), color),
        VertexFormat(leftBottomCorner + glm::vec3(halfLength, -halfWidth, 0), color),
        VertexFormat(leftBottomCorner + glm::vec3(halfLength, halfWidth, 0), color),
        VertexFormat(leftBottomCorner + glm::vec3(-halfLength, halfWidth, 0), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices = { 0, 1, 2,
                    0, 2, 3 };
    }
    rectangle->InitFromData(vertices, indices);
    return rectangle;
}

Mesh* object2Dt::CreateTrapezoid(
	const std::string& name,
	glm::vec3 leftBottomCorner,
	float length,
	float width,
	float offset,
	glm::vec3 color,
	bool fill)
{
	float halfLength = length / 2.0f;
	float halfWidth = width / 2.0f;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(leftBottomCorner + glm::vec3(-halfLength, -halfWidth, 0), color),
		VertexFormat(leftBottomCorner + glm::vec3(halfLength, -halfWidth, 0), color),
		VertexFormat(leftBottomCorner + glm::vec3(halfLength - offset, halfWidth, 0), color),
		VertexFormat(leftBottomCorner + glm::vec3(-halfLength + offset, halfWidth, 0), color)
	};

	Mesh* trapezoid = new Mesh(name);
	std::vector<unsigned int> indices = { 0, 1, 2, 3 };

	if (!fill) {
		trapezoid->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		indices = { 0, 1, 2,
					0, 2, 3 };
	}

	trapezoid->InitFromData(vertices, indices);
	return trapezoid;
}

Mesh* object2Dt::CreateCircle(
	const std::string& name,
	glm::vec3 leftBottomCorner,
	float radius,
	glm::vec3 color,
	bool fill)
{
	int segments = 50;
	float step = 2 * M_PI / segments;

	std::vector<unsigned int> indices;
	std::vector<VertexFormat> vertices;

	for (int i = 0; i < segments; i++) {
		indices.push_back(i);
		float angle = i * step;
		glm::vec3 v = leftBottomCorner + glm::vec3(radius * cos(angle), radius * sin(angle), 0);
		vertices.push_back(VertexFormat(v, color));
	}

	Mesh* circle = new Mesh(name);
	circle->InitFromData(vertices, indices);
	if (fill) {
		circle->SetDrawMode(GL_TRIANGLE_FAN);
	}
	else {
		circle->SetDrawMode(GL_LINE_LOOP);
	}

	return circle;
}


Mesh* object2Dt::CreateHeart(const std::string& name, glm::vec3 leftBottomCorner, float size, glm::vec3 color, bool fill) {
	std::vector<VertexFormat> vertices;
	std::vector<unsigned int> indices;

	int segments = 75;
	float step = 2 * M_PI / segments;

	for (int i = 0; i <= segments; i++) {
		float t = i * step - M_PI;
		glm::vec3 v = leftBottomCorner + glm::vec3(size * 15 * pow(sin(t), 3), size * (12 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cos(4 * t)), 0);
		vertices.push_back(VertexFormat(v, color));
	}

	if (fill) {
		for (int i = 1; i <= segments; i++) {
			indices.push_back(0);
			indices.push_back(i);
			indices.push_back(i % segments + 1);
		}
	}
	else {
		for (int i = 0; i <= segments; ++i) {
			indices.push_back(i);
		}
	}
	Mesh* heart = new Mesh(name);
	heart->InitFromData(vertices, indices);
	heart->SetDrawMode(fill ? GL_TRIANGLES : GL_LINE_LOOP);
	return heart;
}

Mesh* object2Dt::CreateStar(
	const std::string& name,
	glm::vec3 leftBottomCorner,
	float outerRad,
	float innerRad,
	glm::vec3 color)
{
	std::vector<VertexFormat> vertices;
	std::vector<unsigned int> indices;

	float step = 2 * M_PI / 10;

	for (int i = 0; i < 10; i++) {
		float rad;
		if (i % 2 == 0) {
			rad = outerRad;
		}
		else {
			rad = innerRad;
		}
		float angle = i * step;
		glm::vec3 vertex = leftBottomCorner + glm::vec3(rad * cos(angle), rad * sin(angle), 0);
		vertices.push_back(VertexFormat(vertex, color));
	}

	Mesh* star = new Mesh(name);
	vertices.push_back(VertexFormat(leftBottomCorner, color));
	int center = vertices.size() - 1;

	for (int i = 0; i < 10; i++) {
		indices.push_back(i);
		indices.push_back((i + 1) % 10);
		indices.push_back(center);
	}
	star->SetDrawMode(GL_TRIANGLES);


	star->InitFromData(vertices, indices);
	return star;
}
