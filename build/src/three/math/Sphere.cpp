#include "Sphere.h"
#include "Box3.h"
#include "Plane.h"
#include "Matrix4.h"

namespace three {
	namespace math {
		namespace sphere {
			Box3 _box;
		}
		using namespace sphere;

		Sphere& three::math::Sphere::set(Vector3& center, float radius)
		{
			this->center.copy(center);
			this->radius = radius;

			return *this;
		}
		Sphere& Sphere::setFromPoints(std::vector<Vector3>& points, Vector3* optionalCenter)
		{
			if (optionalCenter != nullptr) {
				center.copy(*optionalCenter);
			}
			else {
				_box.setFromPoints(&points[0], points.size()).getCenter(&center);
			}

			float maxRadiusSq = 0;

			for (unsigned i = 0; i < points.size();i++) {

				maxRadiusSq = std::max(maxRadiusSq, center.distanceToSquared(points[i]));

			}

			radius = std::sqrt(maxRadiusSq);

			return *this;
		}
		Sphere& Sphere::setFromPoints(Vector3* points, unsigned length,Vector3* optionalCenter)
		{

			if (optionalCenter != nullptr) {
				center.copy(*optionalCenter);
			}
			else {
				_box.setFromPoints(points,length).getCenter(&center);
			}

			float maxRadiusSq = 0;

			for (unsigned i = 0; i<length;i++) {

				maxRadiusSq = std::max(maxRadiusSq, center.distanceToSquared(points[i]));

			}

			radius = std::sqrt(maxRadiusSq);

			return *this;
		}

		Sphere& Sphere::clone(Sphere* sphere)
		{
			sphere->copy(*this);
			return *sphere;
		}

		Sphere& Sphere::copy(const Sphere& sphere)
		{
			center.copy(sphere.center);
			radius = sphere.radius;

			return *this;
		}
		bool Sphere::isEmpty()
		{
			return radius < 0;
		}

		Sphere& Sphere::makeEmpty()
		{
			center.set(0, 0, 0);
			radius = -1;

			return *this;
		}
		
		bool Sphere::containsPoint(Vector3& point)
		{			
			return (point.distanceToSquared(center) - (radius * radius)) <=0.000001f;
		}
		float Sphere::distanceToPoint(Vector3& point)
		{
			return (point.distanceTo(center) - radius);
		}
		bool Sphere::intersectsSphere(Sphere& sphere)
		{
			float radiusSum = radius + sphere.radius;

			return sphere.center.distanceToSquared(center) <= (radiusSum * radiusSum);
		}
		bool Sphere::intersectsBox(Box3& box)
		{
			return box.intersectsSphere(*this);
		}
		bool Sphere::intersectsPlane(Plane& plane) const
		{
			return std::abs(plane.distanceToPoint(center)) <= radius;
		}
		Vector3& Sphere::clampPoint(Vector3& point, Vector3* target)
		{
			float deltaLengthSq = center.distanceToSquared(point);

			target->copy(point);

			if (deltaLengthSq > (radius * radius)) {

				target->sub(center).normalize();
				target->multiplyScalar(radius).add(center);

			}

			return *target;
		}
		Box3& Sphere::getBoundingBox(Box3* target)
		{
			target->set(center, center);
			target->expandByScalar(radius);

			return *target;
		}
		Sphere& Sphere::applyMatrix4(Matrix4& matrix)
		{
			center.applyMatrix4(matrix);
			radius = radius * matrix.getMaxScaleOnAxis();

			return *this;
		}
		Sphere& Sphere::translate(Vector3& offset)
		{
			center.add(offset);

			return *this;
		}
		bool Sphere::equals(Sphere& sphere)
		{
			return sphere.center.equals(center) && (sphere.radius == radius);
		}
	}
}
