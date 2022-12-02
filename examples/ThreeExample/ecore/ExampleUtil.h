#ifndef EXAMPLE_UTIL_H
#define EXAMPLE_UTIL_H
#include <ecore/ThreeDemoModel.h>
#include <tuple>

void initRenderer(const GLRenderer::ptr& renderer);

void initCamera(Camera::ptr& camera,const GLRenderer::ptr& renderer,const Vector3& position = Vector3(-30,40,30));

std::tuple<Light::ptr, Light::ptr> initDefaultLighting(const Scene::ptr& scene, const Vector3& position);

void initTrackballControls(TrackballControls::ptr& control,const Camera::ptr& camera, const GLRenderer::ptr& renderer);

void addHouseAndTree(const Scene::ptr& scene);

std::tuple<Mesh::ptr,Mesh::ptr> addDefaultCubeAndSphere(const Scene::ptr& scene);

Mesh::ptr addGroundPlane(const Scene::ptr& scene);

Mesh::ptr addLargeGroundPlane(const Scene::ptr& scene, const bool useTexture = false);

Mesh::ptr applyMeshNormalMaterial(const BufferGeometry::ptr& geometry, Material::ptr material);
#endif