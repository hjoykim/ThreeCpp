#ifndef BASIC_MESH_MATERIAL_EXAMPLE_H
#define BASIC_MESH_MATERIAL_EXAMPLE_H
#include <ecore/ThreeDemoModel.h>
#include <ecore/ExampleUtil.h>
#include <three/Materials.h>
#include <three/objects/Mesh.h>
#include <three/geometries/BoxGeometry.h>
#include <three/geometries/PlaneGeometry.h>
#include <three/geometries/SphereGeometry.h>
#include <three/objects/Group.h>
#include <three/loaders/ObjLoader.h>
#include <thread>
#include <imgui/imgui.h>
class BasicMeshMaterialExample : public ThreeDemoClass {

	
public:
	float step = 0;
	Object3D::ptr selectedMesh;
	Mesh::ptr plane;
	Mesh::ptr cube;
	Mesh::ptr sphere;
	MeshBasicMaterial::ptr meshMaterial;
	Group::ptr gopher;

	int side = 0;
	int shadowSide = 0;
	int vertexColors = 1;
	int join = 0;
	int lineCap = 1;
	int selected = 0;

	BasicMeshMaterialExample(int x, int y) : ThreeDemoClass(x, y) {}
		

	void loadGopher(const Material::ptr& material, std::string path, Group::ptr& gopher);

	virtual void init() override;
	

	virtual void render() override;

	virtual void showControls() override;

	void setMaterialGroup(Object3D& o);
};

#endif