#ifndef MATERIAL_LIGHT_H
#define MATERIAL_LIGHT_H
#include <ecore/ThreeDemoModel.h>
#include <three/lights/Light.h>
#include <memory>
class MaterialLight : public ThreeDemoClass {
public:
	MaterialLight(int x, int y) :  ThreeDemoClass(x,y) {

	}
	virtual void init() override {       
        renderer->setClearColor(Color().setHex(0x000000));
        renderer->shadowMap->enabled = true;
        //renderer->shadowMap->type = ShadowMapType::PCFSoftShadowMap;

        

		camera = std::make_shared<PerspectiveCamera>(45.0f, screenX/screenY , 0.1f, 1000.0f);
		camera->position.set(-30.0f, 50.0f, 40.0f);
		camera->lookAt(Vector3());

        isPerspective = true;

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
        scene->setBackgroundColor(Color().set(0xffffff));


       
        auto planeGeometry = std::make_shared<PlaneGeometry>(60, 20);
        auto planeMaterial = std::make_shared<MeshLambertMaterial>();
        planeMaterial->color = Color().setHex(0xcccccc);
        auto plane = std::make_shared<Mesh>(planeGeometry, planeMaterial);
        plane->receiveShadow = true;
        plane->rotation.setX((float)(-0.5f * M_PI));

        plane->position.set(15.0f, 0.0f, 0.0f);

        auto cubeGeometry = std::make_shared<BoxGeometry>(4, 4, 4);
        auto cubeMaterial = std::make_shared<MeshLambertMaterial>();
        cubeMaterial->color = Color().setHex(0xff0000);
        cubeMaterial->wireframe = false;
        auto cube = std::make_shared<Mesh>(cubeGeometry, cubeMaterial);
        cube->position.set(-4.0f, 3.0f, 0.0f);
        cube->castShadow = true;

        

        auto sphereGeometry = std::make_shared<SphereGeometry>(4, 20, 20);
        auto sphereMaterial = std::make_shared<MeshLambertMaterial>();
        auto sphere = std::make_shared<Mesh>(sphereGeometry, sphereMaterial);
        sphereMaterial->color = Color().setHex(0x7777ff);
        sphereMaterial->wireframe = false;
        sphere->castShadow = true;

        sphere->position.set(20, 4, 2);

        scene->add(cube);
        scene->add(sphere);
        scene->add(plane);

        auto spotLight = SpotLight::create(Color().setHex(0xffffff));
        spotLight->position.set(-40, 60, -10);
        spotLight->castShadow = true;
        //spotLight->shadow->mapSize = Vector2(512, 512);
        //spotLight->shadow->camera->_far = 130;
        //spotLight->shadow->camera->_near = 40;

        scene->add(spotLight);

        auto ambientLight = AmbientLight::create(Color().setHex(0x353535));

        //scene->add(ambientLight);

        this->initialized = true;
	}
};
#endif