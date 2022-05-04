#ifndef POINTLIGHT_EXAMPLE_H
#define POINTLIGHT_EXAMPLE_H
#include <ecore/ThreeDemoModel.h>
#include <ecore/ExampleUtil.h>
#include <three/helpers/PointLightHelper.h>
#include <three/helpers/CameraHelper.h>
class PointLightExample : public ThreeDemoClass {
public:
	AmbientLight::ptr ambientLight;
	PointLight::ptr pointLight;
	float rotationSpeed = 0.01f;
	float bouncingSpeed = 0.03f;
	PointLightHelper::ptr helper;
	CameraHelper::ptr shadowHelper;
	Mesh::ptr sphereLightMesh;
	float step;
	float invert = 1;
	float phase = 0;

	PointLightExample(int x, int y) : ThreeDemoClass(x, y) {
	}
	virtual void init() override {
		initRenderer(renderer);
		initCamera(camera,renderer);
		initTrackballControls(controller, camera, renderer);
		scene = std::make_shared<Scene>();

		addHouseAndTree(scene);

		ambientLight = AmbientLight::create(Color(0x0c0c0c));
		scene->add(ambientLight);

		pointLight = PointLight::create(Color(0xccffcc));
		pointLight->decay = 0.1f;
		pointLight->castShadow = true;

		scene->add(pointLight);

		helper = PointLightHelper::create(pointLight);

		shadowHelper = CameraHelper::create(pointLight->shadow->camera);

		auto sphereLight = SphereBufferGeometry::create(0.2f);
		auto sphereLightMaterial = MeshBasicMaterial::create(Color(0xac6c25));
		sphereLightMesh = Mesh::create(sphereLight, sphereLightMaterial);
		sphereLightMesh->position.set(3, 0, 5);
		scene->add(sphereLightMesh);
	}
	virtual void render() override {
		if (!initialized) return;
		if (phase > 2 * PI) {
			invert = invert * -1;
			phase -= 2 * PI;
		}
		else {
			phase += rotationSpeed;
		}
		sphereLightMesh->position.z = +(25 * (sin(phase)));
		sphereLightMesh->position.x = +(14 * (cos(phase)));
		sphereLightMesh->position.y = 5;

		if (invert < 0) {
			float pivot = 14;
			sphereLightMesh->position.x = (invert * (sphereLightMesh->position.x));
		}
		pointLight->position.copy(sphereLightMesh->position);
		helper->update();
		shadowHelper->update();
		ThreeDemoClass::render();
	}
	virtual void showControls() override {
		if (!initialized) return;
		ImGui::Begin("Controls");
		ImGui::ColorPicker3("AmbientColor", (float*)&ambientLight->color, 1);
		ImGui::ColorPicker3("PointColor", (float*)&pointLight->color, 0);
		ImGui::DragFloat("distance", &pointLight->distance, 0.1f, 0.0f,100.0f);
		ImGui::DragFloat("Intensity", &pointLight->intensity, 0.1f, 0.0f, 3.0f);		
		ImGui::End();
	}
};
#endif
