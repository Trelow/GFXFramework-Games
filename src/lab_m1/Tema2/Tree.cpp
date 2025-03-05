#include "Tree.h"

#include "core/gpu/mesh.h"
#include <iostream>
#include "lab_m1/Tema2/transform3d.h"
#include "lab_m1/Tema2/object_3D.h"

#include "components/simple_scene.h"

Tree::Tree() {

	int scale = rand() % 200;
	float scaleValue = 1.5f + (scale / 100.0f);

	float height = 1.0f * scaleValue;
	float radius = 0.6f * scaleValue;

	float offset = 0.4f * scaleValue;



	float r = 0.35f + (rand() % 10) / 100.0f;

	float g = 0.60f + (rand() % 10) / 100.0f;

	float b = 0.15f + (rand() % 10) / 100.0f;



	cone1 = object_3D::CreateCone("cone1", glm::vec3(0, offset + height * 1.4f, 0), radius, height, glm::vec3(r, g, b), true);
	cone2 = object_3D::CreateCone("cone2", glm::vec3(0,  height + offset, 0), radius, height, glm::vec3(r, g, b), true);


	cilinder = object_3D::CreateCylinder("cilinder", glm::vec3(0, height / 2, 0), radius / 4.f, height, glm::vec3(0.45, 0.3, 0.05), true);

	cylinderHeightCollider = height;
	cylinderRadiusCollider = radius / 4.f;
	yOffsetCollider = 0;


	crownHeightCollider = height * 0.85;
	crownRadiusCollider = radius * 0.85;
	crownYOffsetCollider = offset;




}