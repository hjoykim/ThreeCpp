#ifndef TRIANGLES_THREE_H
#define TRIANGLES_THREE_H
#include <three/math/Vector3.h>
namespace three {
	namespace math {

		typedef struct _SplineControlPoint {
			float x, y, z;
		} SplineControlPoint;

		class Plane;
		class Vector2;
		class Box3;

		class Triangle {
		public:
			Vector3 a;
			Vector3 b;
			Vector3 c;

			Triangle() :a(Vector3()),b(Vector3()),c(Vector3()){}

			Triangle(Vector3 a, Vector3 b, Vector3 c) : a(a), b(b), c(c) {}

			virtual ~Triangle() = default;

			Triangle& set(Vector3 a, Vector3 b, Vector3 c);

			Triangle& setFromPointsAndIndices(Vector3* points,unsigned i0,	unsigned i1,unsigned i2);

			Triangle& clone(Triangle* triangle);
			
			Triangle& copy(Triangle& triangle);

			float getArea();

			Vector3& getMidpoint(Vector3* target);

			Vector3& getNormal(Vector3* target);

			Plane& getPlane(Plane* target);

			Vector3& getBarycoord(Vector3& point, Vector3& a, Vector3& b, Vector3& c, Vector3* target);

			Vector3& getBarycoord(Vector3& point, Vector3* target);


			Vector2& getUV(Vector3& point, Vector2& uv1, Vector2& uv2, Vector2& uv3, Vector2* target);

			bool containsPoint(Vector3& point);

			bool intersectsBox(Box3& box3);

			bool isFrontFacing(Vector3& direction);

			Vector3& closestPointToPoint(Vector3& point, Vector3* target);

			bool equals(Triangle& triangle);

			Vector3& getNormal(Vector3& a,Vector3& b,Vector3& c,	Vector3* target);


			bool containsPoint(Vector3& point, Vector3& a, Vector3& b, Vector3& c);
				
			Vector2& getUV(Vector3& point,Vector3& p1,Vector3& p2,Vector3 & p3,Vector2& uv1,Vector2& uv2,Vector2& uv3,Vector2* target);

			bool isFrontFacing(Vector3& a, Vector3& b, Vector3& c, Vector3& direction);
		};
	}
}
#endif
