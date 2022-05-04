#ifndef LENSFLARES_EXAMPLE_H
#define LENSFLARES_EXAMPLE_H
#include <ecore/ThreeDemoModel.h>
#include <ecore/ExampleUtil.h>
#include <three/loaders/TextureLoader.h>
#include <imgui/imgui.h>
#include <iostream>
#include <three/lights/Light.h>
#include <three/objects/Lensflare.h>

class LensflaresExample : public ThreeDemoClass {
public:
	Mesh::ptr cube;
	Mesh::ptr sphere;

	float step = 0;
	float rotationSpeed = 0.03f;
	float bouncingSpeed = 0.03f;
	LensflaresExample(int x, int y) : ThreeDemoClass(x, y) {}

	virtual void init() override {
		initRenderer(renderer);
		initCamera(camera,renderer,Vector3(-20,10,45));
		camera->lookAt(10, 0, 0);
		initTrackballControls(controller, camera, renderer);

		scene = std::make_shared<Scene>();

		Texture::ptr textureGrass = TextureLoader::load("./assets/textures/ground/grasslight-big.jpg");
		textureGrass->wrapS = Wrapping::RepeatWrapping;
		textureGrass->wrapT = Wrapping::RepeatWrapping;
		textureGrass->repeat.set(10, 10);

		auto planeGeometry = PlaneBufferGeometry::create(1000, 1000, 20, 20);
		auto planeMaterial = MeshLambertMaterial::create();
		planeMaterial->map = textureGrass;

		auto plane = Mesh::create(planeGeometry, planeMaterial);
		plane->receiveShadow = true;

		// rotate and position the plane
		plane->rotation.setX(-0.5 * PI);
		plane->position.x = 15;
		plane->position.y = 0;
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

		auto sphereGeometry = SphereBufferGeometry::create(4, 25, 25);
		auto sphereMaterial = MeshPhongMaterial::create(Color(0x7777ff));
		sphere = Mesh::create(sphereGeometry, sphereMaterial);
		sphere->name = "Sphere";
		// position the sphere
		sphere->position.x = 10;
		sphere->position.y = 5;
		sphere->position.z = 10;
		sphere->castShadow = true;

		// add the sphere to the scene
		scene->add(sphere);

		auto ambientLight = AmbientLight::create(Color(0x1c1c1c));
		scene->add(ambientLight);

		auto spotLight0 = SpotLight::create(Color(0xcccccc));
		spotLight0->position.set(-40, 60, -10);		
		scene->add(spotLight0);

		auto spotLight = DirectionalLight::create(Color(0xffffff));
		spotLight->position.set(30, 10, -50);
		spotLight->castShadow = true;	
		spotLight->target = plane;
		spotLight->distance = 0;
		spotLight->shadow->camera->_near = 2;
		spotLight->shadow->camera->_far = 200;
		spotLight->shadow->camera->left = -100;
		spotLight->shadow->camera->cameraRight = 100;
		spotLight->shadow->camera->top = 100;
		spotLight->shadow->camera->bottom = -100;
		spotLight->shadow->shadowMapSize.set(2048,2048);


		scene->add(spotLight);

		auto textureFlare0 = TextureLoader::load("./assets/textures/flares/lensflare0.png");
		auto textureFlare1 = TextureLoader::load("./assets/textures/flares/lensflare3.png");
		auto textureFlare2 = TextureLoader::load("./assets/textures/flares/lensflare3.png");
		auto textureFlare3 = TextureLoader::load("./assets/textures/flares/lensflare3.png");
		auto textureFlare4 = TextureLoader::load("./assets/textures/flares/lensflare3.png");

		auto flareColor = Color(0xffaacc);

		auto lensFlare = Lensflare::create();

		lensFlare->addElement(LensflareElement::create(textureFlare0, 350, 0.0, flareColor));
		lensFlare->addElement(LensflareElement::create(textureFlare1, 60, 0.6, flareColor));
		lensFlare->addElement(LensflareElement::create(textureFlare1, 70, 0.7, flareColor));
		lensFlare->addElement(LensflareElement::create(textureFlare1, 120, 0.9, flareColor));
		lensFlare->addElement(LensflareElement::create(textureFlare1, 70, 1.0, flareColor));

		spotLight->add(lensFlare);

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
		ThreeDemoClass::render();
	}
	virtual void showControls() override {

	}
};
#endif