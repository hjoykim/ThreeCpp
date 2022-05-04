#ifndef BOX2_THREE_H
#define BOX2_THREE_H
#include <three/math/Vector2.h>
namespace three {
	namespace math {

		class Box2 {

		public:

			Vector2 max;

			Vector2 min;

			Box2();

			Box2(const Vector2& min, const Vector2& max ) : min(min),max(max){}

			virtual ~Box2() = default;

			Box2& set(const Vector2& min, const Vector2& max);

			Box2& setFromPoints(const std::vector<Vector2> points);

			Box2& setFromPoints(const Vector2* points,unsigned const length);

			Box2& setFromCenterAndSize(const Vector2& center, const Vector2& size);

			Box2& clone(Box2* target);

			Box2& copy(const Box2& box);

			Box2& makeEmpty();

			bool isEmpty();

			Vector2& getCenter(Vector2* target);

			Vector2& getSize(Vector2* target);

			Box2& expandByPoint(const Vector2& point);

			Box2& expandByVector(const Vector2& vector);

			Box2& expandByScalar(const float scalar);

			bool containsPoint(const Vector2& point);

			bool containsBox(const Box2& box);

			Vector2& getParameter(const Vector2& point, Vector2* target);

			bool intersectsBox(const Box2& box);

			Vector2& clampPoint(const Vector2& point, Vector2* target);

			float distanceToPoint(const Vector2& point);

			Box2& intersect(const Box2& box);

			Box2& Union(const Box2& box);

			Box2& translate(const Vector2& offset);

			bool equals(const Box2& box);
		};
	}
}
#endif