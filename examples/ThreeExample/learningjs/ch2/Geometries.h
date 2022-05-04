#ifndef GEOMETRIES_EXAMPLE_H
#define GEOMETRIES_EXAMPLE_H
#include <ecore/ThreeDemoModel.h>
#include <three/objects/Mesh.h>
#include <three/lights/Light.h>
#include <memory>
#include <imgui/imgui.h>
#include <vector>
namespace three {
	class Geometries : public ThreeDemoClass {
	public:
		Mesh::ptr plane;
		PlaneGeometry::ptr planeGeometry;

		Geometries(int x, int y);
		
		void addGeometries();

		virtual void render() override;

		virtual void showControls() override {}
	};
}
#endif
