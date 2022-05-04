/**
	ported by h.joy from three.js
*/
#ifndef THREE_EULER_H
#define THREE_EULER_H
#include <memory>
#include <three/utils/osdecl.h>
#include <three/utils/simplesignal.h>
#include <three/math/MathUtils.h>

namespace three {
	namespace math {

		class Matrix4;

		class Quaternion;

		class Vector3;

		class Matrix3;


		class Euler {
		public:
			enum class RotationOrder { XYZ, YZX, ZXY, XZY, YXZ, ZYX,Default };
			float _x, _y, _z;
			static const RotationOrder defaultOrder = RotationOrder::XYZ;
		private:
			

			RotationOrder _order;

		public:

			

			Signal<void(const Euler&)> onChange;

			Euler() :_x(0), _y(0), _z(0), _order(RotationOrder::XYZ) {}

			Euler(float x, float y, float z, RotationOrder order) : _x(x), _y(y), _z(z), _order(order) {}

			Euler(const Euler& e) : _x(e._x), _y(e._y), _z(e._z), _order(e._order) {}

			Euler(float x, float y, float z);

			Euler& set(float x, float y, float z, RotationOrder order = RotationOrder::XYZ);

			Euler& clone();

			Euler& copy(const Euler& euler);

			Euler& setFromRotationMatrix(const Matrix4& m, RotationOrder order = RotationOrder::XYZ, bool update = false);

			Euler& setFromQuaternion(const Quaternion& q, RotationOrder order = RotationOrder::XYZ, bool update = false);

			Euler& setFromVector3(const Vector3& v, RotationOrder order = RotationOrder::XYZ);

			Euler& reorder(RotationOrder order);

			bool equals(const Euler& euler);

			Euler& fromArray(const float* array);

			float* toArray(float* array, unsigned offset = 0);

			Vector3& toVector3(Vector3* optionalResult = NULL);

			float getX() const;

			float getY() const;

			float getZ() const;

			RotationOrder getOrder() const;

			void setX(float x);

			void setY(float y);

			void setZ(float z);

			void setOrder(RotationOrder order);

			
			bool operator == (const Euler& euler) const;

			bool operator != (const Euler& euler) const { return !(*this == euler); }

			Euler& operator = (const Euler& euler);
		};
	}
}
#endif
