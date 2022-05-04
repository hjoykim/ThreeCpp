#ifndef CONVEX_GEOMETRY_H
#define CONVEX_GEOMETRY_H
#include <three/core/BufferGeometry.h>
#include <three/core/BufferAttribute.h>
namespace three {
	class ConvexBufferGeometry : public BufferGeometry {
	public:
		ConvexBufferGeometry() : BufferGeometry() {}

		ConvexBufferGeometry(std::vector<Vector3>& points);
	};

	class ConvexGeometry : public Geometry {
	public:
		ConvexGeometry() : Geometry() {}

		ConvexGeometry(std::vector<Vector3>& points);
	};
}
#endif