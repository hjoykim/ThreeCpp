#include "Geometries.h"
#include <three/geometries/CylinderGeometry.h>
#include <three/geometries/IcosahedronGeometry.h>
#include <three/geometries/ConvexGeometry.h>
#include <three/scenes/SceneUtils.h>
#include <three/geometries/LatheGeometry.h>
#include <three/geometries/OctahedronGeometry.h>
#include <three/geometries/ParametricGeometry.h>
#include <three/geometries/TetrahedronGeometry.h>
#include <three/geometries/TorusGeometry.h>
#include <three/geometries/TorusKnotGeometry.h>
void castShadowTraverse(Object3D& e)
{
	e.castShadow = true;
}
Vector3& mobius3d(float u, float t, Vector3& optionalTarget) {
	
	// volumetric mobius strip

	u *= (float)M_PI;
	t *= 2 * (float)M_PI;

	u = u * 2;
	auto phi = u / 2;
	float major = 2.25f, a = 0.125f, b = 0.65f;

	float x, y, z;

	x = (float)(a * cos(t) * cos(phi) - b * sin(t) * sin(phi));
	z = (float)(a * cos(t) * sin(phi) + b * sin(t) * cos(phi));
	y = (major + x) * (float)sin(u);
	x = (major + x) * (float)cos(u);

	optionalTarget.set(x, y, z);
	return optionalTarget;
}
three::Geometries::Geometries(int x, int y) : ThreeDemoClass(x, y)
{

	renderer->setClearColor(Color().setHex(0xEEEEEE));
	renderer->shadowMap->enabled = true;

	isPerspective = true;
	camera = std::make_shared<PerspectiveCamera>(45.0f, screenX / screenY, 0.1f, 1000.0f);
	camera->position.set(-50.0f, 30.00f, 20.0f);
	camera->lookAt(Vector3(-10, 0, 0));

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

	planeGeometry = std::make_shared<PlaneGeometry>(60, 40, 1, 1);
	auto planeMaterial = std::make_shared<MeshLambertMaterial>();
	planeMaterial->color = Color().setHex(0xffffff);
	//planeMaterial->side = Side::DoubleSide;
	plane = std::make_shared<Mesh>(planeGeometry, planeMaterial);
	plane->receiveShadow = true;
	plane->rotation.setX((float)(-0.5f * M_PI));
	plane->position.set(0, 0, 0);

	scene->add(plane);

	auto ambientLight = AmbientLight::create(Color().setHex(0x555555));
	scene->add(ambientLight);

	auto spotLight = SpotLight::create(Color().setHex(0xffffff));
	spotLight->shadow->shadowMapSize.set(1024, 1024);
	spotLight->intensity = 1.2f;
	spotLight->distance = 150.0f;
	spotLight->angle = (float)M_PI / 4;
	spotLight->penumbra = 0.0f;
	spotLight->decay = 2;
	spotLight->position.set(-40, 30, 30);
	spotLight->castShadow = true;
	scene->add(spotLight);

	addGeometries();

}

void three::Geometries::addGeometries()
{
	std::vector<Geometry::ptr> geoms;

	auto cylinderGeometry = std::make_shared<CylinderGeometry>(0, 4, 4);
	auto boxGeometry = std::make_shared<BoxGeometry>(2, 2, 2);
	auto sphereGeometry = std::make_shared<SphereGeometry>(2);
	auto icosahedronBufferGeometry = std::make_shared<IcosahedronGeometry>(4);
	geoms.push_back(cylinderGeometry);
	geoms.push_back(boxGeometry);
	geoms.push_back(sphereGeometry);
	geoms.push_back(icosahedronBufferGeometry);

	std::vector<Vector3> points =
	{
		Vector3(2, 2, 2),
		Vector3(2, 2, -2),
		Vector3(-2, 2, -2),
		Vector3(-2, 2, 2),
		Vector3(2, -2, 2),
		Vector3(2, -2, -2),
		Vector3(-2, -2, -2),
		Vector3(-2, -2, 2)
	};

	auto convexGeometry = std::make_shared<ConvexGeometry>(points);

	geoms.push_back(convexGeometry);



	std::vector<Vector3> pts;
	float detail = 0.1f;
	float radius = 3;

	for (float angle = 0.0f; angle < M_PI; angle += detail)
	{
		pts.push_back(Vector3((float)std::cos(angle) * radius, 0, (float)std::sin(angle) * radius));
	}

	auto latheGeometry = std::make_shared<LatheGeometry>(pts, 12);
	auto octahedronGeometry = std::make_shared<OctahedronGeometry>(3);

	geoms.push_back(latheGeometry);
	geoms.push_back(octahedronGeometry);

	auto parametricGeometry = std::make_shared<ParametricGeometry>(&mobius3d, 20, 10);

	geoms.push_back(parametricGeometry);

	auto tetrahedronGeometry = std::make_shared<TetrahedronGeometry>(3);

	geoms.push_back(tetrahedronGeometry);

	auto torusGeometry = std::make_shared<TorusGeometry>(3, 1, 10, 10);

	geoms.push_back(torusGeometry);

	auto torusKnotGeometry = std::make_shared<TorusKnotGeometry>(3, 0.5f, 50, 20);

	geoms.push_back(torusKnotGeometry);

	auto j = 0;

	for (int i = 0; i < geoms.size(); i++)
	{
		std::vector<Material::ptr> materials;
		auto lambert = std::make_shared<MeshLambertMaterial>();
		lambert->color = Color(random(0,0xffffff));
		lambert->flatShading = true;
		//lambert->side = Side::DoubleSide;
		auto basic = std::make_shared<MeshBasicMaterial>();
		basic->color = Color(0x000000);
		basic->wireframe = true;	
		//basic->side = Side::DoubleSide;
		
		materials.push_back(lambert);
		materials.push_back(basic);
		auto mesh = SceneUtils::createMultiMaterialObject(geoms[i], materials);

		mesh->traverse(&castShadowTraverse);
		mesh->position.x = -24 + ((i % 4) * 12);
		mesh->position.y = 4;
		mesh->position.z = -8 + (j * 12);

		if ((i + 1) % 4 == 0) j++;

		scene->add(mesh);

	}
}

void three::Geometries::render()
{
	ThreeDemoClass::render();
}

