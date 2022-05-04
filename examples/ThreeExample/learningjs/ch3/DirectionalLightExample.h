#ifndef DIRECTIONALLIGHT_EXAMPLE_H
#define DIRECTIONALLIGHT_EXAMPLE_H
#include <ecore/ThreeDemoModel.h>
#include <ecore/ExampleUtil.h>
#include <three/helpers/CameraHelper.h>
class DirectionalLightExample : public ThreeDemoClass {
public:
	DirectionalLight::ptr directionalLight;
	AmbientLight::ptr ambientLight;
	Mesh::ptr plane;
	Mesh::ptr cube;
	Mesh::ptr sphere;
	Mesh::ptr sphereLightMesh;
	CameraHelper::ptr shadowCamera;
	float step = 0;
	float invert = 1;
	float phase = 0;
	float rotationSpeed = 0.03f;
	float bouncingSpeed = 0.03f;

	DirectionalLightExample(int x, int y) :ThreeDemoClass(x, y) {}

	virtual void init() override {
		initRenderer(renderer);
		initCamera(camera, renderer);
		initTrackballControls(controller, camera, renderer);

		scene = std::make_shared<Scene>();

		auto planeGeometry = PlaneBufferGeometry::create(600, 200, 20, 20);
		auto planeMaterial = MeshLambertMaterial::create(Color(0xffffff));
		plane = Mesh::create(planeGeometry, planeMaterial);
		plane->receiveShadow = true;

		// rotate and position the plane
		plane->rotation.setX(-0.5f * PI);
		plane->position.x = 15;
		plane->position.y = -5;
		plane->position.z = 0;

		// add the plane to the scene
		scene->add(plane);

		// create a cube
		auto cubeGeometry = BoxBufferGeometry::create(4, 4, 4);
		auto cubeMaterial = MeshLambertMaterial::create(Color(0xff3333));
		cube = Mesh::create(cubeGeometry, cubeMaterial);
		cube->castShadow = true;

		// position the cube
		cube->position.x = -4;
		cube->position.y = 3;
		cube->position.z = 0;

		// add the cube to the scene
		scene->add(cube);

		auto sphereGeometry = SphereBufferGeometry::create(4, 20, 20);
		auto sphereMaterial = MeshLambertMaterial::create(Color(0x7777ff));
		sphere = Mesh::create(sphereGeometry, sphereMaterial);

		// position the sphere
		sphere->position.x = 20;
		sphere->position.y = 0;
		sphere->position.z = 2;
		sphere->castShadow = true;

		// add the sphere to the scene
		scene->add(sphere);

		ambientLight = AmbientLight::create(Color(0x1c1c1c));
		scene->add(ambientLight);

		directionalLight = DirectionalLight::create(Color(0xff5808));
		directionalLight->position.set(-40, 60, -10);
		directionalLight->castShadow = true;
		directionalLight->shadow->camera->_near = 2;
		directionalLight->shadow->camera->_far = 80;
		directionalLight->shadow->camera->left = -30;
		directionalLight->shadow->camera->cameraRight = 30;
		directionalLight->shadow->camera->top = 30;
		directionalLight->shadow->camera->bottom = -30;

		directionalLight->intensity = 0.5;
		directionalLight->shadow->shadowMapSize.set(1024,1024);

		scene->add(directionalLight);

		shadowCamera = CameraHelper::create(directionalLight->shadow->camera);
		shadowCamera->visible = false;
		scene->add(shadowCamera);
		// add a small sphere simulating the pointlight
		auto sphereLight = SphereBufferGeometry::create(0.2);
		auto sphereLightMaterial = MeshBasicMaterial::create(Color(0xac6c25));
		sphereLightMesh = Mesh::create(sphereLight, sphereLightMaterial);
		sphereLightMesh->castShadow = true;

		sphereLightMesh->position.set(3, 20, 3);
		scene->add(sphereLightMesh);
	}
	virtual void render() override {
		if (!initialized) return;
		// rotate the cube around its axes
		cube->rotation.setX(cube->rotation.getX() + rotationSpeed);
		cube->rotation.setY(cube->rotation.getY() + rotationSpeed);
		cube->rotation.setZ(cube->rotation.getZ() + rotationSpeed);

		// bounce the sphere up and down
		step += bouncingSpeed;
		sphere->position.x = 20 + (10 * (cos(step)));
		sphere->position.y = 2 + (10 * abs(sin(step)));

		sphereLightMesh->position.z = -8;
		sphereLightMesh->position.y = +(27 * (sin(step / 3)));
		sphereLightMesh->position.x = 10 + (26 * (cos(step / 3)));

		directionalLight->position.copy(sphereLightMesh->position);
		shadowCamera->update();
		ThreeDemoClass::render();
	}

	virtual void showControls() override {
		if (!initialized) return;
		static int target = 0;
		ImGui::Begin("Controls");
		ImGui::ColorPicker3("AmbientColor", (float*)&ambientLight->color, 1);
		ImGui::ColorPicker3("PointColor", (float*)&directionalLight->color, 0);
		ImGui::DragFloat("Intensity", &directionalLight->intensity, 0.1f, 0.0f, 5.0f);
		ImGui::Checkbox("debug", &shadowCamera->visible);
		ImGui::Combo("target", &target, "Plane\0Sphere\0Cube\0");
		switch (target) {
		case 0:
			directionalLight->target = plane;
			break;
		case 1:
			directionalLight->target = sphere;
			break;
		case 2:
			directionalLight->target = cube;
		}
		ImGui::End();
	}
};
#endif