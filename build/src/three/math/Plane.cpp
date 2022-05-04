#include "Plane.h"
#include "Sphere.h"
#include "Line3.h"
#include "Box3.h"
#include "Matrix3.h"
#include "Matrix4.h"

namespace three {	
	namespace math {
		namespace plane {
			Vector3 _vector1;
			Vector3 _vector2;
			Matrix3 _normalMatrix;
		}
		using namespace plane;
		Plane& three::math::Plane::set(const Vector3& normal, const float constant)
		{
			this->normal.copy(normal);
			this->constant = constant;

			return *this;
		}
		Plane& Plane::setComponents(const float x, const float y, const float z, const float w)
		{
			normal.set(x, y, z);
			constant = w;

			return *this;
		}
		Plane& Plane::setFromNormalAndCoplanarPoint(const Vector3& normal, const Vector3& point)
		{
			this->normal.copy(normal);
			this->constant = -point.dot(this->normal);

			return *this;
		}
		Plane& Plane::setFromCoplanarPoints(const Vector3& a, const Vector3& b, const Vector3& c)
		{
			Vector3 normal = _vector1.subVectors(c, b).cross(_vector2.subVectors(a, b)).normalize();

			// Q: should an error be thrown if normal is zero (e.g. degenerate plane)?

			this->setFromNormalAndCoplanarPoint(normal, a);

			return *this;
		}
		Plane& Plane::clone(Plane* target)
		{
			target->copy(*this);
			return *target;
		}
		Plane& Plane::copy(const Plane& plane)
		{
			this->normal.copy(plane.normal);
			this->constant = plane.constant;

			return *this;
		}

		Plane& Plane::normalize()
		{
			float inverseNormalLength = 1.0f / this->normal.length();
			this->normal.multiplyScalar(inverseNormalLength);
			this->constant *= inverseNormalLength;

			return *this;
		}
		Plane& Plane::negate()
		{
			constant *= -1;
			normal.negate();

			return *this;
		}
		float Plane::distanceToPoint(const Vector3& point)
		{
			return normal.dot(point) + constant;
		}
		float Plane::distanceToSphere(const Sphere& sphere)
		{
			return distanceToPoint(sphere.center) - sphere.radius;
		}
		Vector3& Plane::projectPoint(const Vector3& point, Vector3* target)
		{
			target->copy(normal).multiplyScalar(-distanceToPoint(point)).add(point);

			return *target;
		}

		Vector3& Plane::intersectLine(const Line3& line, Vector3* target)
		{
			Vector3 direction = line.delta(&_vector1);

			float denominator = normal.dot(direction);

			if (denominator ==  0.0f) {

				// line is coplanar, return origin
				if (distanceToPoint(line.start) ==  0.0f) {

					target->copy(line.start);
					return *target;

				}

				// Unsure if this is the correct method to handle this case.
				return *target;

			}

			float t = -(line.start.dot(normal) + constant) / denominator;

			if (t < 0 || t > 1) {

				return *target;

			}

			target->copy(direction).multiplyScalar(t).add(line.start);

			return *target;
		}
		bool Plane::intersectsLine(const Line3& line)
		{// Note: this tests if a line intersects the plane, not whether it (or its end-points) are coplanar with it.

			float startSign = distanceToPoint(line.start);
			float endSign = distanceToPoint(line.end);

			return (startSign < 0 && endSign > 0) || (endSign < 0 && startSign > 0);			
		}
		bool Plane::intersectsBox(const Box3& box)
		{
			return box.intersectsPlane(*this);
		}
		bool Plane::intersectsSphere(const Sphere& sphere)
		{
			return sphere.intersectsPlane(*this);
		}
		Vector3& Plane::coplanarPoint(Vector3* target)
		{
			target->copy(normal).multiplyScalar(-constant);

			return *target;
		}
		Plane& Plane::applyMatrix4(const Matrix4& matrix, Matrix3* optionalNormalMatrix)
		{
			Matrix3 normalMatrix = optionalNormalMatrix!=nullptr ? *optionalNormalMatrix : _normalMatrix.getNormalMatrix(matrix);
						
			Vector3 referencePoint = coplanarPoint(&_vector1).applyMatrix4(matrix);

			Vector3 _normal = this->normal.applyMatrix3(normalMatrix).normalize();

			constant = -referencePoint.dot(_normal);

			return *this;
		}
		Plane& Plane::translate(const Vector3& offset)
		{
			constant -= offset.dot(normal);

			return *this;
		}
		bool Plane::equals(const Plane& plane)
		{
			return plane.normal.equals(normal) && (plane.constant == constant);
		}
	}
}
