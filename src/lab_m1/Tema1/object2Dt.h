#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2Dt
{
	Mesh* CreateSquare(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
	Mesh* CreateRectangle(const std::string& name, glm::vec3 leftBottomCorner, float length, float width, glm::vec3 color, bool fill);
	Mesh* CreateCircle(const std::string& name, glm::vec3 leftBottomCorner, float radius, glm::vec3 color, bool fill = false);
	Mesh* CreateTrapezoid(const std::string& name, glm::vec3 leftBottomCorner, float length, float width, float offset, glm::vec3 color, bool fill = false);
	Mesh* CreateHeart(const std::string& name, glm::vec3 leftBottomCorner, float size, glm::vec3 color, bool fill);
	Mesh* CreateStar(const std::string& name, glm::vec3 leftBottomCorner,float outerRad,float innerRad, glm::vec3 color);
}
