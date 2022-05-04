#ifndef TORUS_GEOMETRY_H
#define TORUS_GEOMETRY_H
#include <three/core/BufferGeometry.h>
namespace three {
	class TorusBufferGeometry : public BufferGeometry {
	public:
		using ptr = std::shared_ptr<TorusBufferGeometry>;

		TorusBufferGeometry(float radius = 0, float tube = 0, float radialSegments = 0, float tubularSegments = 0, float arc = 0);
	};
	class TorusGeometry : public Geometry {
	public:
		using ptr = std::shared_ptr<TorusGeometry>;
		TorusGeometry(float radius = 0, float tube = 0, float radialSegments = 0, float tubularSegments = 0, float arc = 0);
	};
}
#endif