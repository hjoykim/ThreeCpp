#ifndef CUSTOM_GEOMETRY_H
#define CUSTOM_GEOMETRY_H
#include <ecore/ThreeDemoModel.h>
#include <three/objects/Mesh.h>
#include <three/objects/Group.h>
#include <three/lights/Light.h>
#include <memory>
#include <imgui/imgui.h>
#include <vector>
#include <three/math/Vector3.h>
namespace three {
	class CustomGeometry : public ThreeDemoClass {
	private :
		std::vector<Vector3> v;
		void createMesh();
	public:
		Group::ptr mesh;

		CustomGeometry(int x, int y);


		virtual void init() override;

		virtual void render()  override;

		virtual void showControls()  override;

	};
}
#endif