#pragma once

#include "core/gpu/mesh.h"

class Drone
{
public:
	glm::vec3 dronePosition = glm::vec3(0, 0, 0);
	Mesh * parallelipiped1 = nullptr, *parallelipiped2 = nullptr;
	float parallelipipedAngle = RADIANS(45);

	Mesh* cube1, * cube2, * cube3, * cube4;
	Mesh* propeller1, * propeller2, * propeller3, * propeller4;
	float propellerAngle = 0;

	float rotationOZ = 0;
	float rotationOX = 0;
	float rotationOY = 0;

	glm::vec3 velocity = glm::vec3(0, 0, 0);
	glm::vec3 forward = glm::vec3(0, 0, 0);
	bool isThrusting = false;

	float propellerSpeed = 0;

	Drone();





};