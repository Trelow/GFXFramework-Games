#include "Star.h"

#include "core/gpu/mesh.h"
#include <iostream>
#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2Dt.h"
#include "components/simple_scene.h"

Star::Star(glm::vec2 position, float* groundHeight) {
	star = object2Dt::CreateStar("star", glm::vec3(0, 0, 0), 15, 10, glm::vec3(1, 1, 1));
	this->groundHeight = groundHeight;
	this->position = position;
}

void Star::Falling(float deltaTime) {
	
	if (this->position.y > *(groundHeight) + 15) {
		this->position.y -= 100 * deltaTime;
		this->angle -= deltaTime * 3;
	}
}