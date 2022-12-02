#ifndef BASIC2D_GEOMETRIES_PLANE_H
#define BASIC2D_GEOMETRIES_PLANE_H
#include <ecore/ThreeDemoModel.h>
#include <ecore/ExampleUtil.h>
#include <three/core/BufferGeometry.h>
#include <three/Materials.h>
#include <three/objects/Mesh.h>
#include <three/geometries/BoxGeometry.h>
#include <three/geometries/PlaneGeometry.h>
#include <three/geometries/SphereGeometry.h>
#include <three/objects/Group.h>
#include <three/loaders/ObjLoader.h>
#include <thread>
#include <imgui/imgui.h>

class Basic2DGeometriesPlane : public ThreeDemoClass {
private:
	float width = 20;
	float height = 20;
	float widthSegment = 4;
	float heightSegment = 4;
public:
	Mesh::ptr appliedMesh;
	Mesh::ptr groundPlane;
	int appliedMaterialIndex = 0;
	float step = 0.0f;
	Material::ptr appliedNormalMaterial;
	Material::ptr appliedStandardMaterial;

	Basic2DGeometriesPlane(int x, int y);

	virtual void initRenderer();
	virtual void initCamera();
	virtual void initCameraController();
	virtual void buildMesh();
	virtual void rebuildGeometry();
	virtual void redraw();
	virtual void init() override;
	virtual void initLighting();
	virtual BufferGeometry::ptr buildGeometry();
	virtual void render() override;
	virtual void showControls() override;
	virtual void addCastShadow();
	virtual void addGroundPlaneVisible();
	virtual void addGeometryParameter();
	virtual void addGeometrySettings();
	virtual void addBasicMaterialSettings(Material::ptr& material,string name);
	virtual void addColorPicker(Material::ptr& material);
	virtual void addEmissivePicker(Material::ptr& material);
	virtual void addSpecularPicker(Material::ptr& material);
	virtual void addRoughness(Material::ptr& material);
	virtual void addMetalness(Material::ptr& material);
	virtual void addWireframeLineProperty(Material::ptr& material);
	virtual void addStandardMaterialSettings(Material::ptr& material);
	virtual void addNormalMaterialSettings(Material::ptr* material);
};
#endif