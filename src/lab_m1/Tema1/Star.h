#pragma once

#include "core/gpu/mesh.h"
#include "Map.h"

class Star {


public:
	Mesh* star;

	glm::vec2 position;
	float* groundHeight;
	Star(glm::vec2 position, float* groundHeight);
	void Falling(float deltaTime);
	float angle = 0;
	float liveTime = 15.f;

};