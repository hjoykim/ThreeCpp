#include "FoggyScene.h"
namespace foggyscene {
	float rotationSpeed = 0.02f;
	int numberOfObjects = 0;



	void rotateChildrenCallback(Object3D& e) {
		if (instanceOf<Mesh>(&e)) {
			if (!instanceOf<PlaneGeometry>(e.geometry.get())) {
				e.rotation.setX(e.rotation.getX() + rotationSpeed);
				e.rotation.setY(e.rotation.getY() + rotationSpeed);
				e.rotation.setZ(e.rotation.getZ() + rotationSpeed);
			}
		}
	}
}
using namespace foggyscene;
void FoggyScene::addCube()
{
	auto cubeSize = ceil(random() * 3);
	auto cubeGeometry = std::make_shared<BoxGeometry>(cubeSize, cubeSize, cubeSize);
	auto cubeMaterial = std::make_shared<MeshPhongMaterial>();
	cubeMaterial->color = Color(random(0, 0xffffff));

	auto cube = std::make_shared<Mesh>(cubeGeometry, cubeMaterial);
	cube->name = "cube-" + std::to_string(scene->children.size());
	cube->position.x = -30 + round(random() * planeGeometry->parameters.width);
	cube->position.y = round(random() * 5);
	cube->position.z = -20 + round(random() * planeGeometry->parameters.height);
	cube->castShadow = true;
	scene->add(cube);
}

void FoggyScene::removeCube()
{
	if (scene->children.size() == 0) return;
	auto lastObject = scene->children[scene->children.size() - 1];
	if (instanceOf<Mesh>(lastObject.get())) {
		if (!instanceOf<PlaneGeometry>(lastObject->geometry.get())) {
			scene->remove(lastObject);
			numberOfObjects = scene->children.size();

		}
	}
}

void FoggyScene::render()
{
	numberOfObjects = scene->children.size();
	scene->traverse(&rotateChildrenCallback);
	ThreeDemoClass::render();
}

void FoggyScene::showControls()
{
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
}
