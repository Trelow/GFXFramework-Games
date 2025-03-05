#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>



using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}

bool Tema2::isPositionValid(int x, int z, set<std::pair<int, int>>&usedPositions) {
    for (auto& position : usedPositions) {
        if (glm::distance(glm::vec2(x, z), glm::vec2(position.first, position.second)) < 1.5f) {
            return false;
        }
    }
    return true;
}

void Tema2::Init()
{
    Mesh* mesh = new Mesh("arrow");
    bool loaded = mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "arrow.fbx");
    meshes[mesh->GetMeshID()] = mesh;

	Mesh* mesh2 = new Mesh("clouds");
	loaded = mesh2->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "Cloud.obj");
	meshes[mesh2->GetMeshID()] = mesh2;

	Mesh* mesh3 = new Mesh("thunder");
	loaded = mesh3->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "thunder2.fbx");
	meshes[mesh3->GetMeshID()] = mesh3;

    renderCameraTarget = true;

    camera = new implemented::MyCamera();
    camera->Set(glm::vec3(0, 1, 3.5), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

	glm::ivec2 resolution = window->GetResolution();
    textRenderer = new gfxc::TextRenderer(window->props.selfDir, resolution.x, resolution.y);

    textRenderer->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Hack-Bold.ttf"), 100);
	cout << textRenderer->Characters.size() << endl;

	camera->distanceToTarget = 3.5;

	minimapCam = new implemented::MyCamera();
	minimapCam->Set(glm::vec3(0, 15, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));

	minimapProjectionMatrix = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, 0.01f, 200.0f);

    // Minimap
    miniViewportArea = ViewportArea(resolution.x - 220, resolution.y - 220, 200, 200);
    minimapPoint = object_3D::CreateCylinder("minimapPoint", glm::vec3(0, 10, 0), 2, 0.5, glm::vec3(1, 0, 0), true);
	packetPoint = object_3D::CreateCube("packetPoint", glm::vec3(0, 10, 0), 3, glm::vec3(1, 1, 0), true);
    checkPointMinimap = object_3D::CreateCylinder("cylinder", glm::vec3(0, 10, 0), 2.5f, 0.5f, glm::vec3(0.f, 0.9, 0.9), true);


    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);


    Shader* shader = new Shader("GroundShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "SimpleVS.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "SimpleFS.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;

	Shader* sh = shaders["GroundShader"];


    shader = new Shader("ColorShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "SimpleColorVS.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "SimpleColorFS.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;

    shader = new Shader("ShadowShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "ShadowVS.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "ShadowFS.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;

    int treeCount = 50;
    
	cout << drone.dronePosition.x << " " << drone.dronePosition.y << " " << drone.dronePosition.z << endl;

    for (int i = 0; i < treeCount; i++) {
        Tree* tree = new Tree();
        int x, z;

        do {

            x = rand() % 100 - 50;
            z = rand() % 100 - 50;
			if (abs(x - drone.dronePosition.x) < 2.5 && abs(z - drone.dronePosition.z) < 2.5) {
				continue;
			}

		} while (!isPositionValid(x, z, usedPositions));

        usedPositions.insert({ x, z });

        float height = map.getTerrainHeight(static_cast<float>(x), static_cast<float>(z));
        tree->position = glm::vec3(x, height, z);

        map.trees.push_back(tree);
    }


	packet.position = glm::vec3(0, 0, -1000);

	drone.rotationOY = RADIANS(180);

	// ADD CLOUDS POSITION
	float x = 0;
	float z = 0;
    for (int i = 0; i < 15; i++) {
        float x = 150 * cos(i * 2 * M_PI / 15);
        float z = 150 * sin(i * 2 * M_PI / 15);
		float y = rand() % 50 + 10;
        CloudsPos.push_back(glm::vec3(x, y, z));

        float rotation = atan2(z, x);
        CloudsRotations.push_back(rotation);

    }

}


void Tema2::FrameStart()
{
    glClearColor(0.1, 0.95, 0.95, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{

    if (boostEnabled)
    {
		backwardCamera += deltaTimeSeconds * 2;
        if (backwardCamera > 3.5) {
            backwardCamera = 3.5;
        }

    }
    else {
        backwardCamera -= deltaTimeSeconds;
        if (backwardCamera < 3) {
            backwardCamera = 3;
        }
    }

	thunderTime -= deltaTimeSeconds;
	if (thunderTime <= 0) {
		thunderTime = 5;
		int x, z;
		x = rand() % 100 - 50;
		z = rand() % 100 - 50;
		glm::vec3 thunderPosition = glm::vec3(x, map.getTerrainHeight(static_cast<float>(x), static_cast<float>(z)) + 3.f, z);
		if (thunderPos.size() < 3) {
			thunderPos.push_back(thunderPosition);
		}
		else {
			thunderPos.erase(thunderPos.begin());
			thunderPos.push_back(thunderPosition);
		}


	}

	if (boostEnabled) {
		boostTime -= deltaTimeSeconds;
		if (boostTime <= 0) {
			boostEnabled = false;
			maxSpeed = 2.5f;
		}
	}

	for (int i = 0; i < thunderPos.size(); i++) {
		glm::vec3 thunderPosition = thunderPos[i];
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= transform3d::Translate(thunderPosition.x, thunderPosition.y, thunderPosition.z);
		modelMatrix *= transform3d::Scale(0.0015, 0.0015, 0.0015);
		RenderWithMyColor(meshes["thunder"], shaders["ColorShader"], modelMatrix, glm::vec4(1, 1, 0, 1));
	}

	// Render Clouds
    for (int i = 0; i < 15; i++) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= transform3d::Translate(CloudsPos[i].x, CloudsPos[i].y, CloudsPos[i].z);
		modelMatrix *= transform3d::RotateOY(RADIANS(90));
        modelMatrix *= transform3d::RotateOY(-CloudsRotations[i]);
        modelMatrix *= transform3d::RotateOZ(RADIANS(15));
		modelMatrix *= transform3d::Scale(7, 7, 7);
		RenderWithMyColor(meshes["clouds"], shaders["ColorShader"], modelMatrix, glm::vec4(1, 1, 1, 1));

        CloudsRotations[i] += deltaTimeSeconds * 0.002f;
        if (CloudsRotations[i] > 2 * M_PI) {
            CloudsRotations[i] -= 2 * M_PI;
        }

        CloudsPos[i].x = 150 * cos(CloudsRotations[i]);
        CloudsPos[i].z = 150 * sin(CloudsRotations[i]);

        CloudsRotations[i] = atan2(CloudsPos[i].z, CloudsPos[i].x);
	}

    if (gameStarted) {
        timer -= deltaTimeSeconds;
        if (timer <= 0) {
            timer = 0.f;
            gameEnded = true;
            gameStarted = false;

        }
    }

	timeOfDay += deltaTimeSeconds;
	if (timeOfDay > 1) {
		timeOfDay = 0;
	}


    drone.propellerAngle += deltaTimeSeconds * drone.propellerSpeed;

    glm::mat4 rotationMatrix = glm::mat4(1);

    rotationMatrix *= transform3d::RotateOY(drone.rotationOY);
    if (abs(drone.rotationOX) > 0.01f) {
        rotationMatrix *= transform3d::RotateOX(drone.rotationOX);
    }
    if (abs(drone.rotationOZ) > 0.01f) {
        rotationMatrix *= transform3d::RotateOZ(drone.rotationOZ);
    }


    if (drone.rotationOZ > 0.01f) {
        drone.rotationOZ -= deltaTimeSeconds / 3;
    }
    else if (drone.rotationOZ < -0.01f) {
        drone.rotationOZ += deltaTimeSeconds / 3;
    }

    RenderScene();
    DrawHUD();

    glClear(GL_DEPTH_BUFFER_BIT);
    minimapCameraEnabled = true;
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);
    RenderScene();
    minimapCameraEnabled = false;

    if ((drone.dronePosition.y - 0.2 < map.getTerrainHeight(drone.dronePosition.x, drone.dronePosition.z))) {
        blockS = true;
    }
    else
    {
        blockS = false;
    }

        glm::vec3 futurePosition = drone.dronePosition + drone.velocity * deltaTimeSeconds * 2.f;

		if (futurePosition.y  - 0.1< map.getTerrainHeight(futurePosition.x, futurePosition.z)) {
			drone.velocity.y = 0;
			drone.dronePosition.y -= drone.velocity.y * deltaTimeSeconds * 2.f;

			drone.velocity.x = 0;
			drone.velocity.z = 0;
			drone.dronePosition.x -= drone.velocity.x * deltaTimeSeconds * 2.f;
			drone.dronePosition.z -= drone.velocity.z * deltaTimeSeconds * 2.f;

		}
        

		CollisionDetection(deltaTimeSeconds);


	if (drone.dronePosition.y > 20) {
        drone.dronePosition.y = 20;
        drone.velocity.y = 0;
	}
    
    drone.dronePosition += drone.velocity * deltaTimeSeconds * 2.f;

    Process_Cameras();
}

void Tema2::DrawHUD()
{

    int rowIndex = 0;
    std::string polygonModeText = "";

        polygonModeText = "wireframe";
   
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	string scoreText = to_string(score);
    textRenderer->RenderText(scoreText, 5.f, 10.f, 1.0f, glm::vec3(0.f, 0.f, 0.f));


	if (gameStarted == false)
	{
		textRenderer->RenderText("Press ANY KEY to play", 165.f, 550.f, 0.75f, glm::vec3(0.7f, 0.7f, 0.7f));
	}
    else {
		string timerText = to_string(timer);
		timerText = timerText.substr(0, timerText.find(".") + 2);
		textRenderer->RenderText(timerText, 1025.f, 600.f, 1.0f, glm::vec3(0.9f, 0.3f, 0.6f));
    }

	if (gameEnded) {
		textRenderer->RenderText("Game Over", 375.f, 170.f, 1.0f, glm::vec3(0.9f, 0.3f, 0.6f));
	}

}


void Tema2::CollisionDetection(float deltaTime) {
    // Check collision with thunders
    for (int i = 0; i < thunderPos.size(); i++) {
		glm::vec3 thunderPosition = thunderPos[i];

		float distance = glm::distance(thunderPosition, drone.dronePosition);

        if (distance < 0.7f) {
			boostEnabled = true;
			maxSpeed = 5.f;
			boostTime = 4;
			thunderPos.erase(thunderPos.begin() + i);
        }

    }


    glm::vec3 futurePosition = drone.dronePosition + drone.velocity * deltaTime * 2.f;

    // Check collision with trees
    for (int i = 0; i < map.trees.size(); i++) {
        Tree* tree = map.trees[i];

        glm::vec3 stemPosition = tree->position;
        stemPosition.y -= 0.2f;
        float height = tree->cylinderHeightCollider;


        float localY = futurePosition.y - stemPosition.y;
        float radius = tree->cylinderRadiusCollider;
        float horizontalDistance = glm::distance(glm::vec2(futurePosition.x, futurePosition.z), glm::vec2(stemPosition.x, stemPosition.z));


        if (futurePosition.x > stemPosition.x - radius - 0.1 && futurePosition.x < stemPosition.x + radius + 0.1
            && futurePosition.z > stemPosition.z - radius - 0.1 && futurePosition.z < stemPosition.z + radius + 0.1) {
            if (futurePosition.y > stemPosition.y && futurePosition.y < stemPosition.y + height) {
                drone.dronePosition.x -= drone.velocity.x * deltaTime * 2.f;
                drone.dronePosition.z -= drone.velocity.z * deltaTime * 2.f;
                drone.velocity.x = 0;
                drone.velocity.z = 0;

            }

        }
    }

    for (int i = 0; i < map.trees.size(); i++) {
        Tree* tree = map.trees[i];

        glm::vec3 crownPosition = tree->position + glm::vec3(0, tree->crownYOffsetCollider, 0);
        float height = tree->crownHeightCollider;


        float localY = futurePosition.y - crownPosition.y;
        float radius = tree->crownRadiusCollider;

        float horizontalDistance = glm::distance(glm::vec2(futurePosition.x, futurePosition.z), glm::vec2(crownPosition.x, crownPosition.z));


        if (futurePosition.x > crownPosition.x - radius - 0.1 && futurePosition.x < crownPosition.x + radius + 0.1
            && futurePosition.z > crownPosition.z - radius - 0.1 && futurePosition.z < crownPosition.z + radius + 0.1) {

            if (futurePosition.y > crownPosition.y && futurePosition.y < crownPosition.y + height) {
                drone.dronePosition.x -= drone.velocity.x * deltaTime * 2.f;
                drone.dronePosition.z -= drone.velocity.z * deltaTime * 2.f;
                drone.velocity.x = 0;
                drone.velocity.z = 0;

            }
            if ((futurePosition.y + 0.1f > crownPosition.y && drone.dronePosition.y < crownPosition.y && drone.velocity.y > 0) ||
                (futurePosition.y - 0.1f < crownPosition.y + height && drone.dronePosition.y > crownPosition.y + height && drone.velocity.y < 0)) {
                drone.dronePosition.y -= drone.velocity.y * deltaTime * 2.f;
                drone.velocity.y = 0;

            }



        }


        }

    for (int i = 0; i < map.trees.size(); i++) {
        Tree* tree = map.trees[i];
        glm::vec3 crownPosition = tree->position + glm::vec3(0, tree->crownYOffsetCollider + tree->crownHeightCollider, 0);
        float height = 1.f;


        float localY = futurePosition.y - crownPosition.y;
        float radius = tree->crownRadiusCollider / 2;

        float horizontalDistance = glm::distance(glm::vec2(futurePosition.x, futurePosition.z), glm::vec2(crownPosition.x, crownPosition.z));


        if (futurePosition.x > crownPosition.x - radius - 0.1 && futurePosition.x < crownPosition.x + radius + 0.1
            && futurePosition.z > crownPosition.z - radius - 0.1 && futurePosition.z < crownPosition.z + radius + 0.1) {

            if (futurePosition.y > crownPosition.y && futurePosition.y < crownPosition.y + height) {
                drone.dronePosition.x -= drone.velocity.x * deltaTime * 2.f;
                drone.dronePosition.z -= drone.velocity.z * deltaTime * 2.f;
                drone.velocity.x = 0;
                drone.velocity.z = 0;

            }


            if ((futurePosition.y + 0.1f > crownPosition.y && drone.dronePosition.y < crownPosition.y && drone.velocity.y > 0) ||
                (futurePosition.y - 0.1f < crownPosition.y + height && drone.dronePosition.y > crownPosition.y + height && drone.velocity.y < 0)) {
                drone.dronePosition.y -= drone.velocity.y * deltaTime * 2.f;
                drone.velocity.y = 0;

            }



        }


    }
	

    if (!packet.isCollected && !gameEnded) {
        glm::vec3 packetPosition = packet.position;
        glm::vec3 dronePosition = drone.dronePosition;

        float distance = glm::distance(glm::vec2(packetPosition.x, packetPosition.z), glm::vec2(dronePosition.x, dronePosition.z));

        if (abs(packetPosition.y - dronePosition.y) < packet.boxCollieder + 0.1f && distance < packet.boxCollieder + 0.2f) {
            packet.isCollected = true;

            int x, z;
            do {
                x = rand() % 100 - 50;
                z = rand() % 100 - 50;
            } while (!isPositionValid(x, z, usedPositions));
			checkPoint.position = glm::vec3(x, map.getTerrainHeight(static_cast<float>(x), static_cast<float>(z)), z);
        }
    }

	if (packet.isCollected  && !gameEnded) {
		glm::vec3 checkPointPosition = checkPoint.position;
		glm::vec3 dronePosition = drone.dronePosition;

		float distance = glm::distance(glm::vec2(checkPointPosition.x, checkPointPosition.z), glm::vec2(dronePosition.x, dronePosition.z));
		if (distance < checkPoint.colliderRadius + 0.15f) {
			int x, z;
			x = rand() % 100 - 50;
			z = rand() % 100 - 50;
			packet.position = glm::vec3(x, map.getTerrainHeight(static_cast<float>(x), static_cast<float>(z)), z);
			packet.isCollected = false;
			score++;
            
			if (score > 10) {
                timer += 15;
			}
			else {
				timer += 25.f - score % 10;
			}
		}
	}


}


void Tema2::Process_Cameras() {
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea.x = resolution.x - 220;
    miniViewportArea.y = resolution.y - 220;

    glm::mat4 customRotationMatrix = glm::mat4(1.0f);
    customRotationMatrix = transform3d::RotateOX(drone.rotationOX / 2.f) * customRotationMatrix;
    customRotationMatrix = transform3d::RotateOY(drone.rotationOY) * customRotationMatrix;


    glm::vec4 initialForward = glm::vec4(0, 0, 1, 0);

    glm::vec4 calculatedForward = customRotationMatrix * initialForward;


    drone.forward = glm::normalize(glm::vec3(calculatedForward));

    glm::vec3 backward = glm::normalize(drone.forward) * -1.f;
    glm::vec3 cameraPosition = drone.dronePosition + backward * backwardCamera + glm::vec3(0.f, 0.7f, 0.f);
    camera->Set(cameraPosition, drone.dronePosition - glm::vec3(0, -0.5, 0), glm::vec3(0, 1, 0));
}

void Tema2::RenderScene() {
    glm::mat4 modelMatrix = glm::mat4(1);


    if (!minimapCameraEnabled) {
        modelMatrix *= transform3d::Translate(drone.dronePosition.x, drone.dronePosition.y, drone.dronePosition.z);
        modelMatrix *= transform3d::RotateOY(drone.rotationOY);
        modelMatrix *= transform3d::RotateOX(drone.rotationOX);
        modelMatrix *= transform3d::RotateOZ(drone.rotationOZ);
        modelMatrix *= transform3d::RotateOY(drone.parallelipipedAngle);


        RenderMesh(drone.parallelipiped1, shaders["VertexColor"], modelMatrix);
        RenderMesh(drone.cube1, shaders["VertexColor"], modelMatrix);
        RenderMesh(drone.cube3, shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3d::Translate(drone.dronePosition.x, drone.dronePosition.y, drone.dronePosition.z);
        modelMatrix *= transform3d::RotateOY(drone.rotationOY);
        modelMatrix *= transform3d::RotateOX(drone.rotationOX);
        modelMatrix *= transform3d::RotateOZ(drone.rotationOZ);
        modelMatrix *= transform3d::RotateOY(-drone.parallelipipedAngle);


        RenderMesh(drone.parallelipiped2, shaders["VertexColor"], modelMatrix);
        RenderMesh(drone.cube2, shaders["VertexColor"], modelMatrix);
        RenderMesh(drone.cube4, shaders["VertexColor"], modelMatrix);
        modelMatrix *= transform3d::RotateOY(drone.propellerAngle);

        glm::mat4 propellerModelMatrix = glm::mat4(1);
        propellerModelMatrix *= transform3d::Translate(drone.dronePosition.x, drone.dronePosition.y, drone.dronePosition.z);
        propellerModelMatrix *= transform3d::RotateOY(drone.rotationOY);
        propellerModelMatrix *= transform3d::RotateOX(drone.rotationOX);
        propellerModelMatrix *= transform3d::RotateOZ(drone.rotationOZ);
        propellerModelMatrix *= transform3d::Translate(0.3, 0.08, 0.2775);

        propellerModelMatrix *= transform3d::RotateOY(drone.propellerAngle);

        RenderMesh(drone.propeller1, shaders["VertexColor"], propellerModelMatrix);


        propellerModelMatrix = glm::mat4(1);
        propellerModelMatrix *= transform3d::Translate(drone.dronePosition.x, drone.dronePosition.y, drone.dronePosition.z);
        propellerModelMatrix *= transform3d::RotateOY(drone.rotationOY);
        propellerModelMatrix *= transform3d::RotateOX(drone.rotationOX);
        propellerModelMatrix *= transform3d::RotateOZ(drone.rotationOZ);
        propellerModelMatrix *= transform3d::Translate(-0.2775, 0.08, -0.3);

        propellerModelMatrix *= transform3d::RotateOY(drone.propellerAngle);


        RenderMesh(drone.propeller2, shaders["VertexColor"], propellerModelMatrix);

        propellerModelMatrix = glm::mat4(1);
        propellerModelMatrix *= transform3d::Translate(drone.dronePosition.x, drone.dronePosition.y, drone.dronePosition.z);
        propellerModelMatrix *= transform3d::RotateOY(drone.rotationOY);
        propellerModelMatrix *= transform3d::RotateOX(drone.rotationOX);
        propellerModelMatrix *= transform3d::RotateOZ(drone.rotationOZ);
        propellerModelMatrix *= transform3d::Translate(0.2775, 0.08, -0.3);

        propellerModelMatrix *= transform3d::RotateOY(drone.propellerAngle);

        RenderMesh(drone.propeller3, shaders["VertexColor"], propellerModelMatrix);


        propellerModelMatrix = glm::mat4(1);
        propellerModelMatrix *= transform3d::Translate(drone.dronePosition.x, drone.dronePosition.y, drone.dronePosition.z);
        propellerModelMatrix *= transform3d::RotateOY(drone.rotationOY);
        propellerModelMatrix *= transform3d::RotateOX(drone.rotationOX);
        propellerModelMatrix *= transform3d::RotateOZ(drone.rotationOZ);
        propellerModelMatrix *= transform3d::Translate(-0.3, 0.08, 0.2775);

        propellerModelMatrix *= transform3d::RotateOY(drone.propellerAngle);

        RenderMesh(drone.propeller4, shaders["VertexColor"], propellerModelMatrix);
    }
    else {
		modelMatrix = glm::mat4(1);
		modelMatrix *= transform3d::Translate(drone.dronePosition.x, 0, drone.dronePosition.z);
		RenderMesh(minimapPoint, shaders["VertexColor"], modelMatrix);
        if (!packet.isCollected) {
            modelMatrix = glm::mat4(1);
            modelMatrix *= transform3d::Translate(packet.position.x, 0, packet.position.z);
			modelMatrix *= transform3d::Translate(-1.5f, 0.f, -1.5f);
            RenderMesh(packetPoint, shaders["VertexColor"], modelMatrix);
        }
		if (packet.isCollected) {
			modelMatrix = glm::mat4(1);
			modelMatrix *= transform3d::Translate(checkPoint.position.x, 0, checkPoint.position.z);
			RenderMesh(checkPointMinimap, shaders["VertexColor"], modelMatrix);
		}
    }


    // Map
    modelMatrix = glm::mat4(1);

    RenderWithMyShader(map.plane, shaders["GroundShader"], modelMatrix);


    for (int i = 0; i < map.trees.size(); i++) {
        Tree* tree = map.trees[i];
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3d::Translate(tree->position.x, tree->position.y, tree->position.z);

        RenderMesh(map.trees[i]->cone1, shaders["VertexColor"], modelMatrix);
		

        RenderMesh(map.trees[i]->cone2, shaders["VertexColor"], modelMatrix);

        RenderMesh(map.trees[i]->cilinder, shaders["VertexColor"], modelMatrix);

    }


		

	// Render arrow
	modelMatrix = glm::mat4(1);
    modelMatrix *= transform3d::Translate(drone.dronePosition.x, drone.dronePosition.y, drone.dronePosition.z);
    modelMatrix *= transform3d::Translate(0, 0.35f, 0);
    modelMatrix *= transform3d::RotateOX(RADIANS(90));


    if (!packet.isCollected) {
        glm::vec3 direction = glm::normalize(packet.position - drone.dronePosition);
        float angle = atan2(direction.z, direction.x);
        modelMatrix *= transform3d::RotateOZ(angle);
    }
    else {
		glm::vec3 direction = glm::normalize(checkPoint.position - drone.dronePosition);
		float angle = atan2(direction.z, direction.x);
		modelMatrix *= transform3d::RotateOZ(angle);
    }

	modelMatrix *= transform3d::Scale(0.2f, 0.2f, 0.2f);

    if (!packet.isCollected)
	    RenderWithMyColor(meshes["arrow"], shaders["ColorShader"], modelMatrix, glm::vec4(0.9f, 0.7f, 0.5f, 1.f));
    else {
        RenderWithMyColor(meshes["arrow"], shaders["ColorShader"], modelMatrix, glm::vec4(0.3f, 0.7f, 0.9f, 1.f));
    }

    if (packet.isCollected){
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3d::Translate(drone.dronePosition.x, drone.dronePosition.y, drone.dronePosition.z);
        modelMatrix *= transform3d::RotateOY(drone.rotationOY);
        modelMatrix *= transform3d::RotateOX(drone.rotationOX);
        modelMatrix *= transform3d::RotateOZ(drone.rotationOZ);
		modelMatrix *= transform3d::Translate(-0.2f, -0.225, -0.2);
    }
    else {
		modelMatrix = glm::mat4(1);
		modelMatrix *= transform3d::Translate(packet.position.x, packet.position.y, packet.position.z);
    }
   
	RenderMesh(packet.box, shaders["VertexColor"], modelMatrix);


    glEnable(0x0B44);
    glCullFace(0x0404);
    glFrontFace(0x0901);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (packet.isCollected) {
		modelMatrix = glm::mat4(1);
		modelMatrix *= transform3d::Translate(checkPoint.position.x, checkPoint.position.y, checkPoint.position.z);
		RenderWithMyColor(checkPoint.cylinder, shaders["ColorShader"], modelMatrix, glm::vec4(0.3f, 0.7f, 0.9f, 0.6));
	}

    glDisable(0x0B44);
	glDisable(GL_BLEND);
	

}



void Tema2::FrameEnd()
{
}


void Tema2::RenderWithMyColor(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec4 color) {
    if (!mesh || !shader || !shader->program)
        return;

    shader->Use();
    shader->loc_view_matrix = glGetUniformLocation(shader->program, "view");
    shader->loc_projection_matrix = glGetUniformLocation(shader->program, "projection");
    shader->loc_model_matrix = glGetUniformLocation(shader->program, "model");
    glm::mat4 viewMatrix = minimapCameraEnabled ? minimapCam->GetViewMatrix() : camera->GetViewMatrix();
    glm::mat4 projection = minimapCameraEnabled ? minimapProjectionMatrix : projectionMatrix;
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    
	glUniform4f(glGetUniformLocation(shader->program, "in_color"), color.x, color.y, color.z, color.w);


    mesh->Render();
}
void Tema2::RenderWithMyShader(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) {
	if (!mesh || !shader || !shader->program)
		return;

	shader->Use();
	shader->loc_view_matrix = glGetUniformLocation(shader->program, "view");
	shader->loc_projection_matrix = glGetUniformLocation(shader->program, "projection");
	shader->loc_model_matrix = glGetUniformLocation(shader->program, "model");
	glm::mat4 viewMatrix = minimapCameraEnabled ? minimapCam->GetViewMatrix() : camera->GetViewMatrix();
    glm::mat4 projection = minimapCameraEnabled ? minimapProjectionMatrix : projectionMatrix;
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    
    
	mesh->Render();

}



void Tema2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    shader->Use();
    glm::mat4 viewMatrix = minimapCameraEnabled ? minimapCam->GetViewMatrix() : camera->GetViewMatrix();
    glm::mat4 projection = minimapCameraEnabled ? minimapProjectionMatrix : projectionMatrix;
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

void Tema2::OnInputUpdate(float deltaTime, int mods)
{

    bool up = false;
	bool down = false;
    glm::vec3 inputDirection = glm::vec3(0);
    float acceleration = 4.5f;

    bool s_pressed = false;
	bool w_pressed = false;
    
    if (gameStarted) {
        if (window->KeyHold(GLFW_KEY_UP)) {
            up = true;
        }
        if (window->KeyHold(GLFW_KEY_DOWN)) {
            down = true;
        }

        if (window->KeyHold(GLFW_KEY_W)) {

            w_pressed = true;
            inputDirection += glm::vec3_up * acceleration * deltaTime / 1.25f;

       
            drone.velocity += inputDirection;
            if (glm::length(drone.velocity) > maxSpeed) {
                drone.velocity = glm::normalize(drone.velocity) * maxSpeed;
            }
            drone.propellerSpeed = 50;

        }
        else if (window->KeyHold(GLFW_KEY_S) && !blockS) {

            s_pressed = true;
            inputDirection += glm::vec3_up * acceleration * deltaTime / 2.f;

            drone.velocity -= inputDirection;
            if (glm::length(drone.velocity) > maxSpeed) {
                drone.velocity = glm::normalize(drone.velocity) * maxSpeed;
            }
            drone.propellerSpeed = 50;
        }

        if (blockS) {
            if (drone.propellerSpeed != 0.0)
            {
                if (drone.propellerSpeed > 10.f)
                    drone.propellerSpeed = 10.f;
                else {
                    drone.propellerSpeed -= 10 * deltaTime;
                    if (drone.propellerSpeed < 0.0)
                        drone.propellerSpeed = 0.0;
                }
            }
        }
        else if (!w_pressed && !s_pressed) {
            drone.propellerSpeed = 20.0;
        }



        if (up) {

            drone.velocity += drone.forward * deltaTime * acceleration;
            if (glm::length(drone.velocity) > maxSpeed) {
                drone.velocity = glm::normalize(drone.velocity) * maxSpeed;

            }


            drone.rotationOX += deltaTime;
            if (drone.rotationOX > RADIANS(15)) {
                drone.rotationOX = RADIANS(15);
            }

            drone.propellerSpeed = 50;

        }

        if (down) {
            drone.velocity -= drone.forward * deltaTime * acceleration;

            if (glm::length(drone.velocity) > maxSpeed) {
                drone.velocity = glm::normalize(drone.velocity) * maxSpeed;
            }

            drone.rotationOX -= deltaTime;
            if (drone.rotationOX < -RADIANS(15)) {
                drone.rotationOX = -RADIANS(15);
            }


            drone.propellerSpeed = 50;
            isGoingBackward = true;
            down = true;

        }
        else {
            limitBackwardSpeed = false;
            isGoingBackward = false;
        }



        if (window->KeyHold(GLFW_KEY_A)) {

            if (drone.rotationOZ < -RADIANS(15)) {
                drone.rotationOZ = -RADIANS(15);
            }

            drone.rotationOY += deltaTime;
            drone.propellerSpeed = 50;
            drone.velocity = glm::vec3(transform3d::RotateOY(deltaTime) * glm::vec4(drone.velocity, 1));

        }


        if (window->KeyHold(GLFW_KEY_D)) {


            if (drone.rotationOZ > RADIANS(15)) {
                drone.rotationOZ = RADIANS(15);
            }

            drone.rotationOY -= deltaTime;


            drone.velocity = glm::vec3(transform3d::RotateOY(-deltaTime) * glm::vec4(drone.velocity, 1));
            drone.propellerSpeed = 50;
        }

        if (window->KeyHold(GLFW_KEY_LEFT)) {
            if (drone.rotationOZ < -RADIANS(15)) {
                drone.rotationOZ = -RADIANS(15);
            }
            else {

                drone.rotationOZ -= deltaTime * 1.5;
            }

            glm::vec3 forward = glm::vec3(sin(drone.rotationOY), 0, cos(drone.rotationOY));
            glm::vec3 localLeft = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));

            inputDirection = localLeft * acceleration * deltaTime;

            drone.velocity -= inputDirection;


            if (glm::length(drone.velocity) > maxSpeed) {
                drone.velocity = glm::normalize(drone.velocity) * maxSpeed;
            }

            drone.propellerSpeed = 50;
            isGoingLeft = true;

        }
        else {
            isGoingLeft = false;
        }

        if (window->KeyHold(GLFW_KEY_RIGHT)) {
            if (drone.rotationOZ > RADIANS(15)) {
                drone.rotationOZ = RADIANS(15);
            }
            else {

                drone.rotationOZ += deltaTime * 1.5;
            }

            glm::vec3 forward = glm::vec3(sin(drone.rotationOY), 0, cos(drone.rotationOY));
            glm::vec3 localLeft = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));

            inputDirection = localLeft * acceleration * deltaTime;

            drone.velocity += inputDirection;


            if (glm::length(drone.velocity) > maxSpeed) {
                drone.velocity = glm::normalize(drone.velocity) * maxSpeed;
            }

            drone.propellerSpeed = 50;
            isGoingRight = true;
        }
        else {
            isGoingRight = false;
        }
    }

		float dampingFactor = 0.8;
        float velocityComponentInForwardDirection = glm::dot(drone.velocity, drone.forward);
        glm::vec3 velocityInForwardDirection = drone.forward * velocityComponentInForwardDirection;

		dampingFactor = pow(dampingFactor, deltaTime);
         
        drone.velocity.x *= dampingFactor ;
        drone.velocity.z *= dampingFactor;

		drone.velocity.y -= deltaTime * 2.0f;


        if (!s_pressed) {
            if (drone.velocity.y < -0.5f) {
				drone.velocity.y += deltaTime * 3.f;
            }
        }



	if (drone.rotationOX > 0.01f && ((!up && !down) || s_pressed)) {
        drone.rotationOX -= deltaTime / 3;
    }
	else if (drone.rotationOX < -0.01f && ((!up && !down) || s_pressed)) {
        drone.rotationOX += deltaTime / 3;
    }

}

void Tema2::OnKeyPress(int key, int mods)
{
	if (!gameStarted) {
        gameStarted = true;
		gameEnded = false;
        timer = 50.f;

		score = 0;
        int x, z;
        do {
            x = rand() % 100 - 50;
            z = rand() % 100 - 50;
        } while (!isPositionValid(x, z, usedPositions));

        packet.position = glm::vec3(x, map.getTerrainHeight(static_cast<float>(x), static_cast<float>(z)), z);

		packet.isCollected = false;

        }

}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
