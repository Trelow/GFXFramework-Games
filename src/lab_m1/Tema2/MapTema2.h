#pragma once

#include "core/gpu/mesh.h"
#include "lab_m1/Tema2/Tree.h"

#include <vector>

class MapTema2
{

public:
	Mesh* plane;
	std::vector<Tree*> trees;
	std::vector<Mesh*> grass;
	std::vector<glm::vec3> grassPositions;


	MapTema2();
	float getTerrainHeight(float droneX, float droneZ);

private:
	Mesh* CreateGrid(const std::string& name, int m, int n);

};