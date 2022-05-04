#ifndef FRUSTUM_THREE_H
#define FRUSTUM_THREE_H
#include <three/math/Plane.h>
namespace three {

	class Object3D;	
	class Sprite;

	namespace math {

		class Matrix4;
		class Sphere;
		class Box3;
		class Vector3;

		class Frustum {
		public:			
			Plane planes[6] = { Plane(),Plane(),Plane(),Plane(),Plane(),Plane() };

			
			Frustum() {};		
			

			Frustum(const Plane& p0, const Plane& p1, const Plane& p2, const Plane& p3, const Plane& p4, const Plane& p5) {
				planes[0] = p0;
				planes[1] = p1;
				planes[2] = p2;
				planes[3] = p3;
				planes[4] = p4;
				planes[5] = p5;
			}

			virtual ~Frustum() {};

			Frustum& set(const Plane& p0, const Plane& p1, const Plane& p2, const Plane& p3, const Plane& p4, const Plane& p5);

			Frustum& clone(Frustum* f);

			Frustum& copy(const Frustum& frustum);

			Frustum& setFromProjectionMatrix(const Matrix4& m);

			bool intersectsObject(Object3D& object);

			bool intersectsSprite(const Sprite& sprite);

			bool intersectsSphere(const Sphere& sphere);

			bool intersectsBox(const Box3& box);

			bool containsPoint(const Vector3& point);

		};
	}
}
#endif
