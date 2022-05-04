#ifndef TETRAHEDRON_GEOMETRY_H
#define TETRAHEDRON_GEOMETRY_H
#include <three/geometries/PolyhedronGeometry.h>
namespace three {
	class TetrahedronBufferGeometry : public PolyhedronBufferGeometry {
	public:
		using ptr = std::shared_ptr<TetrahedronBufferGeometry>;

		TetrahedronBufferGeometry(float radius = 0, float detail = 0);
	};
	class TetrahedronGeometry : public Geometry {
	public:
		using ptr = std::shared_ptr<TetrahedronGeometry>;

		TetrahedronGeometry(float radius = 0, float detail = 0);
	};
}
#endif