#include "Cloud.h"

#include "core/gpu/mesh.h"
#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2Dt.h"
#include <iostream>
#include "components/simple_scene.h"


Cloud::Cloud(glm::vec2 position, float speed) {
	circle1 = object2Dt::CreateCircle("circle1", glm::vec3(0, 0, 0), 50, glm::vec3(1, 1, 1), true);
	circle2 = object2Dt::CreateCircle("circle2", glm::vec3(100, 0, 0), 50, glm::vec3(1, 1, 1), true);
	circle3 = object2Dt::CreateCircle("circle3", glm::vec3(50, 10, 0), 50, glm::vec3(1, 1, 1), true);
	this->position = position;		
	this->cloudSpeed = speed;
}

