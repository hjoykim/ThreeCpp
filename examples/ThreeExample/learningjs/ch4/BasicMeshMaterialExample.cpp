#include "BasicMeshMaterialExample.h"


void BasicMeshMaterialExample::loadGopher(const Material::ptr& material, std::string path, Group::ptr& gopher) {
	OBJLoader loader;

	gopher = loader.load(getProgramPath() + "\\" + path);

	
	gopher->traverse([&](Object3D& o) {
		o.material = meshMaterial;
		if (instanceOf<Mesh>(&o) && o.materials.size() > 1) {
			int size = o.materials.size();
			for (int i = 0; i < size; i++)
				o.materials.push_back(meshMaterial);
		}
	});
	gopher->scale.set(4, 4, 4);
}

void BasicMeshMaterialExample::init()
{
	renderer->setClearColor(Color().set(0x000000));
	renderer->shadowMap->enabled = true;
	renderer->shadowMap->type = ShadowMapType::PCFSoftShadowMap;

	camera = std::make_shared<PerspectiveCamera>(45.0f, screenX / screenY, 0.1f, 1000.0f);
	camera->position.set(-20.0f, 50.0f, 40.0f);
	camera->lookAt(Vector3(10, 0, 0));

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

	auto groundGeom = PlaneBufferGeometry::create(100, 100, 4, 4);
	auto groundMaterial = MeshBasicMaterial::create();
	Color color(0x777777);
	groundMaterial->color = color;

	auto groundMesh = Mesh::create(groundGeom, groundMaterial);

	groundMesh->rotation.setX(-PI / 2);
	groundMesh->position.y = -20;
	scene->add(groundMesh);

	auto sphereGeometry = SphereBufferGeometry::create(14, 20, 20);
	auto cubeGeometry = BoxBufferGeometry::create(15, 15, 15);
	auto planeGeometry = PlaneBufferGeometry::create(14, 14, 4, 4);


	meshMaterial = MeshBasicMaterial::create();
	meshMaterial->color.set(0x7777ff);
	meshMaterial->name = "Basic Material";
	meshMaterial->flatShading = true;
	meshMaterial->opacity = 0.01f;
	meshMaterial->fog = true;
	meshMaterial->colorWrite = true;

	thread thread1([&](const string& filepath) 
		{
			OBJLoader loader;
			gopher = loader.load(getProgramPath() + "\\" + filepath);
			gopher->traverse([&](Object3D& o) {
				o.material = meshMaterial;
				if (instanceOf<Mesh>(&o) && o.materials.size() > 1) {
					int size = o.materials.size();
					for (int i = 0; i < size; i++)
						o.materials.push_back(meshMaterial);
				}
				});
			gopher->scale.set(4, 4, 4);
		}
		,string("gopher.obj"));
	thread1.join();
	sphere = Mesh::create(sphereGeometry, meshMaterial);
	cube = Mesh::create(cubeGeometry, meshMaterial);
	plane = Mesh::create(planeGeometry, meshMaterial);

	// position the sphere
	sphere->position.x = 0;
	sphere->position.y = 3;
	sphere->position.z = 2;


	cube->position.copy(sphere->position);
	plane->position.copy(sphere->position);

	scene->add(cube);
	selectedMesh = cube;

	// add subtle ambient lighting
	auto ambientLight = AmbientLight::create(Color(0x0c0c0c));
	scene->add(ambientLight);

	// add spotlight for the shadows
	auto spotLight = SpotLight::create(Color(0xffffff));
	spotLight->position.set(-40, 60, -10);
	spotLight->castShadow = true;
	scene->add(spotLight);
}
void BasicMeshMaterialExample::render()
{
	step += 0.01f;
	selectedMesh->rotation.setY(step);
	ThreeDemoClass::render();
}

void BasicMeshMaterialExample::showControls()
{
	
	ImGui::Begin("Controls");
	if (ImGui::TreeNode("Three.Material")) {
		
		ImGui::Text("id : %d",meshMaterial->id);
		ImGui::Text("uuid :%s",meshMaterial->uuid.str().c_str());
		ImGui::Text("name :%s", meshMaterial->name.c_str());
		ImGui::SliderFloat("opacity", &meshMaterial->opacity, 0.0f, 1.0f);
		ImGui::Checkbox("transparent", &meshMaterial->transparent);
		
		ImGui::Checkbox("visible", &meshMaterial->visible);
		if (ImGui::Combo("side", &side, "FrontSide\0BackSide\0BothSide\0\0")) {
			meshMaterial->side = (Side)side;
		}
		ImGui::Checkbox("colorWrite", &meshMaterial->colorWrite);
		if (ImGui::Checkbox("flatShading", &meshMaterial->flatShading)) {
			meshMaterial->needsUpdate = true;
		}
		ImGui::Checkbox("premultipliedAlpha", &meshMaterial->premultipliedAlpha);
		ImGui::Checkbox("dithering", &meshMaterial->dithering);
		if (ImGui::Combo("shadowside", &side, "FrontSide\0BackSide\0BothSide\0\0")) {
			meshMaterial->shadowSide = (Side)side;
		}
		if (ImGui::Combo("vertexColors", &vertexColors, "NoColors\0FaceColors\0VertexColors\0\0")) {
			meshMaterial->vertexColors = (bool)vertexColors;
		}
		ImGui::Checkbox("fog", &meshMaterial->fog);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Three.MeshBasicMaterial")) {
		
		ImGui::ColorPicker3("color", (float*)&meshMaterial->color,0);
		ImGui::Checkbox("wireframe", &meshMaterial->wireframe);
		ImGui::SliderFloat("wireframeLineWidth", &meshMaterial->wireframeLinewidth, 0.0f, 20.0f);
		if (ImGui::Combo("wireframeLineJoin", &join, "round\0bevel\0miter\0\0")) {
			switch (join) {
			case 0: meshMaterial->wireframeLinejoin = "round"; break;
			case 1: meshMaterial->wireframeLinejoin = "bevel"; break;
			case 2: meshMaterial->wireframeLinejoin = "miter"; break;
			}
		}
		if (ImGui::Combo("wireframeLineCap", &lineCap, "butt\0round\0square\0\0")) {
			switch (lineCap) {
			case 0: meshMaterial->wireframeLinejoin = "butt"; break;
			case 1: meshMaterial->wireframeLinejoin = "round"; break;
			case 2: meshMaterial->wireframeLinejoin = "square"; break;
			}
		}		
		if (ImGui::Combo("selectedMesh", &selected, "cube\0sphre\0plane\0gopher\0\0")) {
			scene->remove(plane);
			scene->remove(cube);
			scene->remove(sphere);
			scene->remove(gopher);
			switch (selected) {
			case 0:scene->add(cube); selectedMesh = cube; break;
			case 1:scene->add(sphere); selectedMesh = sphere; break;
			case 2:scene->add(plane); selectedMesh = plane; break;
			case 3:scene->add(gopher); selectedMesh = gopher; break;
			}
		}
	
		ImGui::TreePop();
	}
	ImGui::End();
}

void BasicMeshMaterialExample::setMaterialGroup(Object3D& o)

{
	o.material = meshMaterial;
	if (instanceOf<Mesh>(&o) && o.materials.size() > 0) {
		for (int i = 0; i < o.materials.size(); i++)
			o.materials.push_back(meshMaterial);
	}
}

