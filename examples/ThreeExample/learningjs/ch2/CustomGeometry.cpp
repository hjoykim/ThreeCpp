#include "CustomGeometry.h"
#include <three/scenes/SceneUtils.h>
namespace three {
	void CustomGeometry::createMesh()
	{
		std::vector<Vector3> vertices = {
				   Vector3(1, 3, 1),
				   Vector3(1, 3, -1),
				   Vector3(1, -1, 1),
				   Vector3(1, -1, -1),
				   Vector3(-1, 3, -1),
				   Vector3(-1, 3, 1),
				   Vector3(-1, -1, -1),
				   Vector3(-1, -1, 1)
		};

		std::vector<Face3> faces = {
				 Face3(0, 2, 1),
				 Face3(2, 3, 1),
				 Face3(4, 6, 5),
				 Face3(6, 7, 5),
				 Face3(4, 5, 1),
				 Face3(5, 0, 1),
				 Face3(7, 6, 2),
				 Face3(6, 3, 2),
				 Face3(5, 7, 0),
				 Face3(7, 2, 0),
				 Face3(1, 3, 4),
				 Face3(3, 6, 4),

		};

		v = vertices;

		auto geom = std::make_shared<Geometry>();

		geom->vertices = vertices;
		geom->faces = faces;
		//std::copy(vertices.begin(), vertices.end(), geom->vertices.begin());
		//std::copy(faces.begin(), faces.end(), geom->faces.begin());

		geom->computeFaceNormals();

		std::vector<Material::ptr> materials;

		auto basicMaterial = std::make_shared<MeshBasicMaterial>();
		basicMaterial->color = Color(ColorKeywords::black);
		basicMaterial->wireframe = true;

		auto lambert = std::make_shared<MeshLambertMaterial>();
		lambert->color = Color(random(0, 0x44ff44));
		lambert->transparent = true;
		lambert->opacity = 0.6f;

		materials.push_back(basicMaterial);
		materials.push_back(lambert);
		mesh = SceneUtils::createMultiMaterialObject(geom, materials);

		mesh->traverse([&](Object3D& e) {
			e.castShadow = true;
		});

		scene->add(mesh);

	}
	CustomGeometry::CustomGeometry(int x, int y) : ThreeDemoClass(x,y)
	{
		renderer->setClearColor(Color().setHex(0xEEEEEE), 1);
		renderer->shadowMap->enabled = true;

		isPerspective = true;

		camera = std::make_shared<PerspectiveCamera>(45.0f, screenX / screenY, 0.1f, 1000.0f);
		camera->position.set(-20.0f, 25.0f, 20.0f);
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

		createMesh();

		auto ambientLight = AmbientLight::create(Color().setHex(0x494949));
		scene->add(ambientLight);

		auto spotLight = SpotLight::create(Color().setHex(0xffffff));
		spotLight->shadow->shadowMapSize.set(1024, 1024);
		spotLight->intensity = 1.0f;
		spotLight->distance = 180.0f;
		spotLight->angle = (float)M_PI / 4;
		//spotLight->penumbra = 0.0f;
		//spotLight->decay = 2;
		spotLight->position.set(-40, 30, 30);
		spotLight->castShadow = true;
		scene->add(spotLight);
	}
	void CustomGeometry::init()
	{
		
	}

	void CustomGeometry::render()
	{
		
		if (mesh != nullptr) {
			for (int i = 0;i < mesh->children.size();i++) {
				auto m = mesh->children[i];
				m->geometry->vertices = this->v;
				m->geometry->elementsNeedUpdate = true;
				m->geometry->computeFaceNormals();
			}
		}
		ThreeDemoClass::render();
	}

	void CustomGeometry::showControls()
	{
		ImGui::Begin("Controls");
		if (ImGui::TreeNode("Vertice1")) {
			ImGui::SliderFloat("x", &v[0].x, -10.0f, 10.0f);
			ImGui::SliderFloat("y", &v[0].y, -10.0f, 10.0f);
			ImGui::SliderFloat("z", &v[0].z, -10.0f, 10.0f);

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Vertice2")) {
			ImGui::SliderFloat("x", &v[1].x, -10.0f, 10.0f);
			ImGui::SliderFloat("y", &v[1].y, -10.0f, 10.0f);
			ImGui::SliderFloat("z", &v[1].z, -10.0f, 10.0f);

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Vertice3")) {
			ImGui::SliderFloat("x", &v[2].x, -10.0f, 10.0f);
			ImGui::SliderFloat("y", &v[2].y, -10.0f, 10.0f);
			ImGui::SliderFloat("z", &v[2].z, -10.0f, 10.0f);

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Vertice4")) {
			ImGui::SliderFloat("x", &v[3].x, -10.0f, 10.0f);
			ImGui::SliderFloat("y", &v[3].y, -10.0f, 10.0f);
			ImGui::SliderFloat("z", &v[3].z, -10.0f, 10.0f);

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Vertice5")) {
			ImGui::SliderFloat("x", &v[4].x, -10.0f, 10.0f);
			ImGui::SliderFloat("y", &v[4].y, -10.0f, 10.0f);
			ImGui::SliderFloat("z", &v[4].z, -10.0f, 10.0f);

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Vertice6")) {
			ImGui::SliderFloat("x", &v[5].x, -10.0f, 10.0f);
			ImGui::SliderFloat("y", &v[5].y, -10.0f, 10.0f);
			ImGui::SliderFloat("z", &v[5].z, -10.0f, 10.0f);

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Vertice7")) {
			ImGui::SliderFloat("x", &v[6].x, -10.0f, 10.0f);
			ImGui::SliderFloat("y", &v[6].y, -10.0f, 10.0f);
			ImGui::SliderFloat("z", &v[6].z, -10.0f, 10.0f);

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Vertice8")) {
			ImGui::SliderFloat("x", &v[7].x, -10.0f, 10.0f);
			ImGui::SliderFloat("y", &v[7].y, -10.0f, 10.0f);
			ImGui::SliderFloat("z", &v[7].z, -10.0f, 10.0f);

			ImGui::TreePop();
		}
		/*ImGui::Text("This is Rotaion Speed Control box");
		ImGui::SliderFloat("RotationSpeed", &rotationSpeed, 0.0f, 0.5f);
		if (ImGui::Button("Add Cube")) {
			addCube();
		}
		if (ImGui::Button("Remove Cube")) {
			removeCube();
		}
		ImGui::Text("Number of Objects : %d", numberOfObjects);*/
		ImGui::End();
	}

}

