#pragma once

#include "core/gpu/mesh.h"

class Packet
{
public:
	Mesh* box;

	glm::vec3 position = glm::vec3(0, 0, 0);

	float boxCollieder = 0.3f;

	bool isCollected = false;

	Packet();

	



};