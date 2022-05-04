#include "Spherical.h"
#include "Vector3.h"
#include "MathUtils.h"

namespace three {
	namespace math {
		Spherical& three::math::Spherical::set(const float radius, const float phi, const float theta)
		{
			this->radius = radius;
			this->phi = phi;
			this->theta = theta;

			return *this;
		}

		Spherical& Spherical::clone(Spherical* spherical)
		{
			spherical->copy(*this);

			return *spherical;
		}

		Spherical& Spherical::copy(const Spherical& other)
		{
			radius = other.radius;
			phi = other.phi;
			theta = other.theta;

			return *this;
		}

		Spherical& Spherical::makeSafe()
		{
			float EPS = 0.000001f;
			
			phi = std::max(EPS, std::min((float)M_PI - EPS, phi));

			return *this;
		}

		Spherical& Spherical::setFromVector3(const Vector3& v)
		{
			return setFromCartesianCoords(v.x, v.y, v.z);
		}

		Spherical& Spherical::setFromCartesianCoords(const float x, const float y, const float z)
		{
			radius = std::sqrt(x * x + y * y + z * z);

			if (radius ==  0) {

				theta = 0;
				phi = 0;

			}
			else {

				theta = std::atan2(x, z);
				phi = std::acos(clamp(y / radius, -1.0f, 1.0f));

			}

			return *this;
		}

	}
}
