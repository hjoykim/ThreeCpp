#include "Line3.h"
namespace three {
	namespace math {
		namespace line3 {
			Vector3 _startP;
			Vector3 _startEnd;
		}
		using namespace line3;
		Line3& three::math::Line3::set(const Vector3& start, const Vector3& end)
		{
			this-> start.copy(start);
			this->end.copy(end);

			return *this;
		}
		Line3& Line3::clone(Line3* target)
		{
			target->copy(*this);

			return *target;
		}
		Line3& Line3::copy(const Line3& line)
		{
			start.copy(line.start);
			end.copy(line.end);

			return *this;
		}
		Vector3& Line3::getCenter(Vector3* target)
		{
			target->addVectors(start, end).multiplyScalar(0.5f);

			return *target;
		}
		Vector3& Line3::delta(Vector3* target) const
		{
			target->subVectors(end, start);

			return *target;
		}
		float Line3::distanceSq() const
		{
			return start.distanceToSquared(end);			
		}
		float Line3::distance() const
		{
			return start.distanceTo(end);
		}
		Vector3& Line3::at(float t, Vector3* target)
		{
			return delta(target).multiplyScalar(t).add(start);
		}
		float Line3::closestPointToPointParameter(const Vector3& point, bool clampToLine)
		{
			_startP.subVectors(point, start);
			_startEnd.subVectors(end, start);

			float startEnd2 = _startEnd.dot(_startEnd);
			float startEnd_startP = _startEnd.dot(_startP);

			float t = startEnd_startP / startEnd2;

			if (clampToLine) {

				t = clamp(t, 0.0f, 1.0f);

			}

			return t;
		}
		Vector3& Line3::closestPointToPoint(const Vector3& point, const bool clampToLine, Vector3* target)
		{
			float t = closestPointToPointParameter(point, clampToLine);
			*target = delta(target);
			target->multiplyScalar(t);
			target->add(this->start);
			return *target;
		}
		Line3& Line3::applyMatrix4(const Matrix4& matrix)
		{
			start.applyMatrix4(matrix);
			end.applyMatrix4(matrix);

			return *this;
		}
		bool Line3::equals(const Line3& line)
		{
			return line.start.equals(start) && line.end.equals(end);
		}
	}
}
