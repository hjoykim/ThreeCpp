#ifndef CONE_GEOMETRY_H
#define CONE_GEOMETRY_H
#include <three/geometries/CylinderGeometry.h>
namespace three {
	class ConeGeometry : public CylinderGeometry {
	public:
		ConeGeometry(float radius,float height, float radialSegments = std::numeric_limits<float>::quiet_NaN(),
			float heightSegments = std::numeric_limits<float>::quiet_NaN(),
			bool openEnded = false,
			float thetaStart = std::numeric_limits<float>::quiet_NaN(),
			float thetaLength = std::numeric_limits<float>::quiet_NaN())
		: CylinderGeometry(0,radius,height,radialSegments,heightSegments,openEnded,thetaStart,thetaLength){}
	};

	class ConeBufferGeometry : public CylinderBufferGeometry {
	public :
		ConeBufferGeometry(float radius, float height, float radialSegments = std::numeric_limits<float>::quiet_NaN(),
			float heightSegments = std::numeric_limits<float>::quiet_NaN(),
			bool openEnded = false,
			float thetaStart = std::numeric_limits<float>::quiet_NaN(),
			float thetaLength = std::numeric_limits<float>::quiet_NaN())
			:CylinderBufferGeometry(0, radius, height, radialSegments, heightSegments, openEnded, thetaStart, thetaLength) {}
	};
}
#endif