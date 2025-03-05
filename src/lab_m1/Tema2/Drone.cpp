#include "Drone.h"

#include "core/gpu/mesh.h"
#include <iostream>
#include "lab_m1/Tema2/transform3d.h"
#include "lab_m1/Tema2/object_3D.h"

#include "components/simple_scene.h"

Drone::Drone()
{
	parallelipiped1 = object_3D::CreateParallelipiped("parallelipiped1", glm::vec3(-0.45, -0.025, -0.05), 0.9, 0.05, 0.075, glm::vec3(0.6, 0.6, 0.6), true);
	parallelipiped2 = object_3D::CreateParallelipiped("parallelipiped2", glm::vec3(-0.45, -0.025, -0.05), 0.9, 0.05, 0.075, glm::vec3(0.6, 0.6, 0.6), true);

	cube1 = object_3D::CreateCube("cube1", glm::vec3(-0.45, 0, -0.05), 0.075, glm::vec3(0.6, 0.6, 0.6), true);
	cube2 = object_3D::CreateCube("cube2", glm::vec3(-0.45, 0, -0.05), 0.075, glm::vec3(0.6, 0.6, 0.6), true);
	cube3 = object_3D::CreateCube("cube3", glm::vec3(0.375, 0.0, -0.05), 0.075, glm::vec3(0.6, 0.6, 0.6), true);
	cube4 = object_3D::CreateCube("cube4", glm::vec3(0.375, 0.0, -0.05), 0.075, glm::vec3(0.6, 0.6, 0.6), true);

	propeller1 = object_3D::CreateParallelipiped("propeller1", glm::vec3(-0.1,- 0.005, -0.015), 0.2, 0.01, 0.03, glm::vec3(0, 0, 0), true);
	propeller2 = object_3D::CreateParallelipiped("propeller2", glm::vec3(-0.1, -0.005, -0.015), 0.2, 0.01, 0.03, glm::vec3(0, 0, 0), true);
	propeller3 = object_3D::CreateParallelipiped("propeller3", glm::vec3(-0.1, -0.005, -0.015), 0.2, 0.01, 0.03, glm::vec3(0, 0, 0), true);
	propeller4 = object_3D::CreateParallelipiped("propeller4", glm::vec3(-0.1, -0.005, -0.015), 0.2, 0.01, 0.03, glm::vec3(0, 0, 0), true);

	forward = glm::vec3(0, 0, 1);


}