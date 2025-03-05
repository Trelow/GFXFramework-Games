#include "Packet.h"

#include "core/gpu/mesh.h"
#include <iostream>
#include "lab_m1/Tema2/transform3d.h"
#include "lab_m1/Tema2/object_3D.h"

#include "components/simple_scene.h"

Packet::Packet() {

	box = object_3D::CreateParallelipiped("box", glm::vec3(0, 0, 0), 0.4f, 0.2f, 0.4f, glm::vec3(0.9f, 0.7f, 0.5f), true);

	
}