#ifndef OCTHEDRON_GEOMETRY_H
#define OCTHEDRON_GEOMETRY_H
#include <three/geometries/PolyhedronGeometry.h>
namespace three {
	class OctahedronBufferGeometry : public PolyhedronBufferGeometry {
	protected:
		float radius;
		float detail;
	public:
		using ptr = std::shared_ptr<OctahedronBufferGeometry>;

		OctahedronBufferGeometry(float radius = 0, float detail = 0);
	};
	class OctahedronGeometry : public Geometry {
	protected:
		float radius;
		float detail;
	public:
		using ptr = std::shared_ptr<OctahedronGeometry>;

		OctahedronGeometry(float radius = 0, float detail = 0);
	};
}
#endif