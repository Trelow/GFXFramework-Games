#pragma once
#include "components/simple_scene.h"
#include <map>

class MapPoint
{
public:
    MapPoint(Mesh* mesh, float y) {
        this->mesh = mesh;
        this->height = y;
    }
    MapPoint() {
        this->mesh = nullptr;
        this->height = 0;
    }
    Mesh* mesh;
    float height;
};

class Map {
public:
    int step = 1;
    std::map<int, MapPoint> heightMap;

    std::pair<int, MapPoint*> findClosestPoint(int x) {
		if (heightMap.empty()) return  { -1, nullptr };
        auto it = heightMap.lower_bound(x);
		if (it == heightMap.end()) return { std::prev(it)->first, &std::prev(it)->second };
        return { it->first, &it->second };
    }
};
