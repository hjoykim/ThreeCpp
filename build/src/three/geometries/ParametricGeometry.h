#ifndef PARAMETRIC_GEOMETRY_H
#define PARAMETRIC_GEOMETRY_H
#include <three/core/BufferGeometry.h>
#include <functional>
namespace three {
	class ParametricBufferGeometry : public BufferGeometry {
	protected:
		std::function<Vector3& (float, float, Vector3&)> func;
		float slices;
		float stacks;
	public:
		using ptr = std::shared_ptr<ParametricBufferGeometry>;

		ParametricBufferGeometry(std::function<Vector3&(float,float,Vector3&)> func, int slices, float stacks);
	};
	class ParametricGeometry : public Geometry {
	protected:
		std::function<Vector3& (float, float, Vector3&)> func;
		float slices;
		float stacks;
	public:
		using ptr = std::shared_ptr<ParametricGeometry>;

		ParametricGeometry(std::function<Vector3& (float, float, Vector3&)> func, int slices, float stacks);
	};
}
#endif