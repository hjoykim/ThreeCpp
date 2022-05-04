#ifndef MATERIAL_LIGHT_ANIMATION_H
#define MATERIAL_LIGHT_ANIMATION_H
#include <ecore/ThreeDemoModel.h>
#include <three/objects/Mesh.h>
#include <three/lights/Light.h>
#include <memory>
#include <imgui/imgui.h>
class MaterialLightAnimation : public ThreeDemoClass {
public:
    Mesh::ptr plane;
    Mesh::ptr cube;
    Mesh::ptr sphere;
    float step = 0.0f;
    float rotationSpeed = 0.02f;
    float bouncingSpeed = 0.04f;
	MaterialLightAnimation(int x, int y) :  ThreeDemoClass(x,y) {

	}
	virtual void init() override {  

        isPerspective = true;
        renderer->setClearColor(Color().setHex(0x000000));
        renderer->shadowMap->enabled = true;
        //renderer->shadowMap->type = ShadowMapType::PCFSoftShadowMap;

        

		camera = std::make_shared<PerspectiveCamera>(45.0f, screenX/screenY , 0.1f, 1000.0f);
		camera->position.set(-30.0f, 50.0f, 40.0f);
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


       
        auto planeGeometry = std::make_shared<PlaneGeometry>(60, 20);
        auto planeMaterial = std::make_shared<MeshLambertMaterial>();
        planeMaterial->color = Color().setHex(0xcccccc);
        plane = std::make_shared<Mesh>(planeGeometry, planeMaterial);
        plane->receiveShadow = true;
        plane->rotation.setX((float)(-0.5f * M_PI));

        plane->position.set(15.0f, 0.0f, 0.0f);

        auto cubeGeometry = std::make_shared<BoxGeometry>(4, 4, 4);
        auto cubeMaterial = std::make_shared<MeshLambertMaterial>();
        cubeMaterial->color = Color().setHex(0xff0000);
        cubeMaterial->wireframe = false;
        cube = std::make_shared<Mesh>(cubeGeometry, cubeMaterial);
        cube->position.set(-4.0f, 3.0f, 0.0f);
        cube->castShadow = true;

        

        auto sphereGeometry = std::make_shared<SphereGeometry>(4, 20, 20);
        auto sphereMaterial = std::make_shared<MeshLambertMaterial>();
        sphere = std::make_shared<Mesh>(sphereGeometry, sphereMaterial);
        sphereMaterial->color = Color().setHex(0x7777ff);
        sphereMaterial->wireframe = false;
        sphere->castShadow = true;

        sphere->position.set(20, 4, 2);

        scene->add(cube);
        scene->add(sphere);
        scene->add(plane);

        auto spotLight = SpotLight::create(Color().setHex(0xffffff));
        spotLight->position.set(-40, 40, -15);
        spotLight->castShadow = true;
        //spotLight->shadow->mapSize = Vector2(512, 512);
        //spotLight->shadow->camera->_far = 130;
        //spotLight->shadow->camera->_near = 40;

        scene->add(spotLight);

        auto ambientLight = AmbientLight::create(Color().setHex(0x353535));

        this->initialized = true;
	}
    virtual void render() override {
        cube->rotation.setX(cube->rotation.getX()+rotationSpeed);
        cube->rotation.setY(cube->rotation.getY() + rotationSpeed);
        cube->rotation.setZ(cube->rotation.getZ() + rotationSpeed);

        step += bouncingSpeed;

        sphere->position.x = 20 + (10 * cos(step));
        sphere->position.y = 2 + (10 * abs(sin(step)));
        ThreeDemoClass::render();
    }
    virtual void showControls() override {
        ImGui::Begin("Controls");
        ImGui::Text("This is Rotaion Speed Control box");
        ImGui::SliderFloat("RotationSpeed", &rotationSpeed, 0.0f, 0.5f);
        ImGui::SliderFloat("BouncingSpeed", &bouncingSpeed, 0.0f, 0.5f);
        ImGui::End();
    }
};
#endif