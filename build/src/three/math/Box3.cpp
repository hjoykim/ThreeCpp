#include "Box3.h"
#include <three/core/BufferAttribute.h>
#include <three/core/Object3D.h>
#include <three/math/Plane.h>
#include <three/math/Triangle.h>
namespace three {
	
	namespace math {
		namespace box3 {
			Vector3 _points[] = { Vector3(), Vector3(), Vector3(), Vector3(), Vector3(), Vector3(), Vector3(), Vector3() };
			Vector3 _vector;
			Box3 _box;
			Vector3 _center, _extents, _triangleNormal, _testAxis;
			Vector3 _v0, _v1, _v2;
			Vector3 _f0, _f1, _f2;
		}
		Box3::Box3()
		{
			min.set(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
			max.set(-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity());
		}
		Box3& three::math::Box3::set(const Vector3& min, const Vector3& max)
		{
			this->min.copy(min);
			this->max.copy(max);

			return *this;
		}
		Box3& Box3::setFromArray(const float* array, const unsigned length)
		{
			float minX = std::numeric_limits<float>::infinity();
			float minY = std::numeric_limits<float>::infinity();
			float minZ = std::numeric_limits<float>::infinity();

			float maxX = -std::numeric_limits<float>::infinity();
			float maxY = -std::numeric_limits<float>::infinity();
			float maxZ = -std::numeric_limits<float>::infinity();

			for (unsigned i = 0; i < length; i += 3) {

				float x = array[i];
				float y = array[i + 1];
				float z = array[i + 2];

				if (x < minX) minX = x;
				if (y < minY) minY = y;
				if (z < minZ) minZ = z;

				if (x > maxX) maxX = x;
				if (y > maxY) maxY = y;
				if (z > maxZ) maxZ = z;

			}

			this->min.set(minX, minY, minZ);
			this->max.set(maxX, maxY, maxZ);

			return *this;
		}
		Box3& Box3::setFromBufferAttribute(const BufferAttribute<float>& attribute)
		{
			float minX = std::numeric_limits<float>::infinity();
			float minY = std::numeric_limits<float>::infinity();
			float minZ = std::numeric_limits<float>::infinity();

			float maxX = -std::numeric_limits<float>::infinity();
			float maxY = -std::numeric_limits<float>::infinity();
			float maxZ = -std::numeric_limits<float>::infinity();

			for (int i = 0, l = attribute.count; i < l; i++) {

				float x = attribute.getX(i);
				float y = attribute.getY(i);
				float z = attribute.getZ(i);

				if (x < minX) minX = x;
				if (y < minY) minY = y;
				if (z < minZ) minZ = z;

				if (x > maxX) maxX = x;
				if (y > maxY) maxY = y;
				if (z > maxZ) maxZ = z;

			}

			this->min.set(minX, minY, minZ);
			this->max.set(maxX, maxY, maxZ);

			return *this;
		}
		Box3& Box3::setFromPoints(const Vector3* points, const unsigned length)
		{
			makeEmpty();

			for (unsigned i = 0; i < length; i++) {

				expandByPoint(points[i]);

			}

			return *this;
		}
		Box3& Box3::setFromCenterAndSize(const Vector3& center, const Vector3& size)
		{
			Vector3 halfSize = box3::_vector.copy(size).multiplyScalar(0.5f);

			min.copy(center).sub(halfSize);
			max.copy(center).add(halfSize);

			return *this;
		}
		Box3& Box3::setFromObject(Object3D& object)
		{
			makeEmpty();

			return expandByObject(object);
		}
		Box3& Box3::clone(Box3* target)
		{
			*target = copy(*this);

			return *target;
		}
		Box3& Box3::copy(const Box3& box)
		{
			this->min.copy(box.min);
			this->max.copy(box.max);

			return *this;
		}
		Box3& Box3::makeEmpty()
		{
			min.x = min.y = min.z = std::numeric_limits<float>::infinity();
			max.x = max.y = max.z = -std::numeric_limits<float>::infinity();

			return *this;
		}
		bool Box3::isEmpty()
		{
			// this is a more robust check for empty than ( volume <= 0 ) because volume can get positive with two negative axes

			return (max.x < min.x) || (max.y < min.y) || (max.z < min.z);

		}
		Vector3& Box3::getCenter(Vector3* target)
		{
			if (isEmpty())
				target->set(0, 0, 0);
			else
				target->addVectors(min, max).multiplyScalar(0.5f);

			return *target;
	
		}
		Vector3& Box3::getSize(Vector3* target)
		{
			return isEmpty() ? target->set(0, 0, 0) : target->subVectors(max, min);
		}
		Box3& Box3::expandByPoint(const Vector3& point)
		{
			min.min(point);
			max.max(point);

			return *this;
		}
		Box3& Box3::expandByVector(const Vector3& vector)
		{
			min.sub(vector);
			max.add(vector);

			return *this;
		}
		Box3& Box3::expandByScalar(float scalar)
		{
			min.addScalar(-scalar);
			max.addScalar(scalar);

			return *this;
		}
		Box3& Box3::expandByObject(Object3D& object)
		{
			// Computes the world-axis-aligned bounding box of an object (including its children),
			// accounting for both the object's, and children's, world transforms

			object.updateWorldMatrix(false, false);

			Geometry::ptr geometry = object.geometry;

			if (geometry != nullptr) {

				if (geometry->boundingBox.isEmpty()) {

					geometry->computeBoundingBox();

				}

				box3::_box.copy(geometry->boundingBox);
				box3::_box.applyMatrix4(object.matrixWorld);

				this->Union(box3::_box);

			}

			auto children = object.children;

			for (unsigned i = 0;i<children.size();i++) {

				expandByObject(*children[i]);

			}

			return *this;
		}
		bool Box3::containsPoint(const Vector3& point)
		{
			return point.x < min.x || point.x > max.x ||
				point.y < min.y || point.y > max.y ||
				point.z < min.z || point.z > max.z ? false : true;
		}
		bool Box3::containsBox(const Box3& box)
		{
			return min.x <= box.min.x && box.max.x <= max.x &&
				min.y <= box.min.y && box.max.y <= max.y &&
				min.z <= box.min.z && box.max.z <= max.z;
		}
		void Box3::getParameter(const Vector3& point, Vector3* target)
		{
			target->set(
				(point.x - min.x) / (max.x - min.x),
				(point.y - min.y) / (max.y - min.y),
				(point.z - min.z) / (max.z - min.z)
			);
		}
		bool Box3::intersectsBox(const Box3& box)
		{
			return box.max.x < min.x || box.min.x > max.x ||
				box.max.y < min.y || box.min.y > max.y ||
				box.max.z < min.z || box.min.z > max.z ? false : true;
		}
		bool Box3::intersectsSphere(const Sphere& sphere)
		{
			// Find the point on the AABB closest to the sphere center.
			clampPoint(sphere.center, &box3::_vector);

			// If that point is inside the sphere, the AABB and sphere intersect.
			return box3::_vector.distanceToSquared(sphere.center) <= (sphere.radius * sphere.radius);
		}
		bool Box3::intersectsPlane(const Plane& plane) const
		{
			// We compute the minimum and maximum dot product values. If those values
			// are on the same side (back or front) of the plane, then there is no intersection.

			float min, max;

			if (plane.normal.x > 0) {

				min = plane.normal.x * this->min.x;
				max = plane.normal.x * this->max.x;

			}
			else {

				min = plane.normal.x * this->max.x;
				max = plane.normal.x * this->min.x;

			}

			if (plane.normal.y > 0) {

				min += plane.normal.y * this->min.y;
				max += plane.normal.y * this->max.y;

			}
			else {

				min += plane.normal.y * this->max.y;
				max += plane.normal.y * this->min.y;

			}

			if (plane.normal.z > 0) {

				min += plane.normal.z * this->min.z;
				max += plane.normal.z * this->max.z;

			}
			else {

				min += plane.normal.z * this->max.z;
				max += plane.normal.z * this->min.z;

			}

			return (min <= -plane.constant && max >= -plane.constant);
		}
		bool Box3::intersectsTriangle(const Triangle& triangle)
		{
			if (isEmpty()) {

				return false;

			}

			// compute box center and extents
			getCenter(&box3::_center);
			box3::_extents.subVectors(max, box3::_center);

			// translate triangle to aabb origin
			box3::_v0.subVectors(triangle.a, box3::_center);
			box3::_v1.subVectors(triangle.b, box3::_center);
			box3::_v2.subVectors(triangle.c, box3::_center);

			// compute edge vectors for triangle
			box3::_f0.subVectors(box3::_v1, box3::_v0);
			box3::_f1.subVectors(box3::_v2, box3::_v1);
			box3::_f2.subVectors(box3::_v0, box3::_v2);

			// test against axes that are given by cross product combinations of the edges of the triangle and the edges of the aabb
			// make an axis testing of each of the 3 sides of the aabb against each of the 3 sides of the triangle = 9 axis of separation
			// axis_ij = u_i x f_j (u0, u1, u2 = face normals of aabb = x,y,z axes vectors since aabb is axis aligned)
			/*float axes[] = {
				0, -_f0.z, _f0.y, 0, -_f1.z, _f1.y, 0, -_f2.z, _f2.y,
					_f0.z, 0, -_f0.x, _f1.z, 0, -_f1.x, _f2.z, 0, -_f2.x,
					-_f0.y, _f0.x, 0, -_f1.y, _f1.x, 0, -_f2.y, _f2.x, 0
			};*/
			std::vector<float> axes = { 0, -box3::_f0.z, box3::_f0.y, 0, -box3::_f1.z, box3::_f1.y, 0, -box3::_f2.z, box3::_f2.y,
										box3::_f0.z, 0, -box3::_f0.x, box3::_f1.z, 0, -box3::_f1.x, box3::_f2.z, 0, -box3::_f2.x,
										-box3::_f0.y, box3::_f0.x, 0, -box3::_f1.y, box3::_f1.x, 0, -box3::_f2.y, box3::_f2.x, 0 };

			if (!satForAxes(axes, box3::_v0, box3::_v1, box3::_v2, box3::_extents)) {

				return false;

			}

			// test 3 face normals from the aabb
			axes.clear();
			axes = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
			if (!satForAxes(axes, box3::_v0, box3::_v1, box3::_v2, box3::_extents)) {

				return false;

			}

			// finally testing the face normal of the triangle
			// use already existing triangle edge vectors here
			box3::_triangleNormal.crossVectors(box3::_f0, box3::_f1);
			axes.clear();
			axes = { box3::_triangleNormal.x, box3::_triangleNormal.y, box3::_triangleNormal.z };

			return satForAxes(axes, box3::_v0, box3::_v1, box3::_v2, box3::_extents);
		}
		void Box3::clampPoint(const Vector3& point, Vector3* target)
		{
			target->copy(point);
			target->clamp(min, max);
		}

		float Box3::distanceToPoint(const Vector3& point)
		{
			Vector3 clampedPoint = box3::_vector.copy(point);
			clampedPoint.clamp(min, max);

			return clampedPoint.sub(point).length();
		}

		Sphere& Box3::getBoundingSphere(Sphere* target)
		{
			getCenter(&(target->center));

			target->radius = getSize(&box3::_vector).length() * 0.5f;

			return *target;
		}

		Box3& Box3::intersect(const Box3& box)
		{
			min.max(box.min);
			max.min(box.max);

			// ensure that if there is no overlap, the result is fully empty, not slightly empty with non-inf/+inf values that will cause subsequence intersects to erroneously return valid values.
			if (isEmpty()) makeEmpty();

			return *this;
		}

		Box3& Box3::Union(const Box3& box)
		{
			min.min(box.min);
			max.max(box.max);

			return *this;
		}

		Box3& Box3::applyMatrix4(const Matrix4& matrix)
		{
			// transform of empty box is an empty box.
			if (isEmpty()) return *this;

			// NOTE: I am using a binary pattern to specify all 2^3 combinations below
			box3::_points[0].set(min.x, min.y, min.z).applyMatrix4(matrix); // 000
			box3::_points[1].set(min.x, min.y, max.z).applyMatrix4(matrix); // 001
			box3::_points[2].set(min.x, max.y, min.z).applyMatrix4(matrix); // 010
			box3::_points[3].set(min.x, max.y, max.z).applyMatrix4(matrix); // 011
			box3::_points[4].set(max.x, min.y, min.z).applyMatrix4(matrix); // 100
			box3::_points[5].set(max.x, min.y, max.z).applyMatrix4(matrix); // 101
			box3::_points[6].set(max.x, max.y, min.z).applyMatrix4(matrix); // 110
			box3::_points[7].set(max.x, max.y, max.z).applyMatrix4(matrix); // 111

			setFromPoints(box3::_points,8);

			return *this;
		}

		Box3& Box3::translate(const Vector3& offset)
		{
			min.add(offset);
			max.add(offset);

			return *this;
		}

		bool Box3::equals(const Box3& box)
		{
			return box.min.equals(min) && box.max.equals(max);
		}

		bool satForAxes(const std::vector<float> axes, const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& extents) {

			for (int i = 0, j = axes.size() - 3; i <= j; i += 3) {

				box3::_testAxis.fromArray(axes, i);
				// project the aabb onto the seperating axis
				float r = extents.x * abs(box3::_testAxis.x) + extents.y * abs(box3::_testAxis.y) + extents.z * abs(box3::_testAxis.z);
				// project all 3 vertices of the triangle onto the seperating axis
				float p0 = v0.dot(box3::_testAxis);
				float p1 = v1.dot(box3::_testAxis);
				float p2 = v2.dot(box3::_testAxis);
				// actual test, basically see if either of the most extreme of the triangle points intersects r
				if (std::max(-std::max(p0, std::max(p1, p2)), std::min(p0, std::min(p1, p2))) > r) {

					// points of the projected triangle are outside the projected half-length of the aabb
					// the axis is seperating and we can exit
					return false;

				}

			}

			return true;
		}
	}
}
