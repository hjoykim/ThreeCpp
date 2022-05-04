#include "Vector2.h"
#include "Matrix3.h"
namespace three
{
	namespace math {
		Vector2& Vector2::applyMatrix3(const Matrix3& m)
		{
			const float* e = m.elements;

			float _x = this->x;
			float _y = this->y;

			x = e[0] * _x + e[3] * _y + e[6];
			y = e[1] * _x + e[4] * _y + e[7];

			return *this;
		}
	}

}
