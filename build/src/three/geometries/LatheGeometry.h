#ifndef LATHE_GEOMETRY_H
#define LATHE_GEOMETRY_H
#include <three/ThreeMath.h>
#include <three/core/BufferGeometry.h>
#include <limits>
#include <vector>
using namespace three::math;
namespace three {
	class LatheBufferGeometry : public BufferGeometry {
	protected:
		std::vector<Vector3> points;
		float segments;
		float phiStart;
		float phiLength;
	public:
		using ptr = std::shared_ptr<LatheBufferGeometry>;
		LatheBufferGeometry(const std::vector<Vector3>& points, float segments = std::numeric_limits<float>::quiet_NaN(), float phiStart = std::numeric_limits<float>::quiet_NaN(), float phiLength = std::numeric_limits<float>::quiet_NaN());
	
	};
	class LatheGeometry : public Geometry {
	protected:
		std::vector<Vector3> points;
		float segments;
		float phiStart;
		float phiLength;
	public:
		using ptr = std::shared_ptr<LatheGeometry>;
		LatheGeometry(const std::vector<Vector3>& points, float segments = std::numeric_limits<float>::quiet_NaN(), float phiStart = std::numeric_limits<float>::quiet_NaN(), float phiLength = std::numeric_limits<float>::quiet_NaN());
	};
}
#endif