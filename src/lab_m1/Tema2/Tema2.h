#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/tema_camera.h"
#include "lab_m1/Tema2/object_3D.h"
#include "lab_m1/Tema2/transform3d.h"
#include "lab_m1/Tema2/Drone.h"
#include "lab_m1/Tema2/MapTema2.h"
#include "lab_m1/Tema2/Packet.h"
#include "lab_m1/Tema2/CheckPoint.h"
#include "components/text_renderer.h"


#include <set>



namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
         struct ViewportArea
         {
             ViewportArea() : x(0), y(0), width(1), height(1) {}
             ViewportArea(int x, int y, int width, int height)
                 : x(x), y(y), width(width), height(height) {}
             int x;
             int y;
             int width;
             int height;
         };
        Tema2();
        ~Tema2();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix) override;
		void RenderWithMyShader(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);
		void RenderAttitudeShader(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        glm::mat4 shadowProjectionMatrix(glm::vec3 lightDir, float groundY);
        bool isPositionValid(int x, int z, std::set<std::pair<int, int>>& usedPositions);
        void RenderScene();
        void Process_Cameras();
		void CollisionDetection(float deltaTime);
		void RenderWithMyColor(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec4 color);
        void DrawHUD();
		void RenderWithMySky(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, float timeOfDay);

     protected:
        implemented::MyCamera *camera;
        implemented::MyCamera* minimapCam;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;
        bool spacePressed = false;

        // TODO(student): If you need any other class variables, define them here.
        gfxc::TextRenderer* textRenderer;

        const glm::vec3 kTextColor = NormalizedRGB(166, 172, 205);
        const glm::vec3 kBackgroundColor = NormalizedRGB(41, 45, 62);

		Drone drone;
		Packet packet;
		CheckPoint checkPoint;

        
        MapTema2 map;
        
		bool isInputOX = false;
		bool isInputOZ = false;
        bool minimapCameraEnabled = false;
        ViewportArea miniViewportArea;
        Mesh* packetPoint;
        Mesh* minimapPoint;
        Mesh* minimapPoint2;
        Mesh* checkPointMinimap;
		glm::mat4 minimapProjectionMatrix;
		bool limitBackwardSpeed = false;
		bool isGoingBackward = false;
		bool isGoingLeft = false;
		bool isGoingRight = false;
		bool blockS = false;

		int score = 0;
		bool gameStarted = false;
		bool gameEnded = false;
		float timer = 0;
		float timeOfDay = 0;

        std::set<std::pair<int, int>> usedPositions;

        std::vector<glm::vec3> CloudsPos;
        std::vector<float> CloudsRotations;

		std::vector<glm::vec3> thunderPos;
		float thunderTime = 5;
        float maxSpeed = 2.5f;
		float boostTime = 3;
		bool boostEnabled = false;
        float backwardCamera = 3.f;
    };
}   // namespace m1
