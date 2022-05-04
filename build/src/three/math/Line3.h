#ifndef LINE3_THREE_H
#define LINE3_THREE_H
#include <three/math/Vector3.h>
namespace three {
	namespace math {

		class Line3 {

		public:

			Vector3 start;

			Vector3 end;

			Line3() :start(Vector3()), end(Vector3()) {}

			Line3(const Vector3& start, const Vector3& end) : start(start), end(end) {}

			virtual ~Line3() = default;

			Line3& set(const Vector3& start, const Vector3& end );

			Line3& clone(Line3* target);

			Line3& copy(const Line3& line);

			Vector3& getCenter(Vector3* target);

			Vector3& delta(Vector3* target) const;

			float distanceSq() const;

			float distance() const;

			Vector3& at(const float t, Vector3* target);

			float closestPointToPointParameter(const Vector3& point, const bool clampToLine=false);

			Vector3& closestPointToPoint(const Vector3& point, const bool clampToLine, Vector3* target);

			Line3& applyMatrix4(const Matrix4& matrix);

			bool equals(const Line3& line);

		};
	}
}
#endif