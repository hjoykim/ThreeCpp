#include "AmbientLightExample.h"

AmbientLightExample::AmbientLightExample(int x, int y) : ThreeDemoClass(x, y) {
}
void AmbientLightExample::init() {
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

	ambientLight = AmbientLight::create(Color().setHex(0x606008));
	scene->add(ambientLight);
	iColor.x = ambientLight->color.r;
	iColor.y = ambientLight->color.g;
	iColor.z = ambientLight->color.b;
	iColor.w = 1.0f;

	spotLight0 = SpotLight::create(Color().set(0xcccccc));
	spotLight0->position.set(-40, 30, -10);
	spotLight0->lookAt(scene->position);
	scene->add(spotLight0);

	addHouseAndTree(scene);
}
void AmbientLightExample::render()
{
	if (!initialized) return;
	ThreeDemoClass::render();
}

void AmbientLightExample::showControls()
{
	if (!initialized) return;
	ImGui::Begin("Controls");

	ImGui::DragFloat("Intensity", &ambientLight->intensity, 0.1f, 0.0f, 3.0f);	
	ImGui::Text("Ambient Color");
	ImGui::ColorPicker3("AmbientColor", (float*)&ambientLight->color, 0);
	ImGui::Checkbox("Disable SpotLight", &spotLight0->visible);
	ImGui::End();
}
