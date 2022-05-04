#ifndef RAY_THREE_H
#define RAY_THREE_H
#include <three/math/Vector3.h>
namespace three {
	namespace math {
		class Matrix4;
		class Plane;
		class Box3;
		class Sphere;

		class Ray {
		public:
			Vector3 origin;
			Vector3 direction;
		 
			Ray(Vector3& origin, Vector3& direction) : origin(origin), direction(direction) {}

			virtual ~Ray() = default;

			Ray& set(Vector3& origin,Vector3& direction);

			void clone(Ray& ray);

			Ray& copy(Ray& Ray);

			Vector3& at(float t, Vector3& target);

			Ray& lookAt(Vector3& v);

			Ray& recast(float t);

			Vector3& closestPointToPoint(Vector3& point, Vector3& target);

			float distanceToPoint(Vector3& point);

			float distanceSqToPoint(Vector3& point);

			float distanceSqToSegment(Vector3& v0,Vector3& v1,Vector3* optionalPointOnRay=nullptr,Vector3* optionalPointOnSegment=nullptr);

			Vector3& intersectSphere(Sphere& sphere, Vector3& target);

			bool intersectsSphere(Sphere& sphere);

			float distanceToPlane(Plane& plane);

			Vector3& intersectPlane(Plane& plane, Vector3& target);

			bool intersectsPlane(Plane& plane);

			Vector3& intersectBox(Box3& box3, Vector3& target);
			
			bool intersectsBox(Box3& box3);

			Vector3 intersectTriangle(Vector3& a,Vector3& b,Vector3& c,	bool backfaceCulling,Vector3& target);

			Ray& applyMatrix4(Matrix4& matrix4);

			bool equals(Ray& ray);

		};
	}
}
#endif