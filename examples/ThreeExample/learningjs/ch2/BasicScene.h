#ifndef BASIC_SCENE_H
#define BASIC_SCENE_H
#include <ecore/ThreeDemoModel.h>
#include <three/objects/Mesh.h>
#include <three/lights/Light.h>
#include <memory>
#include <imgui/imgui.h>
#include <vector>

class BasicScene : public ThreeDemoClass {
public :
	float step = 0.0f;
	Mesh::ptr plane;
	PlaneGeometry::ptr planeGeometry;
	BasicScene(int x, int y) : ThreeDemoClass(x, y) {
		isPerspective = true;
		renderer->setClearColor(Color().setHex(0xEEEEEE));
		renderer->shadowMap->enabled = true;

		camera = std::make_shared<PerspectiveCamera>(45.0f, screenX / screenY, 0.1f, 1000.0f);
		camera->position.set(-30.0f, 40.00f, 30.0f);
		camera->lookAt(Vector3());


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

		scene = std::make_shared<Scene>();
		scene->setBackgroundColor(Color().setHex(0xffffff));

		planeGeometry = std::make_shared<PlaneGeometry>(60, 40,1,1);
		auto planeMaterial = std::make_shared<MeshPhongMaterial>();
		planeMaterial->color = Color().setHex(0xffffff);
		plane = std::make_shared<Mesh>(planeGeometry, planeMaterial);
		plane->receiveShadow = true;
		plane->rotation.setX((float)(-0.5f * M_PI));
		plane->position.set(0, 0, 0);

		scene->add(plane);

		auto ambientLight = AmbientLight::create(Color().setHex(0x0c0c0c));
		scene->add(ambientLight);

		auto spotLight = SpotLight::create(Color().setHex(0xffffff));
		spotLight->position.set(-40, 60, -10);
		spotLight->castShadow = true;
		scene->add(spotLight);


		
	}
	void addCube(); //{
	//	auto cubeSize = ceil(random() * 3);
	//	auto cubeGeometry = std::make_shared<BoxGeometry>(cubeSize, cubeSize, cubeSize);
	//	auto cubeMaterial = std::make_shared<MeshPhongMaterial>();
	//	cubeMaterial->color = std::make_shared<Color>(Color((unsigned)random()*0xffffff));
	//	auto cube = std::make_shared<Mesh>(cubeGeometry, cubeMaterial);
	//	cube->name = "cube-" + std::to_string(scene->children.size());
	//	//cube->position.x = -30 + round(random() * planeGeometry->parameters.get<float>("width"));
	//	cube->position.y = round(random() * 5);
	//	//cube->position.z = -20 + round(random() * planeGeometry->parameters.get<float>("height"));
	//	cube->castShadow = true;
	//	scene->add(cube);
	//}
	void removeCube();/* {
		if (scene->children.size() == 0) return;
		auto lastObject = scene->children[scene->children.size() - 1];
		if (instanceOf<Mesh>(lastObject.get())) {
			scene->remove(lastObject);
			numberOfObjects = scene->children.size();
			scene->children.shrink_to_fit();
		}
	}*/
	virtual void render() override;
	
	virtual void showControls() override;/* {
		ImGui::Begin("Controls");
		ImGui::Text("This is Rotaion Speed Control box");
		ImGui::SliderFloat("RotationSpeed", &rotationSpeed, 0.0f, 0.5f);
		if (ImGui::Button("Add Cube")) {
			addCube();
		}
		if (ImGui::Button("Remove Cube")) {
			removeCube();
		}
		ImGui::Text("Number of Objects : %d", numberOfObjects);
		ImGui::End();
	}*/
};

#endif