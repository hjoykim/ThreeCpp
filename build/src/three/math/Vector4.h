#ifndef THREE_VECTOR4_H
#define THREE_VECTOR4_H
#include <three/utils/osdecl.h>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <three/math/MathUtils.h>
#include <sstream>
namespace three {
	template<typename T>
	class BufferAttribute;

	namespace math {
		class Matrix3;
		class Matrix4;
		class Quaternion;		

		class Vector4 {
			
		public:
			union {
				struct {
					float x, y, z, w;
				};
				float elements[4];
			};
			Vector4() : x(0), y(0), z(0), w(1) {}

			Vector4(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {}

			Vector4(const Vector4& vector)
			{
				memcpy(elements, vector.elements, sizeof(elements));
			}	
			/**
				 * Sets value of this vector.
				 */
			Vector4& set(const float x, const float y, const float z, const float w);

			/**
			 * Sets all values of this vector.
			 */
			Vector4& setScalar(const float scalar);

			/**
			 * Sets X component of this vector.
			 */
			Vector4& setX(const float x);

			/**
			 * Sets Y component of this vector.
			 */
			Vector4& setY(const float y);

			/**
			 * Sets Z component of this vector.
			 */
			Vector4& setZ(const float z);

			/**
			 * Sets w component of this vector.
			 */
			Vector4& setW(const float w);

			Vector4& setComponent(const unsigned index, const float value);

			float getComponent(const unsigned index);

			/**
			 * Clones this vector.
			 */
			Vector4& clone();

			/**
			 * Copies value of v to this vector.
			 */
			Vector4& copy(const Vector4& v);

			/**
			 * Adds v to this vector.
			 */
			Vector4& add(const Vector4& v);

			Vector4& addScalar(const float scalar);

			/**
			 * Sets this vector to a + b.
			 */
			Vector4& addVectors(const Vector4& a, const Vector4& b);

			Vector4& addScaledVector(const Vector4& v, const float s);
			/**
			 * Subtracts v from this vector.
			 */
			Vector4& sub(const Vector4& v);

			Vector4& subScalar(const float s);

			/**
			 * Sets this vector to a - b.
			 */
			Vector4& subVectors(const Vector4& a, const Vector4& b);

			/**
			 * Multiplies this vector by scalar s.
			 */
			Vector4& multiplyScalar(const float s);

			Vector4& applyMatrix4(const Matrix4& m);

			/**
			 * Divides this vector by scalar s.
			 * Set vector to ( 0, 0, 0 ) if s == 0.
			 */
			Vector4& divideScalar(const float s);

			/**
			 * http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToAngle/index.htm
			 * @param q is assumed to be normalized
			 */
			Vector4& setAxisAngleFromQuaternion(const Quaternion& q);

			/**
			 * http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToAngle/index.htm
			 * @param m assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)
			 */
			Vector4& setAxisAngleFromRotationMatrix(const Matrix3& m);

			Vector4& min(const Vector4& v);
			Vector4& max(const Vector4& v);
			Vector4& clamp(const Vector4& min, const Vector4& max);
			Vector4& clampScalar(const float min, const float max);
			Vector4& floor();
			Vector4& ceil();
			Vector4& round();
			Vector4& roundToZero();

			/**
			 * Inverts this vector.
			 */
			Vector4& negate();

			/**
			 * Computes dot product of this vector and v.
			 */
			float dot(const Vector4& v);

			/**
			 * Computes squared length of this vector.
			 */
			float lengthSq() const;

			/**
			 * Computes length of this vector.
			 */
			float length() const ;

			/**
			 * Computes the Manhattan length of this vector.
			 *
			 * @return {number}
			 *
			 * @see {@link http://en.wikipedia.org/wiki/Taxicab_geometry|Wikipedia: Taxicab Geometry}
			 */
			float manhattanLength() const;

			/**
			 * Normalizes this vector.
			 */
			Vector4& normalize();
			/**
			 * Normalizes this vector and multiplies it by l.
			 */
			Vector4& setLength(const float length);

			/**
			 * Linearly interpolate between this vector and v with alpha factor.
			 */
			Vector4& lerp(const Vector4& v, const float alpha);

			Vector4& lerpVectors(const Vector4& v1, const Vector4& v2, const float alpha);

			/**
			 * Checks for strict equality of this vector and v.
			 */
			bool equals(const Vector4& v);

			/**
			 * Sets this vector's x, y, z and w value from the provided array.
			 * @param array the source array.
			 * @param offset (optional) offset into the array. Default is 0.
			 */
			Vector4& fromArray(const float* array, unsigned arrayLength, unsigned offset=0);
			
			Vector4& fromArray(const std::vector<float> array, unsigned offset=0);

			/**
			 * Returns an array [x, y, z, w], or copies x, y, z and w into the provided array.
			 * @param array (optional) array to store the vector to. If this is not provided, a new array will be created.
			 * @param offset (optional) optional offset into the array.
			 * @return The created or provided array.
			 */
			const float* toArray(float * array, unsigned offset=0) const;
						
			std::string to_string() {
				std::stringstream stream;
				stream << "x=" << std::to_string(x) << ",y=" << std::to_string(y) << ",z=" << std::to_string(z)<<",w="<<std::to_string(w);
				return stream.str();
			}

			template<typename T>
			Vector4& fromBufferAttribute(three::BufferAttribute<T>& attribute, unsigned index, unsigned offset=0);

			bool operator !=(const Vector4& v) {
				return !equals(v);
			}

			bool operator == (const Vector4& v) {
				return equals(v);
			}

		};
		inline Vector4 operator *(const Vector4& vector, float scalar)
		{
			Vector4 value(vector);
			value.multiplyScalar(scalar);
			return value;
		}

		inline Vector4 operator - (const Vector4& a, const Vector4& b)
		{
			float x = a.x - b.x;
			float y = a.y - b.y;
			float z = a.z - b.z;
			float w = a.w - b.w;

			return Vector4(x, y, z, w);
		}
	}
}
#endif