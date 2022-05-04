#ifndef ICOSAHEDRONGEOMETRY_H
#define ICOSAHEDRONGEOMETRY_H
#include <three/geometries/PolyhedronGeometry.h>
#include <limits>
namespace three {
	class IcosahedronBufferGeometry : public PolyhedronBufferGeometry {
	public:
		using ptr = std::shared_ptr<IcosahedronBufferGeometry>;
		IcosahedronBufferGeometry(float radius, float detail = std::numeric_limits<float>::quiet_NaN());
	};
	class IcosahedronGeometry : public Geometry {
	public :
		using ptr = std::shared_ptr<IcosahedronGeometry>;
		IcosahedronGeometry(float radius, float detail = std::numeric_limits<float>::quiet_NaN());
	};
}
#endif