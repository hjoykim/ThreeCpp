#ifndef TORUSKNOT_GEOMETRY_H
#define TORUSKNOT_GEOMETRY_H
#include <three/core/BufferGeometry.h>
namespace three {
	class TorusKnotBufferGeometry : public BufferGeometry {
	private :
		void calculatePositionOnCurve(float u, float p, float q, float radius, Vector3& position);
	public:
		using ptr = std::shared_ptr<TorusKnotBufferGeometry>;

		TorusKnotBufferGeometry(float radius = 0, float tube = 0, float tubularSegments = 0, float radialSegments = 0, float p = 0, float q = 0);
	};
	class TorusKnotGeometry : public Geometry {
	public:
		using ptr = std::shared_ptr<TorusKnotGeometry>;

		TorusKnotGeometry(float radius = 0, float tube = 0, float tubularSegments = 0, float radialSegments = 0, float p = 0, float q = 0);
	};
}
#endif