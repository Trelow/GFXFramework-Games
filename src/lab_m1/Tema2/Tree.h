#pragma once

#include "core/gpu/mesh.h"

class Tree
{
public:
	Mesh* cone1 = nullptr, *cone2 = nullptr;
	Mesh* cilinder = nullptr;
	
	glm::vec3 position = glm::vec3(0, 0, 0);

	Tree();

	float cylinderRadiusCollider = 0.0f;
	float cylinderHeightCollider = 0.0f;
	float yOffsetCollider = 0.0f;

	float crownRadiusCollider = 0.0f;
	float crownHeightCollider = 0.0f;
	float crownYOffsetCollider = 0.0f;




};