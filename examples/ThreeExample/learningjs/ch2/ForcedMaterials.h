#ifndef FORCED_MATERIALS_H
#define FORCED_MATERIALS_H
#include <ecore/ThreeDemoModel.h>
#include <three/objects/Mesh.h>
#include <three/lights/Light.h>
#include <memory>
#include <imgui/imgui.h>
#include <vector>

class ForcedMaterials : public ThreeDemoClass {
public:
	float step = 0.0f;
	Mesh::ptr plane;
	PlaneGeometry::ptr planeGeometry;

	ForcedMaterials(int x, int y) : ThreeDemoClass(x, y) {
		isPerspective = true;
		renderer->setClearColor(Color().setHex(0xEEEEEE));
		renderer->shadowMap->enabled = true;

		camera = std::make_shared<PerspectiveCamera>(45.0f, screenX / screenY, 0.1f, 1000.0f);
		camera->position.set(-30.0f, 40.00f, 30.0f);
		camera->lookAt(Vector3());


		Vector4 screen = Vector4(0, 0, screenX, screenY);
		controller = std::make_shared<TrackballControls>(camera, screen);

		controller->staticMoving = false;
		controller->rotateSpeed = 4.0f;
		controller->zoomSpeed = 3;
		controller->panSpeed = 3;
		controller->noZoom = false;
		controller->noPan = false;
		controller->noRotate = false;
		controller->dynamicDampingFactor = 0.3f;

		scene = std::make_shared<Scene>();
		scene->setBackgroundColor(Color().setHex(0xffffff));

		scene->overrideMaterial = std::make_shared<MeshLambertMaterial>();
		scene->overrideMaterial->color = Color(0xffffff);

		planeGeometry = std::make_shared<PlaneGeometry>(60, 40, 1, 1);
		auto planeMaterial = std::make_shared<MeshLambertMaterial>();
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
	void addCube();

	void removeCube();

	virtual void render() override;

	virtual void showControls() override;
};
#endif