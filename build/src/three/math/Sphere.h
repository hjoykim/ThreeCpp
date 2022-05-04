#ifndef SPHERE_THREE_H
#define SPHERE_THREE_H
#include <three/math/Vector3.h>
#include <vector>
namespace three {
	namespace math {

		class Box3;

		class Plane;

		class Sphere {
		public:
			Vector3 center;
			float radius;

			Sphere() : center(Vector3(0, 0, 0)), radius(-1) {}

			Sphere(Vector3& center, float radius=-1): center(center), radius(radius) {}

			virtual ~Sphere() = default;

			Sphere& set(Vector3& center, float radius);

			Sphere& setFromPoints(std::vector<Vector3>& points, Vector3* optionalCenter = nullptr);

			Sphere& setFromPoints(Vector3* points,unsigned length,Vector3* optionalCenter=nullptr);

			Sphere& clone(Sphere* sphere);

			Sphere& copy(const Sphere& sphere);

			bool isEmpty();

			Sphere& makeEmpty();

			bool containsPoint(Vector3& point);

			float distanceToPoint(Vector3& point);

			bool intersectsSphere(Sphere& sphere);
				
			bool intersectsBox(Box3& box);;

			bool intersectsPlane(Plane& plane) const;

			Vector3& clampPoint(Vector3& point, Vector3* target);

			Box3& getBoundingBox(Box3* target);

			Sphere& applyMatrix4(Matrix4& matrix);

			Sphere& translate(Vector3& offset);

			bool equals(Sphere& sphere);
		};
	}
}
#endif
