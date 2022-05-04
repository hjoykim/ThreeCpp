#include "Cylindrical.h"
#include "Vector3.h"
namespace three {
	namespace math {
		Cylindrical& three::math::Cylindrical::clone(Cylindrical* c)
		{
			c->copy(*this);

			return *this;
		}
		Cylindrical& Cylindrical::copy(const Cylindrical& other)
		{
			radius = other.radius;
			theta = other.theta;
			y = other.y;

			return *this;
		}
		Cylindrical& Cylindrical::set(const float radius, const float theta, const  float y)
		{
			this->radius = radius;
			this->theta = theta;
			this->y = y;

			return *this;
		}
		Cylindrical& Cylindrical::setFromVector3(const Vector3& vec3)
		{
			return setFromCartesianCoords(vec3.x, vec3.y, vec3.z);
		}
		Cylindrical& Cylindrical::setFromCartesianCoords(const float x, const float y, const float z)
		{
			radius = std::sqrt(x * x + z * z);
			theta = std::atan2(x, z);
			this->y = y;

			return *this;
		}
	}
}
