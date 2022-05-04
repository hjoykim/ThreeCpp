#ifndef CYLINDRICAL_THREE_H
#define CYLINDRICAL_THREE_H
namespace three {
	namespace math {
		
		class Vector3;

		class Cylindrical {
		public:

			float radius;

			float theta;

			float y;

			Cylindrical() : radius(1.0f), theta(0), y(0) {};

			Cylindrical(const float radius, const float theta, const float y) : radius(radius), theta(theta), y(y) {}

			Cylindrical& clone(Cylindrical* c);

			Cylindrical& copy(const Cylindrical& other);

			Cylindrical& set(const float radius, const float theta, const float y);

			Cylindrical& setFromVector3(const Vector3& vec3);

			Cylindrical& setFromCartesianCoords(const float x, const float y, const float z);
		};
	}
}
#endif
