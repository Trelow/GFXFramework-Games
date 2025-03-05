#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object_3D
{
	Mesh* CreateCube(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
	Mesh* CreateParallelipiped(const std::string& name, glm::vec3 leftBottomCorner, float length, float width, float height, glm::vec3 color, bool fill = false);
	Mesh* CreateCone(const std::string& name, glm::vec3 tip, float radius, float height, glm::vec3 color, bool fill = false);
	Mesh* CreateCylinder(const std::string& name, glm::vec3 center, float radius, float height, glm::vec3 color, bool fill = false);
    
}
