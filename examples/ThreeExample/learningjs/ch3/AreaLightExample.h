#ifndef RECTAREALIGHT_EXAMPLE_H
#define RECTAREALIGHT_EXAMPLE_H
#include <ecore/ThreeDemoModel.h>
#include <ecore/ExampleUtil.h>
#include <three/loaders/TextureLoader.h>
#include <imgui/imgui.h>
#include <iostream>
#include <three/lights/Light.h>
class AreaLightExample : public ThreeDemoClass {
public:
	RectAreaLight::ptr areaLight1;
	RectAreaLight::ptr areaLight2;
	RectAreaLight::ptr areaLight3;

	Mesh::ptr plane;
	Mesh::ptr plane1;
	Mesh::ptr plane2;
	Mesh::ptr plane3;
	AreaLightExample(int x, int y) : ThreeDemoClass(x, y) {

	}
	virtual void init() override {
		initRenderer(renderer);
		initCamera(camera, renderer);
		initTrackballControls(controller, camera, renderer);

		scene = std::make_shared<Scene>();

		auto planeGeometry = PlaneBufferGeometry::create(70, 70, 1, 1);
		auto planeMaterial = MeshStandardMaterial::create();
		planeMaterial->roughness = 0.044676705160855;
		planeMaterial->metalness = 0.0f;

		plane = Mesh::create(planeGeometry, planeMaterial);
		// plane.receiveShadow  = true;

		// rotate and position the plane
		plane->rotation.setX(-0.5f * PI);
		plane->position.set(0, 0, 0);

		// add the plane to the scene
		scene->add(plane);

		// call the render function
		
		auto spotLight0 = SpotLight::create(Color(0xcccccc));
		spotLight0->position.set(-40, 60, -10);
		spotLight0->intensity = 0.1;
		spotLight0->lookAt(plane->position);
		scene->add(spotLight0);

		areaLight1 = RectAreaLight::create(Color(0xff0000), 500, 4, 10);
		areaLight1->position.set(-10, 10, -35);
		scene->add(areaLight1);

		areaLight2 = RectAreaLight::create(Color(0x00ff00), 500, 4, 10);
		areaLight2->position.set(0, 10, -35);
		scene->add(areaLight2);

		areaLight3 = RectAreaLight::create(Color(0x0000ff), 500, 4, 10);
		areaLight3->position.set(10, 10, -35);
		scene->add(areaLight3);

		auto planeGeometry1 = BoxBufferGeometry::create(4, 10, 0);
		auto planeGeometry1Mat = MeshBasicMaterial::create(Color(0xff0000));
		plane1 = Mesh::create(planeGeometry1, planeGeometry1Mat);
		plane1->position.copy(areaLight1->position);
		scene->add(plane1);

		auto planeGeometry2 = BoxBufferGeometry::create(4, 10, 0);
		auto planeGeometry2Mat = MeshBasicMaterial::create(Color(0x00ff00));
		plane2 = Mesh::create(planeGeometry2, planeGeometry2Mat);

		plane2->position.copy(areaLight2->position);
		scene->add(plane2);

		auto planeGeometry3 = BoxBufferGeometry::create(4, 10, 0);
		auto planeGeometry3Mat = MeshBasicMaterial::create(Color(0x0000ff));
		plane3 = Mesh::create(planeGeometry3, planeGeometry3Mat);

		plane3->position.copy(areaLight3->position);
		scene->add(plane3);
	}
	virtual void render() override {
		if (!initialized) return;
		ThreeDemoClass::render();
	}
	virtual void showControls() override {
		if (!initialized) return;
		ImGui::Begin("Controls");
		if (ImGui::ColorPicker3("color1", (float*)&areaLight1->color, 0)) {
			plane1->material->color = areaLight1->color;
		}
		ImGui::DragFloat("intensity1", &areaLight1->intensity, 0.1f, 0.0f, 1000.0f);

		if (ImGui::ColorPicker3("color2", (float*)&areaLight2->color, 0)) {
			plane2->material->color = areaLight2->color;
		}
		ImGui::DragFloat("intensity2", &areaLight2->intensity, 0.1f, 0.0f, 1000.0f);

		if (ImGui::ColorPicker3("color3", (float*)&areaLight3->color, 0)) {
			plane3->material->color = areaLight3->color;
		}
		ImGui::DragFloat("intensity3", &areaLight3->intensity, 0.1f, 0.0f, 1000.0f);

		ImGui::End();

	}
};
#endif
