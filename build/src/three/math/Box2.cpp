#include "Box2.h"
#include <cmath>
namespace three {
	namespace math {
		Vector2 _vector;

		Box2::Box2() 
		{
			min.set(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
			max.set(-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity());
		}
		Box2& three::math::Box2::set(const Vector2& min, const Vector2& max)
		{
			this->min.copy(min);
			this->max.copy(max);

			return *this;
		}
		Box2& Box2::setFromPoints(const std::vector<Vector2> points)
		{

			makeEmpty();

			for (unsigned i = 0; i < points.size(); i++) 
			{
				expandByPoint(points[i]);
			}

			return *this;
		}
		Box2& Box2::setFromPoints(const Vector2* points,unsigned const length)
		{
			makeEmpty();

			for (unsigned i = 0; i < length; i++) {

				expandByPoint(points[i]);

			}

			return *this;
		}
		Box2& Box2::setFromCenterAndSize(const Vector2& center, const Vector2& size)
		{
			Vector2 halfSize = _vector.copy(size).multiplyScalar(0.5f);
			this->min.copy(center);
			this->min.sub(halfSize);

			this->max.copy(center);
			this->max.add(halfSize);

			return *this;
		}
		Box2& Box2::clone(Box2* target)
		{
			*target = copy(*this);

			return *target;
		}
		Box2& Box2::copy(const Box2& box)
		{
			min.copy(box.min);
			max.copy(box.max);

			return *this;
		}
		Box2& Box2::makeEmpty()
		{
			min.x = min.y = std::numeric_limits<float>::infinity();
			max.x = max.y = -std::numeric_limits<float>::infinity();

			return *this;
		}
		bool Box2::isEmpty()
		{
			return (max.x < min.x) || (max.y < min.y);
		}
		Vector2& Box2::getCenter(Vector2* target)
		{
			return isEmpty() ? target->set(0, 0) : target->addVectors(min,max).multiplyScalar(0.5f);
		}
		Vector2& Box2::getSize(Vector2* target)
		{
			return isEmpty() ? target->set(0, 0) : target->subVectors(max, min);
		}
		Box2& Box2::expandByPoint(const Vector2& point)
		{
			min.min(point);
			max.max(point);

			return *this;
		}
		Box2& Box2::expandByVector(const Vector2& vector)
		{
			min.sub(vector);
			max.add(vector);

			return *this;
		}
		Box2& Box2::expandByScalar(const float scalar)
		{
			min.addScalar(-scalar);
			max.addScalar(scalar);

			return *this;
		}
		bool Box2::containsPoint(const Vector2& point)
		{
			return point.x < min.x || point.x > max.x ||
				point.y < min.y || point.y > max.y ? false : true;

		}
		bool Box2::containsBox(const Box2& box)
		{
			return min.x <= box.min.x && box.max.x <= max.x &&
				min.y <= box.min.y && box.max.y <= max.y;
		}
		Vector2& Box2::getParameter(const Vector2& point, Vector2* target)
		{
			return target->set(
				(point.x - min.x) / (max.x - min.x),
				(point.y - min.y) / (max.y - min.y)
			);
		}
		bool Box2::intersectsBox(const Box2& box)
		{
			return box.max.x < min.x || box.min.x > max.x ||
				box.max.y < min.y || box.min.y > max.y ? false : true;
		}
		Vector2& Box2::clampPoint(const Vector2& point, Vector2* target)
		{
			return target->copy(point).clamp(min, max);
		}
		float Box2::distanceToPoint(const Vector2& point)
		{
			Vector2 clampedPoint = _vector.copy(point).clamp(min, max);
			return clampedPoint.sub(point).length();
		}
		Box2& Box2::intersect(const Box2& box)
		{
			min.max(box.min);
			max.min(box.max);

			return *this;
		}
		Box2& Box2::Union(const Box2& box)
		{
			min.min(box.min);
			max.max(box.max);

			return *this;
		}
		Box2& Box2::translate(const Vector2& offset)
		{
			min.add(offset);
			max.add(offset);

			return *this;
		}
		bool Box2::equals(const Box2& box)
		{
			return box.min.equals(min) && box.max.equals(max);
		}
	}
}