#include "CheckPoint.h"

#include "core/gpu/mesh.h"
#include <iostream>
#include "lab_m1/Tema2/transform3d.h"
#include "lab_m1/Tema2/object_3D.h"

#include "components/simple_scene.h"

CheckPoint::CheckPoint() {

	cylinder = object_3D::CreateCylinder("cylinder", glm::vec3(0, 0, 0), 2.f, 1000.f, glm::vec3(0.3f, 0.7f, 0.9f), true);
}