#include "Tank.h"

#include "core/gpu/mesh.h"
#include <iostream>
#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2Dt.h"

#include "components/simple_scene.h"
//720

void Tank::init() {
	mesh1 = object2Dt::CreateTrapezoid("tank", glm::vec3(0, 0, 0), 60, 12, -5, glm::vec3(0, 1, 0), true);
	mesh2 = object2Dt::CreateTrapezoid("tank2", glm::vec3(0, 15, 0), 85, 20, 5, glm::vec3(0, 0, 1), true);
	mesh3 = object2Dt::CreateCircle("tank3", glm::vec3(0, 22, 0), 17, glm::vec3(1, 0, 0), true);
	gun = object2Dt::CreateRectangle("gun1", glm::vec3(gunCenter.x, gunCenter.y, 0), 40, 6, glm::vec3(0, 0, 0), true);
	trajectory = new Mesh("trajectory");
	hpBar = object2Dt::CreateRectangle("hpBar", glm::vec3(0, 75, 0), 60, 12, glm::vec3(0, 1, 0), true);
	reloadBar = object2Dt::CreateRectangle("reloadBar", glm::vec3(0, -15, 0), 50, 8, glm::vec3(1, 1, 1), true);
	ultraIndicator = object2Dt::CreateCircle("ultraIndicator", glm::vec3(0, 25, 0), 6, glm::vec3(1, 0, 0), true);
	position = glm::vec2(750, 18);
}

Tank::Tank(){
	init();
}


Tank::Tank(int id) {
	init();
	this->id = id;
}

void Tank::UpdateReloadTime(float deltaTimeSeconds) {
	reloadTime -= deltaTimeSeconds;
	if (reloadTime < 0) {
		reloadTime = 0;
	}
}

void Tank::SetColors(glm::vec3 lowerBodyColor, glm::vec3 upperBodyColor, glm::vec3 circleColor) {
	this->lowerBodyColor = lowerBodyColor;
	this->upperBodyColor = upperBodyColor;
	this->circleColor = circleColor;
}


bool Tank::CheckCollision(Bullet* bullet) {
	if (bullet->tankId == id || health <= 0) {
		return false;
	}

	float distance = glm::distance(position, bullet->position);

	if (distance < 45) {
		health--;
		return true;
	}
	return false;
}

void Tank::move(int direction, Map& map, float deltaTimeSeconds) {
	position.x += speed * deltaTimeSeconds * direction;
	int x = position.x;
	auto value = map.findClosestPoint(x);
	MapPoint prevValue = map.heightMap[value.first - map.step];

	glm::vec2 b = glm::vec2(value.first, value.second->height);
	glm::vec2 a = glm::vec2(value.first - map.step, prevValue.height);
	float t = (position.x - a.x) / (b.x - a.x);
	position.y = a.y + t * (b.y - a.y);

	targetAngle = atan2(b.y - a.y, b.x - a.x);
}

void Tank::rotate(float deltaTimeSeconds) {
	float t = deltaTimeSeconds * 20.f;
	currentAngle += t * (targetAngle - currentAngle);
}

void Tank::rotateTheGun(int direction, float deltaTimeSeconds) {
	gunAngle += direction * deltaTimeSeconds * 0.75f;
}

void Tank::calculateTrajectoryOfBullet() {
	trajectoryPoints.clear();
	glm::vec2 start = glm::vec2(0,  gunCenter.y);

	float angleRad = gunAngle;

	float v0x = bulletSpeed * cos(angleRad);
	float v0y = bulletSpeed * sin(angleRad);

	float y = start.y;
	float x = start.x;
	float step = 0.02;

	for (int i = 0; i < 200; ++i) {
		x = x + v0x * step;
		v0y -= gravity * step;
		y = y + v0y * step;
		trajectoryPoints.push_back(glm::vec2(x, y));
	}

	gunAngle2 = atan2(trajectoryPoints[5].y - start.y, trajectoryPoints[5].x - start.x);

	std::vector<VertexFormat> vertices;
	std::vector<unsigned int> indices;

	for (size_t i = 0; i < trajectoryPoints.size(); ++i) {
		vertices.emplace_back(glm::vec3(trajectoryPoints[i], 0.0f), glm::vec3(1, 0, 0));
		indices.push_back(i);
	}

	trajectory->InitFromData(vertices, indices);
	trajectory->SetDrawMode(GL_LINE_STRIP);
}

void Tank::ScaleTrajectory(float deltaTimeSeconds) {
	trajectoryScale += scaleTrajUp ? 8 * deltaTimeSeconds : -8 * deltaTimeSeconds;
	if (trajectoryScale > 6) {
		scaleTrajUp = false;
		trajectoryScale = 6;
	}
	else if (trajectoryScale < 3) {
		scaleTrajUp = true;
		trajectoryScale = 3;
	}
		
}

void Tank::createTripleBullet(std::vector<Bullet*>& bullets) {
	float gunLength = 40;
	float gunWidth = 6;
	bullets.push_back(new Bullet(bulletStartPosition, glm::vec2(bulletSpeed * cos(gunAngle), bulletSpeed * sin(gunAngle)), gunAngle, id));
	bullets.push_back(new Bullet(bulletStartPosition, glm::vec2(bulletSpeed * cos(gunAngle + 0.1f), bulletSpeed * sin(gunAngle + 0.1f)), gunAngle + 0.1f, id));
	bullets.push_back(new Bullet(bulletStartPosition, glm::vec2(bulletSpeed * cos(gunAngle - 0.1f), bulletSpeed * sin(gunAngle - 0.1f)), gunAngle - 0.1f, id));
}

Bullet* Tank::createBullet() {
	glm::vec2 velocity = glm::vec2(bulletSpeed * cos(gunAngle), bulletSpeed * sin(gunAngle));
	return new Bullet(bulletStartPosition, velocity, gunAngle, id);
}


Bullet::Bullet(glm::vec2 position, glm::vec2 speed, float angle, int id) {
	mesh = object2Dt::CreateCircle("bullet", glm::vec3(0, 0, 0), 5, glm::vec3(0, 0, 0), true);
	this->position = position;
	this->velocity = speed;
	this->tankId = id;
}

void Bullet::move(float deltaTimeSeconds) {
	float gravity = 1000.f;
	velocity.y -= gravity * deltaTimeSeconds;
	position.x = position.x + velocity.x * deltaTimeSeconds;
	position.y = position.y + velocity.y * deltaTimeSeconds;

}

glm::vec2 Bullet::SimulateNextStep(float deltaTimeSeconds) {
	float gravity = 1000.f;
	float x = position.x + velocity.x * deltaTimeSeconds;
	float y = position.y + (velocity.y - gravity * deltaTimeSeconds) * deltaTimeSeconds;
	return glm::vec2(x, y);
}