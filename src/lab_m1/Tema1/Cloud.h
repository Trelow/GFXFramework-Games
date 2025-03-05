#pragma once

#include "core/gpu/mesh.h"
#include "Map.h"

class Cloud {


public:
	Mesh* circle1;
	Mesh* circle2;
	Mesh* circle3;
	float cloudSpeed = 50.f;
	glm::vec2 position;
	Cloud(glm::vec2 position, float speed);

};