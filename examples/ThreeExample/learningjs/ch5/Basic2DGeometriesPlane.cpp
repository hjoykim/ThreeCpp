#include "Basic2DGeometriesPlane.h"

Basic2DGeometriesPlane::Basic2DGeometriesPlane(int x, int y) : ThreeDemoClass(x, y) {
	scene = std::make_shared<Scene>();
}
void Basic2DGeometriesPlane::initRenderer() {
	renderer->setClearColor(Color().set(0x000000));
	renderer->shadowMap->enabled = true;
	renderer->shadowMap->type = ShadowMapType::PCFSoftShadowMap;
}
void Basic2DGeometriesPlane::initCamera() {
	camera = std::make_shared<PerspectiveCamera>(45.0f, screenX / screenY, 0.1f, 1000.0f);
	camera->position.set(-20.0f, 50.0f, 40.0f);
	camera->lookAt(Vector3(10, 0, 0));
}
void Basic2DGeometriesPlane::initCameraController() {
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
}
void Basic2DGeometriesPlane::init() {
	initRenderer();
	initCamera();
	initCameraController();

	buildMesh();

}
void Basic2DGeometriesPlane::initLighting() {
	AmbientLight::ptr ambientLight = AmbientLight::create(Color().setHex(0x343434));
	scene->add(ambientLight);

	SpotLight::ptr spotLight = SpotLight::create(Color().setHex(0xffffff));
	spotLight->position.set(-10, 30, 40);
	spotLight->shadow->shadowMapSize.x = 2048;
	spotLight->shadow->shadowMapSize.y = 2048;
	spotLight->castShadow = true;	
	spotLight->decay = 2;
	spotLight->penumbra = 0.05f;
	spotLight->shadow->camera->fov = 15;
	scene->add(spotLight);

}
void Basic2DGeometriesPlane::buildMesh() {
	groundPlane = addLargeGroundPlane(scene);
	groundPlane->position.y = -10;

	initLighting();

	appliedMesh = applyMeshNormalMaterial(buildGeometry(), appliedNormalMaterial);
	appliedMesh->castShadow = true;

	scene->add(appliedMesh);	

	

}
void Basic2DGeometriesPlane::rebuildGeometry()
{
}
void Basic2DGeometriesPlane::redraw()
{
}
BufferGeometry::ptr Basic2DGeometriesPlane::buildGeometry() {
	return PlaneBufferGeometry::create(width, height, widthSegment, heightSegment);
}

void Basic2DGeometriesPlane::render()
{
	step += 0.01f;
	appliedMesh->rotation.setX(step);
	appliedMesh->rotation.setY(step);
	appliedMesh->rotation.setZ(step);

	ThreeDemoClass::render();

	showControls();
}

void Basic2DGeometriesPlane::showControls()
{
}

void Basic2DGeometriesPlane::addCastShadow()
{
}

void Basic2DGeometriesPlane::addGroundPlaneVisible()
{
}

void Basic2DGeometriesPlane::addGeometryParameter()
{
}

void Basic2DGeometriesPlane::addGeometrySettings()
{
}

void Basic2DGeometriesPlane::addBasicMaterialSettings(Material::ptr& material, string name)
{
}

void Basic2DGeometriesPlane::addColorPicker(Material::ptr& material)
{
}

void Basic2DGeometriesPlane::addEmissivePicker(Material::ptr& material)
{
}

void Basic2DGeometriesPlane::addSpecularPicker(Material::ptr& material)
{
}

void Basic2DGeometriesPlane::addRoughness(Material::ptr& material)
{
}

void Basic2DGeometriesPlane::addMetalness(Material::ptr& material)
{
}

void Basic2DGeometriesPlane::addWireframeLineProperty(Material::ptr& material)
{
}

void Basic2DGeometriesPlane::addStandardMaterialSettings(Material::ptr& material)
{
}

void Basic2DGeometriesPlane::addNormalMaterialSettings(Material::ptr* material)
{
}
