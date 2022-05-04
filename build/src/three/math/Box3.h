#ifndef BOX3_THREE_H
#define BOX3_THREE_H
#include <three/math/Vector3.h>
namespace three {

	template<typename T>
	class BufferAttribute;

	class Object3D;

	namespace math {
		class Sphere;
		class Plane;
		class Triangle;

		class Box3 {

		public:
			Vector3 max;

			Vector3 min;

			Box3();
			
			Box3(const Vector3& min, const Vector3& max) : min(min), max(max) {};

			virtual ~Box3() = default;

			Box3& set(const Vector3& min, const Vector3& max);

			Box3& setFromArray(const float* array, const unsigned length);

			Box3& setFromBufferAttribute(const BufferAttribute<float>& attribute);

			Box3& setFromPoints(const Vector3* points,unsigned length);

			Box3& setFromCenterAndSize(const Vector3& center, const Vector3& size);

			Box3& setFromObject(Object3D& object);

			Box3& clone(Box3* target);

			Box3& copy(const Box3& box);

			Box3& makeEmpty();

			bool isEmpty();

			Vector3& getCenter(Vector3* target);

			Vector3& getSize(Vector3* target);

			Box3& expandByPoint(const Vector3& point);

			Box3& expandByVector(const Vector3& vector);

			Box3& expandByScalar(const float scalar);

			Box3& expandByObject(Object3D& object);

			bool containsPoint(const Vector3& point);

			bool containsBox(const Box3& box);

			void getParameter(const Vector3& point,Vector3* target);

			bool intersectsBox(const Box3& box);

			bool intersectsSphere(const Sphere& sphere);

			bool intersectsPlane(const Plane& plane) const;

			bool intersectsTriangle(const Triangle& triangle);

			void clampPoint(const Vector3& point, Vector3* target);

			float distanceToPoint(const Vector3& point);

			Sphere& getBoundingSphere(Sphere* target);

			Box3& intersect(const Box3& box);

			Box3& Union(const Box3& box);

			Box3& applyMatrix4(const Matrix4& matrix);

			Box3& translate(const Vector3& offset);

			bool equals(const Box3& box);
		};
		bool satForAxes(const std::vector<float> axes, const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& extents);
	}
}
#endif