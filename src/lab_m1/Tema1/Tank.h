#pragma once

#include "core/gpu/mesh.h"
#include "Map.h"

class Bullet {
public:
	Mesh* mesh;
	glm::vec2 position;
	glm::vec2 velocity;
	bool isAlive = true;
	int tankId = 0;

	Bullet(glm::vec2 position, glm::vec2 speed, float angle, int id);
	void move(float deltaTimeSeconds);
	glm::vec2 SimulateNextStep(float deltaTimeSeconds);
};


class Tank {
public:
	Mesh* mesh1 = nullptr, *mesh2 = nullptr, *mesh3 = nullptr, *hpBar = nullptr, *gun = nullptr, *trajectory = nullptr,
		*bullet = nullptr, *collisionCircle = nullptr, *reloadBar = nullptr, *ultraIndicator = nullptr;
	glm::vec2 position;
	glm::vec2 gunCenter = glm::vec2(20, 28);
	glm::vec2 bulletStartPosition = glm::vec2(0, 0);
	glm::vec3 lowerBodyColor = glm::vec3(0.1f, 0.1f, 0.1f);
	glm::vec3 upperBodyColor = glm::vec3(0.1f, 0.1f, 0.1f);
	glm::vec3 circleColor = glm::vec3(0.1f, 0.1f, 0.1f);
	std::vector<glm::vec2> trajectoryPoints;
	int health = 5;
	int id = 1;
	float currentAngle = 0, targetAngle = 0, gunAngle = 20.f, gunAngle2 = 0, speed = 125.f, reloadTime = 0.f, trajectoryScale = 1.f, bulletSpeed = 650.f;
	float gravity = 1000.f;
	bool scaleTrajUp = true;
	bool utraReady = false;

	Tank();
    Tank(int id);

	void init();
    void move(int direction, Map& map, float deltaTimeSeconds);
	void rotateTheGun(int direction, float deltaTimeSeconds);
    void rotate(float deltaTimeSeconds);
	void calculateTrajectoryOfBullet();
	Bullet* createBullet();
	void createTripleBullet(std::vector<Bullet*>& bullets);
	bool CheckCollision(Bullet* bullet);
	void SetColors(glm::vec3 lowerBodyColor, glm::vec3 upperBodyColor, glm::vec3 circleColor);
	void UpdateReloadTime(float deltaTimeSeconds);
	void ScaleTrajectory(float deltaTimeSeconds);
};
