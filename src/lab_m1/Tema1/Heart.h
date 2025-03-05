#pragma once

#include "core/gpu/mesh.h"
#include "Map.h"

class Heart {


public:
	Mesh* circle;
	Mesh* circle2;
	Mesh* heart;
	Mesh* heartOutline;
	glm::vec2 position;
	float scaling = 1.f;
	bool scaleUp = true;
	float disappearTime = 6.f;
	Heart(glm::vec2 position);

	bool CheckCollision(glm::vec2 position, float radius);

};