/**
	ported by h.joy from three.js
*/
#ifndef THREE_QUATERNION_H
#define THREE_QUATERNION_H
#include <cassert>
#include <three/utils/osdecl.h>
#include <three/utils/simplesignal.h>
#include <three/math/MathUtils.h>
namespace three {
	namespace math {
		class Euler;

		class Vector3;

		class Matrix4;

		class Quaternion
		{
			union {
				struct {
					float _x, _y, _z, _w;
				};
				float elements[4];
			};
			
		public:

			Signal<void(const Quaternion&)> onChange;

			Quaternion();

			Quaternion(float x, float y, float z) :_x(x), _y(y), _z(z), _w(1) {}

			Quaternion(float x, float y, float z, float w) :_x(x), _y(y), _z(z), _w(w) {}

			Quaternion(float scalar) :_x(scalar), _y(scalar), _z(scalar), _w(scalar) {}

			Quaternion(const Quaternion& q) : _x(q._x), _y(q._y), _z(q._z), _w(q._w) {}

			Quaternion& operator = (const Quaternion& q) {
				return set(q, true);
			}

			Quaternion& set(const Quaternion& q, bool emitSignal) {
				_x = q._x;
				_y = q._y;
				_z = q._z;
				_w = q._w;

				if (emitSignal) onChange.emitSignal(*this);

				return *this;
			}
			Quaternion& clone() {
				return *this;
			}
			Quaternion& set(float x, float y, float z) {
				_x = x;
				_y = y;
				_z = z;
				_w = 1;

				return *this;
			}

			Quaternion& set(float x, float y, float z,float w) {
				_x = x;
				_y = y;
				_z = z;
				_w = w;

				return *this;
			}
			Quaternion& copy(const Quaternion& q) {
				_x = q._x;
				_y = q._y;
				_z = q._z;
				_w = q._w;

				return *this;
			}
			float getX() const { return _x; }
			float getY() const { return _y; }
			float getZ() const { return _z; }
			float getW() const { return _w; }

			void setX(float value) {
				_x = value;
				onChange.emitSignal(*this);
			}
			void setY(float value) {
				_y = value;
				onChange.emitSignal(*this);
			}
			void setZ(float value) {
				_z = value;
				onChange.emitSignal(*this);
			}
			void setW(float value) {
				_w = value;
				onChange.emitSignal(*this);
			}

			Quaternion& setFromEuler(const Euler& euler, bool update = false);

			Quaternion& setFromAxisAngle(const Vector3& axis, float angle);

			Quaternion& setFromRotationMatrix(const Matrix4& m);

			Quaternion& setFromUnitVectors(const Vector3& vFrom, const Vector3& vTo);

			float angleTo(const Quaternion& q);

			Quaternion& rotateTowards(const Quaternion& q, float step);

			Quaternion& inverse() {
				return conjugate();
			}

			Quaternion& conjugate() {
				_x *= -1;
				_y *= -1;
				_z *= -1;

				onChange.emitSignal(*this);

				return *this;
			}

			float dot(const Quaternion& v) const {
				return _x * v._x + _y * v._y + _z * v._z + _w * v._w;
			}

			float lengthSq() const {
				return _x * _x + _y * _y + _z * _z + _w * _w;
			}

			float length() const {
				return sqrt(_x * _x + _y * _y + _z * _z + _w * _w);
			}

			Quaternion& normalize() {
				float l = length();
				if (l == 0) {
					_x = 0;
					_y = 0;
					_z = 0;
					_w = 1;
				}
				else {
					l = 1 / l;
					_x = _x * l;
					_y = _y * l;
					_z = _z * l;
					_w = _w * l;

				}
				return *this;
			}

			Quaternion& multiply(const Quaternion& q) {
				return multiplyQuaternions(*this, q);
			}

			Quaternion& premultiply(const Quaternion& q) {
				return multiplyQuaternions(q, *this);
			}
			Quaternion& multiplyQuaternions(const Quaternion& a, const Quaternion& b) {
				float qax = a._x, qay = a._y, qaz = a._z, qaw = a._w,
					qbx = b._x, qby = b._y, qbz = b._z, qbw = b._w;

				_x = qax * qbw + qaw * qbx + qay * qbz - qaz * qby;
				_y = qay * qbw + qaw * qby + qaz * qbx - qax * qbz;
				_z = qaz * qbw + qaw * qbz + qax * qby - qay * qbx;
				_w = qaw * qbw - qax * qbx - qay * qby - qaz * qbz;

				onChange.emitSignal(*this);

				return *this;
			}
			bool equals(const Quaternion& quaternion);

			Quaternion& fromArray(const float* array, unsigned offset = 0) {
				_x = array[offset];
				_y = array[offset + 1];
				_z = array[offset + 2];
				_w = array[offset + 3];

				return *this;
			}

			float* toArray(float* array, unsigned offset = 0) {
				float a[4];
				if (array == NULL) array = a;

				array[offset] = _x;
				array[offset + 1] = _y;
				array[offset + 2] = _z;
				array[offset + 3] = _w;

				return array;
			}

			Quaternion& slerp(const Quaternion& qb, float t) {
				if (t == 0) return *this;
				if (t == 1) return copy(qb);

				float x = _x, y = _y, z = _z, w = _w;

				// http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/

				float cosHalfTheta = w * qb._w + x * qb._x + y * qb._y + z * qb._z;

				if (cosHalfTheta < 0) {

					_w = -qb._w;
					_x = -qb._x;
					_y = -qb._y;
					_z = -qb._z;

					cosHalfTheta = -cosHalfTheta;

				}
				else {

					copy(qb);

				}

				if (cosHalfTheta >= 1.0f) {

					_w = w;
					_x = x;
					_y = y;
					_z = z;

					return *this;

				}

				float sqrSinHalfTheta = 1.0f - cosHalfTheta * cosHalfTheta;

				if (sqrSinHalfTheta <= std::numeric_limits<float>::epsilon()) {

					float s = 1 - t;
					_w = s * w + t * _w;
					_x = s * x + t * _x;
					_y = s * y + t * _y;
					_z = s * z + t * _z;

					normalize();
					onChange.emitSignal(*this);

					return *this;

				}

				float sinHalfTheta = sqrt(sqrSinHalfTheta);
				float halfTheta = atan2(sinHalfTheta, cosHalfTheta);
				float ratioA = sin((1 - t) * halfTheta) / sinHalfTheta,
					ratioB = sin(t * halfTheta) / sinHalfTheta;

				_w = (w * ratioA + _w * ratioB);
				_x = (x * ratioA + _x * ratioB);
				_y = (y * ratioA + _y * ratioB);
				_z = (z * ratioA + _z * ratioB);

				onChange.emitSignal(*this);

				return *this;
			}

			Quaternion& slerpFlat(float* dst, unsigned dstOffset, const float* src0, unsigned srcOffset0, const float* src1, unsigned srcOffset1, float t) {

				float x0 = src0[srcOffset0 + 0], y0 = src0[srcOffset0 + 1], z0 = src0[srcOffset0 + 2], w0 = src0[srcOffset0 + 3],
					x1 = src1[srcOffset1 + 0], y1 = src1[srcOffset1 + 1], z1 = src1[srcOffset1 + 2], w1 = src1[srcOffset1 + 3];

				if (w0 != w1 || x0 != x1 || y0 != y1 || z0 != z1) {

					float s = 1 - t,
						_cos = x0 * x1 + y0 * y1 + z0 * z1 + w0 * w1,
						sqrSin = 1 - _cos * _cos;

					int dir = (_cos >= 0 ? 1 : -1);


					// Skip the Slerp for tiny steps to avoid numeric problems:
					if (sqrSin > std::numeric_limits<float>::epsilon()) {

						float _sin = sqrt(sqrSin),
							len = atan2(_sin, _cos * dir);

						s = sin(s * len) / _sin;
						t = sin(t * len) / _sin;

					}

					float tDir = t * dir;

					x0 = x0 * s + x1 * tDir;
					y0 = y0 * s + y1 * tDir;
					z0 = z0 * s + z1 * tDir;
					w0 = w0 * s + w1 * tDir;

					// Normalize in case we just did a lerp:
					if (s == 1 - t) {

						float f = 1 / sqrt(x0 * x0 + y0 * y0 + z0 * z0 + w0 * w0);

						x0 *= f;
						y0 *= f;
						z0 *= f;
						w0 *= f;

					}

				}

				dst[dstOffset] = x0;
				dst[dstOffset + 1] = y0;
				dst[dstOffset + 2] = z0;
				dst[dstOffset + 3] = w0;

				return *this;
			}

			Quaternion& operator *=(const Quaternion& q) {
				return multiplyQuaternions(*this, q);
			}
		};

		inline Quaternion operator *(const Quaternion& q1, const Quaternion& q2) {
			Quaternion result(q1);

			result *= q2;

			return result;
		}
	}
}
#endif 

