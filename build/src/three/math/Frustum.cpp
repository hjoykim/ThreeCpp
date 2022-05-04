#include "Frustum.h"
#include "Matrix4.h"
#include "Sphere.h"
#include "Box3.h"
#include "Vector3.h"
#include <three/core/Object3D.h>
#include <three/objects/Sprite.h>
namespace three {
	namespace math {
		namespace frustum {
			Sphere _sphere;
			Vector3 _vector;
		}
		using namespace frustum;

		Frustum& three::math::Frustum::set(const Plane& p0, const Plane& p1, const Plane& p2, const Plane& p3, const Plane& p4, const Plane& p5)
		{
			planes[0].copy(p0);
			planes[1].copy(p1);
			planes[2].copy(p2);
			planes[3].copy(p3);
			planes[4].copy(p4);
			planes[5].copy(p5);

			return *this;
		}
		Frustum& Frustum::clone(Frustum* f)
		{
			f->copy(*this);

			return *f;
		}
		Frustum& Frustum::copy(const Frustum& frustum)
		{
			for (int i = 0; i < 6; i++) {

				planes[i].copy(frustum.planes[i]);

			}

			return *this;
		}
		Frustum& Frustum::setFromProjectionMatrix(const Matrix4& m)
		{
			float me0 = m.elements[0], me1 = m.elements[1], me2 = m.elements[2], me3 = m.elements[3];
			float me4 = m.elements[4], me5 = m.elements[5], me6 = m.elements[6], me7 = m.elements[7];
			float me8 = m.elements[8], me9 = m.elements[9], me10 = m.elements[10], me11 = m.elements[11];
			float me12 = m.elements[12], me13 = m.elements[13], me14 = m.elements[14], me15 = m.elements[15];

			planes[0].setComponents(me3 - me0, me7 - me4, me11 - me8, me15 - me12).normalize();
			planes[1].setComponents(me3 + me0, me7 + me4, me11 + me8, me15 + me12).normalize();
			planes[2].setComponents(me3 + me1, me7 + me5, me11 + me9, me15 + me13).normalize();
			planes[3].setComponents(me3 - me1, me7 - me5, me11 - me9, me15 - me13).normalize();
			planes[4].setComponents(me3 - me2, me7 - me6, me11 - me10, me15 - me14).normalize();
			planes[5].setComponents(me3 + me2, me7 + me6, me11 + me10, me15 + me14).normalize();

			return *this;
		}
		bool Frustum::intersectsObject(three::Object3D& object)
		{
			auto geometry = object.geometry;

			if (geometry->boundingSphere.isEmpty() == true) 
				geometry->computeBoundingSphere();

			_sphere.copy(geometry->boundingSphere).applyMatrix4(object.matrixWorld);

			return this->intersectsSphere(_sphere);
		}
		bool Frustum::intersectsSprite(const three::Sprite& sprite)
		{
			return false;
		}
		bool Frustum::intersectsSphere(const Sphere& sphere)
		{
			Vector3 center = sphere.center;
			float negRadius = -sphere.radius;

			for (int i = 0; i < 6; i++) {

				float distance = planes[i].distanceToPoint(center);

				if (distance < negRadius) {

					return false;

				}
			}

			return true;
		}
		bool Frustum::intersectsBox(const Box3& box)
		{

			for (int i = 0; i < 6; i++) {

				Plane plane = planes[i];

				// corner at max distance

				_vector.x = plane.normal.x > 0 ? box.max.x : box.min.x;
				_vector.y = plane.normal.y > 0 ? box.max.y : box.min.y;
				_vector.z = plane.normal.z > 0 ? box.max.z : box.min.z;

				if (plane.distanceToPoint(_vector) < 0) {

					return false;

				}

			}

			return true;
		}
		bool Frustum::containsPoint(const Vector3& point)
		{
			for (int i = 0; i < 6; i++) {

				if (planes[i].distanceToPoint(point) < 0) {

					return false;

				}

			}

			return true;
		}
	}
}
