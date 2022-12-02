#include "ExampleUtil.h"
#include <three/geometries/BoxGeometry.h>
#include <three/geometries/ConeGeometry.h>
#include <three/geometries/CylinderGeometry.h>
#include <filesystem>
#include <three/loaders/ObjLoader.h>
#include <three/loaders/TextureLoader.h>
void initRenderer(const GLRenderer::ptr& renderer) {
    renderer->setClearColor(Color().set(0x000000));
    renderer->shadowMap->enabled = true;
    renderer->shadowMap->type = ShadowMapType::PCFSoftShadowMap;
}
void initCamera(Camera::ptr& camera, const GLRenderer::ptr& renderer,const Vector3& position) {
    Vector4 viewport;
    renderer->getViewport(viewport);
    camera = std::make_shared<PerspectiveCamera>(45.0f, viewport.z / viewport.w, 0.1f, 1000.0f);
    camera->position.copy(position);
    camera->lookAt(Vector3());
}

std::tuple<Light::ptr, Light::ptr> initDefaultLighting(const Scene::ptr& scene, const Vector3& position) {
    auto spotLight = SpotLight::create(Color(0xffffff));
    spotLight->position.copy(position);
    spotLight->shadow->shadowMapSize.set(2048,2048);
    spotLight->shadow->camera->fov = 15;
    spotLight->castShadow = true;
    spotLight->decay = 2;
    spotLight->penumbra = 0.05;
    spotLight->name = "spotLight";

    scene->add(spotLight);

    auto ambientLight = AmbientLight::create(Color(0x343434));
    ambientLight->name = "ambientLight";
    scene->add(ambientLight);

    return { ambientLight,spotLight };
}

void initTrackballControls(TrackballControls::ptr& controller,const Camera::ptr& camera, const GLRenderer::ptr& renderer) {
    Vector4 screen;
    renderer->getViewport(screen);

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

void createBoundingWall(const Scene::ptr& scene) {
    auto wallLeft = BoxBufferGeometry::create(70, 2, 2);
    auto wallRight = BoxBufferGeometry::create(70, 2, 2);
    auto wallTop = BoxBufferGeometry::create(2, 2, 50);
    auto wallBottom = BoxBufferGeometry::create(2, 2, 50);

    auto wallMaterial = std::make_shared<MeshPhongMaterial>(Color().set(0xa0522d));
        

    auto wallLeftMesh = Mesh::create(wallLeft, wallMaterial);
    auto wallRightMesh = Mesh::create(wallRight, wallMaterial);
    auto wallTopMesh = Mesh::create(wallTop, wallMaterial);
    auto wallBottomMesh = Mesh::create(wallBottom, wallMaterial);

    wallLeftMesh->position.set(15, 1, -25);
    wallRightMesh->position.set(15, 1, 25);
    wallTopMesh->position.set(-19, 1, 0);
    wallBottomMesh->position.set(49, 1, 0);

    scene->add(wallLeftMesh);
    scene->add(wallRightMesh);
    scene->add(wallBottomMesh);
    scene->add(wallTopMesh);
}
void createGroundPlane(const Scene::ptr& scene) {
    // create the ground plane
    auto planeGeometry = PlaneBufferGeometry::create(70, 50);
    auto planeMaterial = MeshPhongMaterial::create(Color().set(0x9acd32));
    auto plane = Mesh::create(planeGeometry, planeMaterial);
    plane->receiveShadow = true;

    // rotate and position the plane
    plane->rotation.setX(-0.5f * M_PI);
    plane->position.x = 15;
    plane->position.y = 0;
    plane->position.z = 0;

    scene->add(plane);
}
void createHouse(const Scene::ptr& scene) {
    auto roof = std::make_shared<ConeBufferGeometry>(5, 4);
    auto base = std::make_shared<CylinderBufferGeometry>(5, 5, 6);

    // create the mesh
    auto roofMesh = Mesh::create(roof, std::make_shared<MeshPhongMaterial>(Color().set(0x8b7213)));
    auto baseMesh = Mesh::create(base, std::make_shared<MeshPhongMaterial>(Color().set(0xffe4c4)));

    roofMesh->position.set(25, 8, 0);
    baseMesh->position.set(25, 3, 0);

    roofMesh->receiveShadow = true;
    baseMesh->receiveShadow = true;
    roofMesh->castShadow = true;
    baseMesh->castShadow = true;

    scene->add(roofMesh);
    scene->add(baseMesh);
}
void createTree(const Scene::ptr& scene) {
    auto trunk = BoxBufferGeometry::create(1, 8, 1);
    auto leaves = std::make_shared<SphereBufferGeometry>(4);

    // create the mesh
    auto trunkMesh = Mesh::create(trunk, std::make_shared<MeshPhongMaterial>(Color().set(0x8b4513)));
    auto leavesMesh = Mesh::create(leaves, std::make_shared<MeshPhongMaterial>(Color().set(0x00ff00)));

    // position the trunk. Set y to half of height of trunk
    trunkMesh->position.set(-10, 4, 0);
    leavesMesh->position.set(-10, 12, 0);

    trunkMesh->castShadow = true;
    trunkMesh->receiveShadow = true;
    leavesMesh->castShadow = true;
    leavesMesh->receiveShadow = true;

    scene->add(trunkMesh);
    scene->add(leavesMesh);
}
void addHouseAndTree(const Scene::ptr& scene)
{
	createBoundingWall(scene);
	createGroundPlane(scene);
	createHouse(scene);
	createTree(scene);
}
std::tuple<Mesh::ptr, Mesh::ptr> addDefaultCubeAndSphere(const Scene::ptr& scene) {
    // create a cube
    auto cubeGeometry = std::make_shared<BoxBufferGeometry>(4, 4, 4);
    auto cubeMaterial = std::make_shared<MeshLambertMaterial>(Color(0xff0000));
    auto cube = std::make_shared<Mesh>(cubeGeometry, cubeMaterial);
    cube->castShadow = true;

    // position the cube
    cube->position.x = -4;
    cube->position.y = 3;
    cube->position.z = 0;

    // add the cube to the scene
    scene->add(cube);

    auto sphereGeometry = std::make_shared<SphereBufferGeometry>(4, 20, 20);
    auto sphereMaterial = std::make_shared<MeshLambertMaterial>(Color(0x7777ff));
    auto sphere = std::make_shared<Mesh>(sphereGeometry, sphereMaterial);

    // position the sphere
    sphere->position.x = 20;
    sphere->position.y = 0;
    sphere->position.z = 2;
    sphere->castShadow = true;

    // add the sphere to the scene
    scene->add(sphere);

    return { cube,sphere };
}

Mesh::ptr addGroundPlane(const Scene::ptr& scene)
{
    // create the ground plane
    auto planeGeometry = PlaneBufferGeometry::create(60, 20, 120, 120);
    auto planeMaterial = MeshPhongMaterial::create(Color().set(0xffffff));
    auto plane = Mesh::create(planeGeometry, planeMaterial);
    plane->receiveShadow = true;

    // rotate and position the plane
    plane->rotation.setX(-0.5f * M_PI);
    plane->position.x = 15;
    plane->position.y = 0;
    plane->position.z = 0;

    scene->add(plane);

    return plane;
}

Mesh::ptr addLargeGroundPlane(const Scene::ptr& scene, const bool useTexture) {

    auto planeGeometry = PlaneBufferGeometry::create(10000,10000);
    auto planeMaterial = MeshPhongMaterial::create(Color().set(0xffffff));
    if (useTexture) {
        std::string dir = filesystem::current_path().parent_path().parent_path().string();
        TextureLoader loader;
        auto texture = loader.load(dir + "\\assets\\textures\\general\\floor-wood.jpg");
        planeMaterial->map = texture;
    }
    auto plane = Mesh::create(planeGeometry, planeMaterial);
    plane->receiveShadow = true;

    plane->rotation.setX(-0.5f * (float)M_PI);
    plane->position.set(0, 0, 0);

    scene->add(plane);

    return plane;
}
Mesh::ptr applyMeshNormalMaterial(const BufferGeometry::ptr& geometry, Material::ptr material) {
    if (material == NULL || !instanceOf<MeshNormalMaterial>(material.get())) {
        material = MeshNormalMaterial::create();
        material->side = Side::DoubleSide;
    }
    return Mesh::create(geometry, material);
}
