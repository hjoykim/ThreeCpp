#ifndef MESH_PROPERTIES_H
#define MESH_PROPERTIES_H
#include <ecore/ThreeDemoModel.h>
#include <three/objects/Mesh.h>
#include <three/objects/Group.h>
#include <three/lights/Light.h>
#include <memory>
#include <imgui/imgui.h>
#include <vector>
#include <three/math/Vector3.h>
namespace three {
	class MeshProperties : public ThreeDemoClass {
	private :
		float step = 0.0f;
		float translateX = 0.0f;
		float translateY = 0.0f;
		float translateZ = 0.0f;
		float rotationX = 0.0f;
		float rotationY = 0.0f;
		float rotationZ = 0.0f;
		Mesh::ptr cube;
	public :
		MeshProperties(int x, int y);

		virtual void init() override;

		virtual void render() override;

		virtual void showControls() override;
	};
}
#endif