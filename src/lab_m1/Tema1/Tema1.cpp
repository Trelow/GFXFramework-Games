#include "lab_m1/Tema1/Tema1.h"
#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2Dt.h"

#include <vector>
#include <iostream>

#include "stb/stb_image.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


float Tema1::MapFunction(int t) {
	return A1 * sin(O1 * t) + A2 * sin(O2 * t);
}

void Tema1::Init()
{
	srand(time(NULL)); // Seed the random number generator

	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(100, (float)resolution.x + 100 , 0, (float)resolution.y , 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);
	initialRes = window->GetResolution();

	map.step = 5;
	GenerateMap(resolution);
	GenerateClouds();


	tank1 = Tank(1);
	tank2 = Tank(2);
	tank1.position = glm::vec2(200, 18);

	tank1.SetColors(glm::vec3(0.67, 0.72, 0.48), glm::vec3(0.23, 0.32, 0), glm::vec3(0.23, 0.32, 0));
	tank2.position = glm::vec2(1240, 18);
	tank1.calculateTrajectoryOfBullet();
	tank2.calculateTrajectoryOfBullet();
	tank2.SetColors(glm::vec3(0.23, 0.32, 0), glm::vec3(0.67, 0.72, 0.48), glm::vec3(0.67, 0.72, 0.48));

}

void Tema1::GenerateMap(glm::ivec2& resolution) {
	for (int x = 0; x < resolution.x * 2; x += map.step) {
		float y = MapFunction(x);
		Mesh* mesh = object2Dt::CreateSquare("mapSqaure" + to_string(x), glm::vec3(0, 0, 0), 1, glm::vec3(1, 0, 0), true);
		map.heightMap.insert({ x , MapPoint(mesh, y + 200) });
		AddMeshToList(mesh);
	}

}

void Tema1::GenerateClouds() {
	for (int i = 0; i < 4; i++) {
		float x = -200 + i * 450;
		float y = 650 - (rand() % 150);
		float speed = 10;
		Cloud* cloud = new Cloud(glm::vec2(x, y), speed);
		clouds.push_back(cloud);
	}
}

void Tema1::MoveClouds(float deltaTime) {
	for (int i = 0; i < clouds.size(); i++) {
		Cloud* cloud = clouds[i];
		cloud->position.x += cloud->cloudSpeed * deltaTime;
		if (cloud->position.x > (clouds.size() - 1) * 450 + 225) {
			cloud->position.x = -200;
			cloud->position.y = 650 - (rand() % 150);
		}

	}
}

void Tema1::FrameStart()
{
	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0.63, 0.82, 0.87, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// Sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);

}

void Tema1::ExplodeTerrain(std::pair<int, MapPoint*>& value) {
	float radius = 40;
	float maxDepth = 40;
	float center = map.heightMap[value.first].height;

	int start = std::max(0.f, value.first - radius);
	int end = std::min((float)initialRes.x + 150, value.first + radius);

	for (int i = start; i < end; i += map.step) {
		float depth = maxDepth * sqrt(1 - pow(abs(i - value.first) / radius, 2));
		float newHeight = center - depth;
		if (map.heightMap[i].height > newHeight) {
			map.heightMap[i].height = newHeight;
		}
	}
}

void Tema1::DetectBulletCollisions(float deltaTime) {
	for (int i = 0; i < bullets.size(); i++) {
		Bullet* bullet = NULL;
		bullet = bullets[i];
		if (bullet->isAlive) {
			auto value = map.findClosestPoint(bullet->position.x);

			glm::vec2 P2 = glm::vec2(value.first, value.second->height);
			glm::vec2 P1 = glm::vec2(value.first - map.step, map.heightMap[value.first - map.step].height);
			float t = (bullet->position.x - P1.x) / (P2.x - P1.x);
			glm::vec2 I = P1 * (1 - t) + P2 * t;

			// Interpolate next position
			glm::vec2 nextPos = bullet->SimulateNextStep(deltaTime);
			float distance = glm::distance(I, bullet->position);
			glm::vec2 def2 = I - nextPos;
			bool passedThrough = false;
			if (def2.y >= 0) {
				passedThrough = true;
			}
			else {
				passedThrough = false;
			}

			// Deform the terrain
			if (distance < 5 || passedThrough) {
				bullet->isAlive = false;
				bullets.erase(bullets.begin() + i);
				delete bullet;
				i--;
				ExplodeTerrain(value);
			}

			// Check collision for each tank
			bool collision = tank1.CheckCollision(bullet) || tank2.CheckCollision(bullet);
			if (collision) {
				bullet->isAlive = false;
				bullets.erase(bullets.begin() + i);
				delete bullet;
				i--;
				continue;

			}

			// Check collision for each heart
			for (int j = 0; j < hearts.size(); j++) {
				Heart* heart = hearts[j];
				collision = heart->CheckCollision(bullet->position, 5);

				if (collision) {
					hearts.erase(hearts.begin() + j);
					delete heart;
					if (bullet->tankId == 1) {
						if (tank1.health < 5)
							tank1.health++;
					}
					else {
						if (tank2.health < 5)
							tank2.health++;
					}
					break;
				}

			}

			// Check collision for each star
			for (int j = 0; j < stars.size(); j++) {
				Star* star = stars[j];
				bool collision = false;
				float distance = glm::distance(star->position, bullet->position);
				if (distance < 15) {
					collision = true;
				}
				if (collision) {
					stars.erase(stars.begin() + j);
					delete star;
					if (bullet->tankId == 1) {
						tank1.utraReady = true;
					}
					else {
						tank2.utraReady = true;
					}
					break;
				}
			}
		}
	}
}


void Tema1::SlideTerrain(float deltaTime) {
	int radius = 10;
	float epsilon = 0.001f;
	std::map<int, float> newHeights;

	for (int x = 0; x < initialRes.x + 150; x += map.step) {
		float adjs = 0;
		float heights = 0;

		for (int i = -radius; i <= radius; i++) {
			if (map.heightMap.find(x + i * map.step) != map.heightMap.end()) {
				float adj = exp(-(i * i) / 0.148);
				heights += map.heightMap[x + i * map.step].height * adj;
				adjs += adj;
			}
		}

		if (adjs != 0) {
			float currentHeight = map.heightMap[x].height;
			float newHeight = heights / adjs;
			if (abs(newHeight - currentHeight) < epsilon) {
				continue;
			}
			newHeights[x] = currentHeight + 850 * (newHeight - currentHeight) * deltaTime;
		}
		else {
			newHeights[x] = map.heightMap[x].height;
		}
	}
	// Update new heights
	for (auto& pair : newHeights) {
		map.heightMap[pair.first].height = pair.second;
	}
}

void Tema1::RenderTerrain() {
	glm::mat3 modelMatrix = glm::mat3(1);
	for (int x = 0; x < initialRes.x + 150; x += map.step) {
		MapPoint value = map.heightMap[x];
		MapPoint nextValue;
		if (map.heightMap.find(x + map.step) != map.heightMap.end()) {
			nextValue = map.heightMap[x + map.step];
		}
		else
		{
			break;
		}

		glm::vec2 a = glm::vec2(x, value.height);
		glm::vec2 b = glm::vec2(x + map.step, nextValue.height);

		modelMatrix = glm::mat3(1);

		modelMatrix *= transform2D::Translate(a.x, a.y);
		modelMatrix *= transform2D::Shear(a, b);
		modelMatrix *= transform2D::Scale(b.x - a.x, -max(a.y, b.y));

		RenderMesh2D(value.mesh, modelMatrix, glm::vec3(0.65, 0.59, 0.47));
	}
}

void Tema1::TankLogic(Tank& tank, float deltaTime) {
	if (tank.health > 0) {
		tank.rotate(deltaTime);
		RenderTank(tank);
		tank.calculateTrajectoryOfBullet();
		tank.move(0, map, deltaTime);
	}
}

void Tema1::MoveBullets(float deltaTime) {
	glm::mat3 modelMatrix = glm::mat3(1);
	for (int i = 0; i < bullets.size(); i++) {
		Bullet* bullet = bullets[i];
		if (bullet->isAlive) {
			bullet->move(deltaTime);
			modelMatrix = glm::mat3(1);
			modelMatrix *= transform2D::Translate(bullet->position.x, bullet->position.y);
			RenderMesh2D(bullet->mesh, modelMatrix, glm::vec3(0, 0, 0));
		}
		else {
			bullets.erase(bullets.begin() + i);
		}
	}
}


void Tema1::StarCollisionWithTank() {
	for (int i = 0; i < stars.size(); i++) {
		Star* star = stars[i];

		float distance1 = glm::distance(star->position, tank1.position);
		float distance2 = glm::distance(star->position, tank2.position);

		if (distance1 < 40) {
			tank1.utraReady = true;
			stars.erase(stars.begin() + i);
			delete star;
			i--;

		}

		else if (distance2 < 40) {
			tank2.utraReady = true;
			stars.erase(stars.begin() + i);
			delete star;
			i--;
		}
	}

}
void Tema1::Update(float deltaTimeSeconds)
{

	glm::mat3 modelMatrix = glm::mat3(1);


	RenderBulletLoadingBar(tank1);
	RenderBulletLoadingBar(tank2);

	SlideTerrain(deltaTimeSeconds);
	
	RenderTerrain();

	TankLogic(tank1, deltaTimeSeconds);
	TankLogic(tank2, deltaTimeSeconds);


	DetectBulletCollisions(deltaTimeSeconds);
	MoveBullets(deltaTimeSeconds);


	RenderHealthBar(tank1);
	RenderHealthBar(tank2);

	StarCollisionWithTank();

	// Hearts
	if (heartTimer <= 0) {
		heartTimer = 5 + rand() % 5;
		float x = rand() % initialRes.x + 100;
		auto value = map.findClosestPoint(x);
		float y = value.second->height + 150 + rand() % 100;
		Heart* heart = new Heart(glm::vec2(x, y));
		hearts.push_back(heart);
	}
	else {
		heartTimer -= deltaTimeSeconds;
	}
	RenderHearts(deltaTimeSeconds);

	// Clouds
	MoveClouds(deltaTimeSeconds);
	RenderClouds();

	tank1.UpdateReloadTime(deltaTimeSeconds);
	tank2.UpdateReloadTime(deltaTimeSeconds);

	tank1.ScaleTrajectory(deltaTimeSeconds);
	tank2.ScaleTrajectory(deltaTimeSeconds);


	// Stars Logic
	starTimer -= deltaTimeSeconds;
	if (starTimer <= 0) {
		starTimer = 10;
		float x = rand() % initialRes.x + 100;
		auto value = map.findClosestPoint(x);
		float y = 750;
		Star* star = new Star(glm::vec2(x, y), &value.second->height);
		stars.push_back(star);
	}

	for (int i = 0; i < stars.size(); i++) {
		Star* star = stars[i];
		if (star->liveTime <= 0) {
			stars.erase(stars.begin() + i);
			delete star;
			i--;
			continue;
		}

		star->Falling(deltaTimeSeconds);
		modelMatrix = glm::mat3(1);
		modelMatrix *= transform2D::Translate(star->position.x, star->position.y);
		modelMatrix *= transform2D::Rotate(star->angle);
		RenderMesh2D(star->star, modelMatrix, glm::vec3(0.99, 0.95, 0.6));
		star->liveTime -= deltaTimeSeconds;
	}
}



void Tema1::RenderBulletLoadingBar(Tank& tank) {
	glm::mat3 modelMatrix = glm::mat3(1);
	modelMatrix = glm::mat3(1);
	modelMatrix *= transform2D::Translate(tank.position.x, tank.position.y);
	modelMatrix *= transform2D::Rotate(tank.currentAngle);
	modelMatrix *= transform2D::Scale(tank.reloadTime / 0.5, 1);
	RenderMesh2D(tank.reloadBar, modelMatrix, glm::vec3(0.3, 0.3, 0.3));
}

void Tema1::RenderHearts(float deltaTime) {
	for (int i = 0; i < hearts.size(); i++) {
		Heart* heart = hearts[i];
		heart->disappearTime -= deltaTime;
		if (heart->disappearTime <= 0) {
			hearts.erase(hearts.begin() + i);
			delete heart;
			i--;
			continue;
		}

		heart->scaling += heart->scaleUp ? deltaTime : -deltaTime;

		if (heart->scaling > 1.25) {
			heart->scaling = 1.25;
			heart->scaleUp = false;
		}
		if (heart->scaling < 1) {
			heart->scaling = 1;
			heart->scaleUp = true;
		}


		glm::mat3 modelMatrix = glm::mat3(1);
		modelMatrix *= transform2D::Translate(heart->position.x, heart->position.y);
		modelMatrix *= transform2D::Scale(heart->scaling, heart->scaling);
		RenderMesh2D(heart->heart, modelMatrix, glm::vec3(0.66, 0.33, 0.52));
		RenderMesh2D(heart->heartOutline, modelMatrix, glm::vec3(0.7, 0.7, 0.7));
	}
}

void Tema1::RenderClouds() {
	for (int i = 0; i < clouds.size(); i++) {
		Cloud* cloud = clouds[i];
		glm::mat3 modelMatrix = glm::mat3(1);
		modelMatrix *= transform2D::Translate(cloud->position.x, cloud->position.y);
		RenderMesh2D(cloud->circle1, modelMatrix, glm::vec3(0.9, 0.9, 0.9));
		modelMatrix = glm::mat3(1);
		modelMatrix *= transform2D::Translate(cloud->position.x, cloud->position.y);
		RenderMesh2D(cloud->circle2, modelMatrix, glm::vec3(0.9, 0.9, 0.9));
		modelMatrix = glm::mat3(1);
		modelMatrix *= transform2D::Translate(cloud->position.x, cloud->position.y);
		RenderMesh2D(cloud->circle3, modelMatrix, glm::vec3(0.9, 0.9, 0.9));
	}
}

void Tema1::RenderHealthBar(Tank& tank) {
	glm::mat3 modelMatrix;
	modelMatrix = glm::mat3(1);
	modelMatrix *= transform2D::Translate(tank.position.x, tank.position.y);
	modelMatrix *= transform2D::Scale((float)tank.health / 5, 1);
	RenderMesh2D(tank.hpBar, modelMatrix, glm::vec3(0.66, 0.33, 0.52));

}

void Tema1::RenderTank(Tank& tank) {
	glm::mat3 modelMatrix = glm::mat3(1);

	modelMatrix = glm::mat3(1);
	modelMatrix *= transform2D::Translate(tank.position.x, tank.position.y);
	modelMatrix *= transform2D::Rotate(tank.currentAngle);
	RenderMesh2D(tank.mesh1, modelMatrix, tank.lowerBodyColor);
	RenderMesh2D(tank.mesh2, modelMatrix, tank.upperBodyColor);

	RenderMesh2D(tank.mesh3, modelMatrix, tank.circleColor);


	glm::mat3 modelMatrix2 = glm::mat3(1);
	modelMatrix = glm::mat3(1);


	// Trajectory
	modelMatrix2 *= transform2D::Translate(tank.position.x, tank.position.y);
	modelMatrix2 *= transform2D::Rotate(tank.currentAngle);
	modelMatrix2 *= transform2D::Translate(0, tank.gunCenter.y);
	modelMatrix2 *= transform2D::Rotate(-tank.currentAngle);
	modelMatrix2 *= transform2D::Translate(0, -tank.gunCenter.y);



	glm::vec3 localGunTip = glm::vec3(0, tank.gunCenter.y, 1.0f);
	tank.bulletStartPosition = modelMatrix2 * localGunTip;

	modelMatrix *= transform2D::Translate(tank.bulletStartPosition.x, tank.bulletStartPosition.y);
	modelMatrix *= transform2D::Rotate(tank.currentAngle);
	modelMatrix *= transform2D::Translate(0, -28);
	modelMatrix *= transform2D::Translate(0, tank.gunCenter.y);
	modelMatrix *= transform2D::Rotate(tank.gunAngle2 - tank.currentAngle);
	modelMatrix *= transform2D::Translate(0, -tank.gunCenter.y);


	RenderMesh2D(tank.gun, modelMatrix, glm::vec3(0.1, 0.2, 0.1));

	glLineWidth(tank.trajectoryScale);
	float dif = fmod(tank.gunAngle2 - tank.currentAngle + 2 * M_PI, 2 * M_PI);
	
	if (dif > M_PI) {
		dif -= 2 * M_PI;
	}

	if ((dif < 3.3f && dif > -0.3) || (dif > -3.3f && dif < -2.8f))
		if (tank.utraReady)
			RenderMesh2D(tank.trajectory, modelMatrix2, glm::vec3(0.99, 0.62, 0.33));
		else
			RenderMesh2D(tank.trajectory, modelMatrix2, glm::vec3(0.9, 0.9, 0.9));
	glLineWidth(1.0f);
}


void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	if (tank1.health > 0) {
		if (window->KeyHold(GLFW_KEY_A)) {
			tank1.move(-1, map, deltaTime);
		}
		if (window->KeyHold(GLFW_KEY_D)) {
			tank1.move(1, map, deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_W)) {
			tank1.rotateTheGun(1, deltaTime);
		}
		if (window->KeyHold(GLFW_KEY_S)) {
			tank1.rotateTheGun(-1, deltaTime);
		}
	}

	if (tank2.health > 0) {
		if (window->KeyHold(GLFW_KEY_LEFT)) {
			tank2.move(-1, map, deltaTime);
		}
		if (window->KeyHold(GLFW_KEY_RIGHT)) {
			tank2.move(1, map, deltaTime);
		}
		if (window->KeyHold(GLFW_KEY_UP)) {
			tank2.rotateTheGun(1, deltaTime);
		}
		if (window->KeyHold(GLFW_KEY_DOWN)) {
			tank2.rotateTheGun(-1, deltaTime);
		}
	}
}

void Tema1::OnShootButtonPress(Tank& tank) {
	float dif = fmod(tank.gunAngle2 - tank.currentAngle + 2 * M_PI, 2 * M_PI);

	if (dif > M_PI) {
		dif -= 2 * M_PI;
	}

	if (!((dif < 3.3f && dif > -0.3) || (dif > -3.3f && dif < -2.8f))) {
		return;
	}

	if (tank.reloadTime <= 0) {
		if (tank.utraReady) {
			tank.createTripleBullet(bullets);
			tank.reloadTime = 0.5f;
			tank.utraReady = false;
		}
		else {
			Bullet* bullet = tank.createBullet();
			bullets.push_back(bullet);
			tank.reloadTime = 0.5f;
		}
	}
}

void Tema1::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_SPACE && tank1.health > 0) {
		OnShootButtonPress(tank1);	
	}
	if (key == GLFW_KEY_ENTER && tank2.health > 0) {
		OnShootButtonPress(tank2);
	}
}


void Tema1::OnKeyRelease(int key, int mods)
{
	// Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// Add mouse move event
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// Add mouse button press event
}



void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
