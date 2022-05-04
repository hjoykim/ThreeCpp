#ifndef SPHERE_GEOMETRY_THREE_H
#define SPHERE_GEOMETRY_THREE_H
#include <three/core/Geometry.h>
#include <three/core/BufferGeometry.h>
#include <limits>
namespace three {
	class SphereBufferGeometry : public BufferGeometry {
	public:
		using ptr = std::shared_ptr<SphereBufferGeometry>;
		SphereBufferGeometry(float radius = 0, float widthSegments = 0, float heightSegments = 0,
			float phiStart = std::numeric_limits<float>::quiet_NaN(),
			float phiLength = std::numeric_limits<float>::quiet_NaN(),
			float thetaStart = std::numeric_limits<float>::quiet_NaN(),
			float thetaLength = std::numeric_limits<float>::quiet_NaN());
		

		static ptr create(float radius = 0, float widthSegments = 0, float heightSegments = 0,
			float phiStart = std::numeric_limits<float>::quiet_NaN(),
			float phiLength = std::numeric_limits<float>::quiet_NaN(),
			float thetaStart = std::numeric_limits<float>::quiet_NaN(),
			float thetaLength = std::numeric_limits<float>::quiet_NaN()) {
			return std::make_shared<SphereBufferGeometry>(radius,widthSegments,heightSegments,phiStart,phiLength,thetaStart,thetaLength);
		}
	};


	class SphereGeometry : public Geometry {
	public:
		using ptr = std::shared_ptr<SphereGeometry>;

		SphereGeometry(float radius = 0, float widthSegments = 0, float heightSegments = 0,
			float phiStart = std::numeric_limits<float>::quiet_NaN(),
			float phiLength = std::numeric_limits<float>::quiet_NaN(),
			float thetaStart = std::numeric_limits<float>::quiet_NaN(),
			float thetaLength = std::numeric_limits<float>::quiet_NaN());
		

		static ptr create(float radius = 0, float widthSegments = 0, float heightSegments = 0,
			float phiStart = std::numeric_limits<float>::quiet_NaN(),
			float phiLength = std::numeric_limits<float>::quiet_NaN(),
			float thetaStart = std::numeric_limits<float>::quiet_NaN(),
			float thetaLength = std::numeric_limits<float>::quiet_NaN()) {
			return std::make_shared<SphereGeometry>(radius,widthSegments,heightSegments,phiStart,phiLength,thetaStart,thetaLength);
		}
	};
}
#endif
