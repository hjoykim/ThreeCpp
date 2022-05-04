#ifndef SPHERICAL_THREE_H
#define SPHERICAL_THREE_H
#
namespace three {
	namespace math {
		class Vector3;

		class Spherical {
		public:
			float radius;
			float phi;
			float theta;

			Spherical() : radius(1.0), phi(0), theta(0) {}

			Spherical(const float radius, const float phi, const float theta) : radius(radius),phi(phi),theta(theta){}

			virtual ~Spherical() = default;

			Spherical& set(const float radius, const float phi, const float theta);

			Spherical& clone(Spherical* spherical);

			Spherical& copy(const Spherical& other);

			Spherical& makeSafe();

			Spherical& setFromVector3(const Vector3& v);

			Spherical& setFromCartesianCoords(const float x, const float y, const float z);

		};
	}
}
#endif
