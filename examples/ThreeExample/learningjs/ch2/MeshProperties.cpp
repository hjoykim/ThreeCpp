#include "MeshProperties.h"
namespace three {
	MeshProperties::MeshProperties(int x, int y) : ThreeDemoClass(x,y)
	{
		renderer->setClearColor(Color().setHex(0xEEEEEE), 1);
		renderer->shadowMap->enabled = true;

		isPerspective = true;

		camera = std::make_shared<PerspectiveCamera>(45.0f, screenX / screenY, 0.1f, 1000.0f);
		camera->position.set(-30.0f, 40.0f, 30.0f);
		camera->lookAt(Vector3(5, 0, 0));

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

		auto planeGeometry = std::make_shared<PlaneGeometry>(60, 40, 1, 1);
		auto planeMaterial = std::make_shared<MeshLambertMaterial>();
		planeMaterial->color = Color().setHex(0xffffff);

		auto plane = std::make_shared<Mesh>(planeGeometry, planeMaterial);
		plane->receiveShadow = true;

		plane->rotation.setX((float)(-0.5f * M_PI));
		plane->position.set(0, 0, 0);

		scene->add(plane);

		auto ambientLight = AmbientLight::create(Color().setHex(0x0c0c0c));
		scene->add(ambientLight);

		auto spotLight = SpotLight::create(Color().setHex(0xffffff));		
		spotLight->position.set(-40, 60, 20);
		spotLight->castShadow = true;
		scene->add(spotLight);

		auto material = std::make_shared<MeshLambertMaterial>(Color().setHex(0x44ff44));
		auto geom = std::make_shared<BoxGeometry>(5, 8, 3);
		cube = std::make_shared<Mesh>(geom, material);

		cube->position.y = 4;
		cube->castShadow = true;

		scene->add(cube);
		
	}

	void MeshProperties::init()
	{
	}

	void MeshProperties::render()
	{
		ThreeDemoClass::render();
	}

	void MeshProperties::showControls()
	{
		ImGui::Begin("Controls");
		if (ImGui::TreeNode("scale")) {
			ImGui::SliderFloat("scaleX", &cube->scale.x, 1, 5);
			ImGui::SliderFloat("scaleY", &cube->scale.y, 1, 5);
			ImGui::SliderFloat("scaleZ", &cube->scale.z, 1, 5);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("position")) {
			ImGui::SliderFloat("positionX", &cube->position.x, -10, 10);
			ImGui::SliderFloat("positionY", &cube->position.y, -4, 20);
			ImGui::SliderFloat("positionZ", &cube->position.z, -10, 10);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("rotation")) {
			ImGui::SliderFloat("rotationX", &rotationX, -4, 4);
			ImGui::SliderFloat("rotationY", &rotationY, -4, 4);
			ImGui::SliderFloat("rotationZ", &rotationZ, -4, 4);
			cube->rotation.setX(rotationX);
			cube->rotation.setY(rotationY);
			cube->rotation.setZ(rotationZ);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("translate")) {
			ImGui::SliderFloat("translateX", &translateX, -10, 10);
			ImGui::SliderFloat("translateY", &translateY, -10, 10);
			ImGui::SliderFloat("translateZ", &translateZ, -10, 10);
			if (ImGui::Button("Button")) {
				cube->translateX(translateX);
				cube->translateY(translateY);
				cube->translateZ(translateZ);

			}
			ImGui::TreePop();
			
		}
		ImGui::Checkbox("Visible", &cube->visible);
		ImGui::End();
	}
	
}
