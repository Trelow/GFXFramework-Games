#pragma once

#include "components/simple_scene.h"
#include "Map.h"
#include "Tank.h"
#include "Cloud.h"
#include "Heart.h"
#include "Star.h"

namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

		float MapFunction(int t);
        void DetectBulletCollisions(float deltaTime);
		void RenderTank(Tank& tank);
		void RenderHealthBar(Tank& tank);
		void MoveAI(float deltaTime);
		void RenderClouds();
		void RenderHearts(float deltaTime);
		void GenerateClouds();
		void MoveClouds(float deltaTime);
		void RenderBulletLoadingBar(Tank& tank);
		void GenerateMap(glm::ivec2& resolution);
        void ExplodeTerrain(std::pair<int, MapPoint*>& value);
		void SlideTerrain(float deltaTime);
		void RenderTerrain();
		void TankLogic(Tank &tank, float deltaTime);
		void MoveBullets(float deltaTime);
		void StarCollisionWithTank();
		void OnShootButtonPress(Tank& tank);

    protected:

        float A1 = 100.0f;
        float A2 = 30.0f;
        float O1 = 0.009f;
        float O2 = 0.025f;
        
		float heartTimer = 3.f;

     /*   float A1 = 70.0f;
        float A2 = 30.0f;
        float Omega1 = 0.004f;
        float Omega2 = 0.01f;*/


		Map map;
        glm::ivec2 initialRes;

		Tank tank1, tank2;
		bool isAIenabled = false;

		std::vector<Bullet*> bullets;
		std::vector<Cloud*> clouds;
		std::vector<Heart*> hearts;
		std::vector<Star*> stars;
		float starTimer = 10.f;

    };
}   // namespace m1
