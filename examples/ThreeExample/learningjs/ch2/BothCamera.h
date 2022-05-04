#ifndef BOTH_CAMERA_H
#define BOTH_CAMERA_H
#include <ecore/ThreeDemoModel.h>
#include <three/objects/Mesh.h>
#include <three/objects/Group.h>
#include <three/lights/Light.h>
#include <memory>
#include <imgui/imgui.h>
#include <vector>
#include <three/math/Vector3.h>
#include <three\utils\Timer.h>
#include <iostream>
#include <three/utils/StopWatch.h>
namespace three {
	class BothCameraExample : public ThreeDemoClass {
	public :
		BothCameraExample(int x, int y) :ThreeDemoClass(x, y) {
			renderer->setClearColor(Color().setHex(0xEEEEEE), 1);
			isPerspective = true;

			scene = std::make_shared<Scene>();
			scene->setBackgroundColor(Color().setHex(0xffffff));

			pcamera = std::make_shared<PerspectiveCamera>(45.0f, screenX / screenY, 0.1f, 1000.0f);
			pcamera->position.set(120.0f, 60.0f, 180.0f);
			pcamera->lookAt(scene->position);

			camera = std::dynamic_pointer_cast<Camera>(pcamera);
			Vector4 screen = Vector4(0, 0, screenX, screenY);
			controller = std::make_shared<TrackballControls>(camera, screen);

			controller->staticMoving = true;
			controller->rotateSpeed = 4.0f;
			controller->zoomSpeed = 3;
			controller->panSpeed = 3;
			controller->noZoom = false;
			controller->noPan = false;
			controller->noRotate = false;
			controller->dynamicDampingFactor = 0.3f;

			auto planeGeometry = std::make_shared<PlaneBufferGeometry>(180,180);
			auto planeMaterial = std::make_shared<MeshLambertMaterial>();
			planeMaterial->color = Color().setHex(0xffffff);
			auto plane = std::make_shared<Mesh>(planeGeometry, planeMaterial);
			plane->rotation.setX((float)(-0.5f * M_PI));
			plane->position.set(0, 0, 0);
			scene->add(plane);

			auto cubeGeometry = std::make_shared<BoxBufferGeometry>(4, 4, 4);

			float height = planeGeometry->parameters.height;
			float width = planeGeometry->parameters.width;
			
			int cubeSize = (height / 5) * (width / 5);		
			scene->children.reserve(cubeSize);
			for (auto j = 0; j < (height / 5); j++) {
				for (auto i = 0; i < width / 5; i++) {
					auto rnd = randInt(0, 255);
					auto cubeMaterial = std::make_shared<MeshLambertMaterial>();
					cubeMaterial->color = Color(rnd /255.0f, 0, 0);
					auto cube = Mesh::create(cubeGeometry, cubeMaterial);

					cube->position.z = -((height) / 2) + 2 + (j * 5);
					cube->position.x = -((width) / 2) + 2 + (i * 5);
					cube->position.y = 2;
					
					
					scene->add(cube);
				}
			}


			auto ambientLight = AmbientLight::create(Color().setHex(0x292929));
			scene->add(ambientLight);

			auto directionalLight = DirectionalLight::create(Color().setHex(0xffffff),0.7f);
			directionalLight->position.set(-20, 40, 60);
			scene->add(directionalLight);
		}
		void switchCamera() {
			Vector4 screen = Vector4(0,0,screenX, screenY);
			if (isPerspective) {				
				pcamera->position.set(120.0f, 60.0f, 180.0f);
				controller->initTrackballControls(pcamera, screen);
				controller->camera->lookAt(scene->position);

				camera = std::dynamic_pointer_cast<Camera>(pcamera);
			}
			else {
				if (ocamera == nullptr)
					ocamera = std::make_shared<OrthographicCamera>(screenX / -16, screenX / 16, screenY / 16, screenY / -16, -200, 500);

				ocamera->position.set(120.0f, 60.0f, 180.0f);
				controller->initTrackballControls(ocamera, screen);
				controller->camera->lookAt(scene->position);
				camera = std::dynamic_pointer_cast<Camera>(ocamera);
			}
		}
		
		virtual void init() override {

		}
		virtual void render() override{
			if (isPerspective)
				renderer->render(scene, pcamera);
			else
				renderer->render(scene, ocamera);
		}
		virtual void showControls() override {
			ImGui::Begin("Controls");
			ImGui::Text("SwitchCamera");
			
			if (ImGui::Button("Perspective")) {
				isPerspective = true;
				switchCamera();				
			}
			if (ImGui::Button("Orthographic")) {
				isPerspective = false;
				switchCamera();
			}			
			ImGui::End();
		}
	};
}
#endif