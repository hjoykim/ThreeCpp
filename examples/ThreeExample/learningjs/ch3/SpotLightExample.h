#ifndef SPOTLIGHT_EXAMPLE_H
#define SPOTLIGHT_EXAMPLE_H
#include <ecore/ThreeDemoModel.h>
#include <ecore/ExampleUtil.h>
#include <three/helpers/CameraHelper.h>
#include <three/helpers/SpotLightHelper.h>
class SpotLightExample : public ThreeDemoClass {
public:
	// for controlling the rendering
	float step = 0;
	float invert = 1;
	float phase = 0;

	Mesh::ptr plane;

	Mesh::ptr cube;

	Mesh::ptr sphere;

	Mesh::ptr sphereLightMesh;

	AmbientLight::ptr ambientLight;

	SpotLight::ptr spotLight;

	SpotLightHelper::ptr spotLightHelper;

	CameraHelper::ptr debugCamera;

	float rotationSpeed = 0.03f;
	float bouncingSpeed = 0.03f;
	bool stopMovingLight = false;
	
	SpotLightExample(int x, int y) : ThreeDemoClass(x, y) {
	}

	virtual void init() override {
		renderer->setClearColor(Color().set(0x000000));
		renderer->shadowMap->enabled = true;
		renderer->shadowMap->type = ShadowMapType::PCFSoftShadowMap;

		camera = std::make_shared<PerspectiveCamera>(45.0f, screenX / screenY, 0.1f, 1000.0f);
		camera->position.set(-30.0f, 40.0f, 30.0f);
		camera->lookAt(Vector3());

		Vector4 screen = Vector4(0, 0, screenX, screenY);
		controller = std::make_shared<TrackballControls>(camera, screen);

		controller->staticMoving = true;
		controller->rotateSpeed = 3.0f;
		controller->zoomSpeed = 2;
		controller->panSpeed = 2;
		controller->noZoom = false;
		controller->noPan = false;
		controller->noRotate = false;
		controller->dynamicDampingFactor = 0.2f;

		scene = std::make_shared<Scene>();

		auto [cube, sphere] = addDefaultCubeAndSphere(scene);
		this->cube = cube;
		this->sphere = sphere;

		plane = addGroundPlane(scene);

		plane->name = "Plane";

		ambientLight = AmbientLight::create(Color().setHex(0x1c1c1c));
		scene->add(ambientLight);

		// add spotlight for a bit of light
		auto spotLight0 = SpotLight::create(Color().set(0xcccccc));
		spotLight0->position.set(-40, 30, -10);
		spotLight0->lookAt(plane->position);
		scene->add(spotLight0);

		//// add target and light
		//auto target = std::make_shared<Object3D>();
		//target->position.set(5, 0, 0);

		spotLight = SpotLight::create(Color().set(0xffffff));
		spotLight->position.set(-40, 60, -10);
		spotLight->castShadow = true;
		spotLight->shadow->camera->_near = 1;
		spotLight->shadow->camera->_far = 100;
		spotLight->target = plane;
		spotLight->distance = 0;
		spotLight->angle = 0.4f;
		spotLight->shadow->camera->fov = 120;
		scene->add(spotLight);

		debugCamera = std::make_shared<CameraHelper>(spotLight->shadow->camera);
		scene->add(debugCamera);

		spotLightHelper = SpotLightHelper::create(spotLight,Color().set(0xffffff));

		scene->add(spotLightHelper);

		auto sphereLight = SphereGeometry::create(0.2f);
		auto sphereLightMaterial = MeshBasicMaterial::create(Color(0xac6c25));
		sphereLightMesh = Mesh::create(sphereLight, sphereLightMaterial);

		sphereLightMesh->castShadow = true;

		sphereLightMesh->position.set(3, 20, 3);

		scene->add(sphereLightMesh);	
	}

	virtual void render() override {
		if (!initialized) return;
		// rotate the cube around its axes
		cube->rotation.setX(cube->rotation._x + rotationSpeed);
		cube->rotation.setY(cube->rotation._y + rotationSpeed);
		cube->rotation.setZ(cube->rotation._z + rotationSpeed);

		// bounce the sphere up and down

		step += bouncingSpeed;

		sphere->position.x = 20 + (10 * (cos(step)));
		sphere->position.y = 2 + (10 * abs(sin(step)));

		if (!stopMovingLight) {
			if (phase > 2 * M_PI) {
				invert = invert * -1;
				phase -= 2 * M_PI;
			}
			else {
				phase += rotationSpeed;
			}

			sphereLightMesh->position.z = +(7 * (sin(phase)));
			sphereLightMesh->position.x = +(14 * (cos(phase)));
			sphereLightMesh->position.y = 15;

			if (invert < 0) {
				auto pivot = 14;
				sphereLightMesh->position.x = (invert * (sphereLightMesh->position.x - pivot)) + pivot;
			}

			spotLight->position.copy(sphereLightMesh->position);
		}
		
		spotLightHelper->update();
		debugCamera->update();
		ThreeDemoClass::render();
		
	}
	virtual void showControls() override {
		if (!initialized) return;
		static int target = 0;
		ImGui::Begin("Controls");
		ImGui::ColorPicker3("AmbientColor", (float*)&ambientLight->color, 1);
		ImGui::ColorPicker3("PointColor", (float*)&spotLight->color, 0);
		ImGui::DragFloat("angle",&spotLight->angle, 0.1f, 0.0f, (float)M_PI * 2.0f, "%.3f");
		ImGui::DragFloat("Intensity", &spotLight->intensity, 0.1f, 0.0f, 5.0f);
		ImGui::DragFloat("penumbra", &spotLight->penumbra, 0.1f, 0.0f, 1.0f);
		ImGui::DragFloat("distance", &spotLight->distance, 0.1f, 0.0f, 3.0f);
		ImGui::Checkbox("showDebug", &debugCamera->visible);
		
		ImGui::Checkbox("castShadow", &spotLight->castShadow);
		ImGui::Combo("target", &target, "Plane\0Sphere\0Cube\0");
		switch (target) {
		case 0:
			spotLight->target = plane;
			break;
		case 1:
			spotLight->target = sphere;
			break;
		case 2:
			spotLight->target = cube;
		}

		ImGui::Checkbox("stopMovingLight", &stopMovingLight);
		ImGui::End();
	}
};
#endif