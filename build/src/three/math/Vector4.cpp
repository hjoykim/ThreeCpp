#include "Vector4.h"
#include "Matrix4.h"
#include "Matrix3.h"
#include "Quaternion.h"
#include <three/core/BufferAttribute.h>
namespace three {
	namespace math {
		Vector4& three::math::Vector4::set(const float x, const float y, const float z, const float w)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;

			return *this;

		}
		Vector4& Vector4::setScalar(const float scalar)
		{
			x = scalar;
			y = scalar;
			z = scalar;
			w = scalar;

			return *this;
		}
		Vector4& Vector4::setX(const float x)
		{
			this->x = x;

			return *this;
		}
		Vector4& Vector4::setY(const float y)
		{
			this->y = y;

			return *this;
		}
		Vector4& Vector4::setZ(const float z)
		{
			this->z = z;

			return *this;
		}
		Vector4& Vector4::setW(const float w)
		{
			this->w = w;

			return *this;
		}
		Vector4& Vector4::setComponent(const unsigned index, const float value)
		{
			switch (index) {

			case 0: x = value; break;
			case 1: y = value; break;
			case 2: z = value; break;
			case 3: w = value; break;


			}

			return *this;
		}
		float Vector4::getComponent(const unsigned index)
		{
			switch (index) {

			case 0: return x;
			case 1: return y;
			case 2: return z;
			case 3: return w;
			default: return x;
			}
		}
		Vector4& Vector4::clone()
		{
			return *this;
		}
		Vector4& Vector4::copy(const Vector4& v)
		{
			memcpy(elements, v.elements, sizeof(elements));

			return *this;
		}
		Vector4& Vector4::add(const Vector4& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			w += v.w;

			return *this;
		}
		Vector4& Vector4::addScalar(const float scalar)
		{
			x += scalar;
			y += scalar;
			z += scalar;
			w += scalar;

			return *this;
		}
		Vector4& Vector4::addVectors(const Vector4& a, const Vector4& b)
		{
			x = a.x + b.x;
			y = a.y + b.y;
			z = a.z + b.z;
			w = a.w + b.w;

			return *this;
		}
		Vector4& Vector4::addScaledVector(const Vector4& v, const float s)
		{
			x += v.x * s;
			y += v.y * s;
			z += v.z * s;
			w += v.w * s;

			return *this;
		}
		Vector4& Vector4::sub(const Vector4& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			w -= v.w;

			return *this;
		}
		Vector4& Vector4::subScalar(const float s)
		{
			x -= s;
			y -= s;
			z -= s;
			w -= s;

			return *this;
		}
		Vector4& Vector4::subVectors(const Vector4& a, const Vector4& b)
		{
			x = a.x - b.x;
			y = a.y - b.y;
			z = a.z - b.z;
			w = a.w - b.w;

			return *this;
		}
		Vector4& Vector4::multiplyScalar(const float s)
		{
			x *= s;
			y *= s;
			z *= s;
			w *= s;

			return *this;
		}
		Vector4& Vector4::applyMatrix4(const Matrix4& m)
		{
			float x = this->x, y = this->y, z = this->z, w = this->w;
			const float* e = m.elements;

			this->x = e[0] * x + e[4] * y + e[8] * z + e[12] * w;
			this->y = e[1] * x + e[5] * y + e[9] * z + e[13] * w;
			this->z = e[2] * x + e[6] * y + e[10] * z + e[14] * w;
			this->w = e[3] * x + e[7] * y + e[11] * z + e[15] * w;

			return *this;
		}
		Vector4& Vector4::divideScalar(const float s)
		{
			return multiplyScalar(1 / s);
		}
		Vector4& Vector4::setAxisAngleFromQuaternion(const Quaternion& q)
		{
			this->w = 2 * acos(q.getW());

			float s = sqrt(1 - q.getW() * q.getW());

			if (s < 0.0001) {

				x = 1;
				y = 0;
				z = 0;

			}
			else {

				x = q.getX() / s;
				y = q.getY() / s;
				z = q.getZ() / s;

			}

			return *this;
		}
		Vector4& Vector4::setAxisAngleFromRotationMatrix(const Matrix3& m)
		{
			// http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToAngle/index.htm

			// assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)

			float angle, x, y, z,		// variables for result
				epsilon = 0.01f,		// margin to allow for rounding errors
				epsilon2 = 0.1f;		// margin to distinguish between 0 and 180 degrees

			const float* te = m.elements,

				m11 = te[0], m12 = te[4], m13 = te[8],
				m21 = te[1], m22 = te[5], m23 = te[9],
				m31 = te[2], m32 = te[6], m33 = te[10];

			if ((abs(m12 - m21) < epsilon) &&
				(abs(m13 - m31) < epsilon) &&
				(abs(m23 - m32) < epsilon)) {

				// singularity found
				// first check for identity matrix which must have +1 for all terms
				// in leading diagonal and zero in other terms

				if ((abs(m12 + m21) < epsilon2) &&
					(abs(m13 + m31) < epsilon2) &&
					(abs(m23 + m32) < epsilon2) &&
					(abs(m11 + m22 + m33 - 3) < epsilon2)) {

					// this singularity is identity matrix so angle = 0

					set(1, 0, 0, 0);

					return *this; // zero angle, arbitrary axis

				}

				// otherwise this singularity is angle = 180

				angle = PI;

				float xx = (m11 + 1) / 2;
				float yy = (m22 + 1) / 2;
				float zz = (m33 + 1) / 2;
				float xy = (m12 + m21) / 4;
				float xz = (m13 + m31) / 4;
				float yz = (m23 + m32) / 4;

				if ((xx > yy) && (xx > zz)) {

					// m11 is the largest diagonal term

					if (xx < epsilon) {

						x = 0.0f;
						y = 0.707106781f;
						z = 0.707106781f;

					}
					else {

						x = sqrt(xx);
						y = xy / x;
						z = xz / x;

					}

				}
				else if (yy > zz) {

					// m22 is the largest diagonal term

					if (yy < epsilon) {

						x = 0.707106781f;
						y = 0.0f;
						z = 0.707106781f;

					}
					else {

						y = sqrt(yy);
						x = xy / y;
						z = yz / y;

					}

				}
				else {

					// m33 is the largest diagonal term so base result on this

					if (zz < epsilon) {

						x = 0.707106781f;
						y = 0.707106781f;
						z = 0.0f;

					}
					else {

						z = sqrt(zz);
						x = xz / z;
						y = yz / z;

					}

				}

				set(x, y, z, angle);

				return *this; // return 180 deg rotation
			}

			float s = sqrt((m32 - m23) * (m32 - m23) +
			(m13 - m31) * (m13 - m31) +
			(m21 - m12) * (m21 - m12)); // used to normalize

			if (abs(s) < 0.001) s = 1;

			// prevent divide by zero, should not happen if matrix is orthogonal and should be
			// caught by singularity test above, but I've left it in just in case

			x = (m32 - m23) / s;
			y = (m13 - m31) / s;
			z = (m21 - m12) / s;
			w = acos((m11 + m22 + m33 - 1) / 2);

			return *this;
		}
		Vector4& Vector4::min(const Vector4 & v)
		{
			x = std::min(x, v.x);
			y = std::min(y, v.y);
			z = std::min(z, v.z);
			w = std::min(w, v.w);

			return *this;
		}
		Vector4& Vector4::max(const Vector4& v)
		{
			x = std::max(x, v.x);
			y = std::max(y, v.y);
			z = std::max(z, v.z);
			w = std::max(w, v.w);

			return *this;
		}
		Vector4& Vector4::clamp(const Vector4& min, const Vector4& max)
		{
			// assumes min < max, componentwise

			x = std::max(min.x, std::min(max.x, x));
			y = std::max(min.y, std::min(max.y, y));
			z = std::max(min.z, std::min(max.z, z));
			w = std::max(min.w, std::min(max.w, w));

			return *this;
		}
		Vector4& Vector4::clampScalar(const float min, const float max)
		{
			x = std::max(min, std::min(max, x));
			y = std::max(min, std::min(max, y));
			z = std::max(min, std::min(max, z));
			w = std::max(min, std::min(max, w));

			return *this;
		}
		Vector4& Vector4::floor()
		{
			x = std::floor(x);
			y = std::floor(y);
			z = std::floor(z);
			w = std::floor(w);

			return *this;
		}
		Vector4& Vector4::ceil()
		{
			x = std::ceil(x);
			y = std::ceil(y);
			z = std::ceil(z);
			w = std::ceil(w);

			return *this;
		}
		Vector4& Vector4::round()
		{
			x = std::round(x);
			y = std::round(y);
			z = std::round(z);
			w = std::round(w);

			return *this;
		}
		Vector4& Vector4::roundToZero()
		{
			x = (x < 0) ? std::ceil(x) : std::floor(x);
			y = (y < 0) ? std::ceil(y) : std::floor(y);
			z = (z < 0) ? std::ceil(z) : std::floor(z);
			w = (w < 0) ? std::ceil(w) : std::floor(w);

			return *this;
		}
		Vector4& Vector4::negate()
		{
			x = -x;
			y = -y;
			z = -z;
			w = -w;

			return *this;
		}
		float Vector4::dot(const Vector4& v)
		{
			return x * v.x + y * v.y + z * v.z + w * v.w;
		}
		float Vector4::lengthSq() const
		{
			return x * x + y * y + z * z + w * w;
		}
		float Vector4::length() const
		{
			return sqrt(x * x + y * y + z * z + w * w);
		}
		float Vector4::manhattanLength() const
		{
			return abs(x) + abs(y) + abs(z) + abs(w);

		}
		Vector4& Vector4::normalize()
		{
			float len = length();
			return divideScalar(len != 0 ? len : 1);
		}
		Vector4& Vector4::setLength(const float length)
		{
			return normalize().multiplyScalar(length);
		}
		Vector4& Vector4::lerp(const Vector4& v, const float alpha)
		{
			x += (v.x - x) * alpha;
			y += (v.y - y) * alpha;
			z += (v.z - z) * alpha;
			w += (v.w - w) * alpha;

			return *this;
		}
		Vector4& Vector4::lerpVectors(const Vector4& v1, const Vector4& v2, const float alpha)
		{
			return subVectors(v2, v1).multiplyScalar(alpha).add(v1);
		}
		bool Vector4::equals(const Vector4& v)
		{			
			return ((v.x == x) && (v.y == y) && (v.z == z) && (v.w == w));
		}
		Vector4& Vector4::fromArray(const float* array, unsigned arrayLength, unsigned offset)
		{
			x = offset < arrayLength ? array[offset] : std::numeric_limits<float>::quiet_NaN();
			y = offset < arrayLength ? array[offset + 1] : std::numeric_limits<float>::quiet_NaN();
			z = offset < arrayLength ? array[offset + 2] : std::numeric_limits<float>::quiet_NaN();
			w = offset < arrayLength ? array[offset + 3] : std::numeric_limits<float>::quiet_NaN();

			return *this;
		}

		Vector4& Vector4::fromArray(const std::vector<float> array, unsigned offset)
		{
			unsigned arrayLength = array.size();
			x = offset < arrayLength ? array[offset] : std::numeric_limits<float>::quiet_NaN();
			y = offset < arrayLength ? array[offset + 1] : std::numeric_limits<float>::quiet_NaN();
			z = offset < arrayLength ? array[offset + 2] : std::numeric_limits<float>::quiet_NaN();
			w = offset < arrayLength ? array[offset + 3] : std::numeric_limits<float>::quiet_NaN();

			return *this;
		}

		const float* Vector4::toArray(float* array, unsigned offset) const
		{
			float a[4];
			if (array == nullptr) array = a;

			array[offset] = x;
			array[offset + 1] = y;
			array[offset + 2] = z;
			array[offset + 3] = w;

			return array;
		}
		template<typename T>
		Vector4& Vector4::fromBufferAttribute(three::BufferAttribute<T>& attribute, unsigned index, unsigned offset)
		{
			x = attribute.getX(index);
			y = attribute.getY(index);
			z = attribute.getZ(index);
			w = attribute.getW(index);

			return *this;
		}
	}	
}



