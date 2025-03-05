#include "Heart.h"

#include "core/gpu/mesh.h"
#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2Dt.h"
#include <iostream>
#include "components/simple_scene.h"

Heart::Heart(glm::vec2 position) {
	circle = object2Dt::CreateCircle("circle", glm::vec3(0, 0, 0), 20, glm::vec3(1, 0, 0), true);
	circle2 = object2Dt::CreateCircle("circle2", glm::vec3(0, 0, 0), 24, glm::vec3(1, 0, 0), true);
	heart = object2Dt::CreateHeart("heart", glm::vec3(0, 0, 0), 0.75f, glm::vec3(1, 0, 0), true);
	heartOutline = object2Dt::CreateHeart("heartOutline", glm::vec3(0, 0, 0), 1.f, glm::vec3(1, 0, 0), true);
	this->position = position;
}


bool Heart::CheckCollision(glm::vec2 position, float radius) {
	float distance = glm::distance(this->position, position);
	if (distance < radius + 20) {
		return true;
	}
	return false;
}