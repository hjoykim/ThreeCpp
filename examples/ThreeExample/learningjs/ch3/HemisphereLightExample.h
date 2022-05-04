#ifndef HEMISPHERELIGHT_EXAMPLE_H
#define HEMISPHERELIGHT_EXAMPLE_H
#include <ecore/ThreeDemoModel.h>
#include <ecore/ExampleUtil.h>
#include <three/loaders/TextureLoader.h>
#include <imgui/imgui.h>
#include <iostream>
class HemisphereLightExample : public ThreeDemoClass {
public:

	HemisphereLight::ptr hemiLight;
	Mesh::ptr cube;
	Mesh::ptr sphere;
	float step = 0;
	float invert = 1;
	float phase = 0;
	float rotationSpeed = 0.03f;
	float bouncingSpeed = 0.03f;

	HemisphereLightExample(int x, int y) :ThreeDemoClass(x,y) {
    }


	virtual void init() override {
		initRenderer(renderer);
		initCamera(camera, renderer);
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
        plane->name = "Plane";

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

        // position the sphere
        sphere->position.x = 10;
        sphere->position.y = 5;
        sphere->position.z = 10;
        sphere->castShadow = true;

        // add the sphere to the scene
        scene->add(sphere);

        // add spotlight for a bit of light
        auto spotLight0 = SpotLight::create(Color(0xcccccc));
        spotLight0->position.set(-40, 60, -10);
        spotLight0->lookAt(plane->position);
        scene->add(spotLight0);

       

        hemiLight = HemisphereLight::create(Color(0x0000ff), Color(0x00ff00), 0.6);
        hemiLight->position.set(0, 500, 0);
        scene->add(hemiLight);

        auto dirLight = DirectionalLight::create(Color(0xffffff));
        dirLight->name = "DirectionalLight";
        dirLight->position.set(30, 10, -50);
        dirLight->castShadow = true;
        dirLight->target = plane;
        dirLight->shadow->camera->_near = 0.1f;
        dirLight->shadow->camera->_far = 200;
        dirLight->shadow->camera->left = -50;
        dirLight->shadow->camera->cameraRight = 50;
        dirLight->shadow->camera->top = 50;
        dirLight->shadow->camera->bottom = -50;
        dirLight->shadow->shadowMapSize.set(2048,2048);
        scene->add(dirLight);

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
        if (!initialized) return;

        static bool changeIntensity=false;

        ImGui::Begin("Controls");
        ImGui::ColorPicker3("groundColor", (float*)&hemiLight->groundColor, 1);
        ImGui::ColorPicker3("color", (float*)&hemiLight->color, 0);
        ImGui::DragFloat("Intensity", &hemiLight->intensity, 0.1f, 0.0f, 5.0f);
       
        
        ImGui::End();
	}
};
#endif