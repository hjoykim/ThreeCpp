#ifndef PLANE_THREE_H
#define PLANE_THREE_H
#include <three/math/Vector3.h>
namespace three {
	namespace math {

		class Sphere;

		class Line3;

		class Box3;

		class Plane {

		public:
			Vector3 normal;

			float constant;

			Plane():normal(Vector3(1,0,0)),constant(0){}

			Plane(const Vector3& normal, const float constant=0) :normal(normal), constant(constant) {}

			virtual ~Plane() = default;

			Plane& set(const Vector3& normal, const float constant);

			Plane& setComponents(const float x, const float y, const float z, const float w);

			Plane& setFromNormalAndCoplanarPoint(const Vector3& normal, const Vector3& point);

			Plane& setFromCoplanarPoints(const Vector3& a, const Vector3& b, const Vector3& c);

			Plane& clone(Plane* target);

			Plane& copy(const Plane& plane);

			Plane& normalize();

			Plane& negate();

			float distanceToPoint(const Vector3& point);

			float distanceToSphere(const Sphere& sphere);

			Vector3& projectPoint(const Vector3& point, Vector3* target);

			Vector3& intersectLine(const Line3& line, Vector3* target);

			bool intersectsLine(const Line3& line);

			bool intersectsBox(const Box3& box);

			bool intersectsSphere(const Sphere& sphere);

			Vector3& coplanarPoint(Vector3* target);

			Plane& applyMatrix4(const Matrix4& matrix, Matrix3* optionalNormalMatrix=nullptr);

			Plane& translate(const Vector3& offset);

			bool equals(const Plane& plane);

		};
	}
}
#endif