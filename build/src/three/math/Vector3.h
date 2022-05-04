#ifndef THREE_VECTOR3_H
#define THREE_VECTOR3_H
#include <cassert>
#include <algorithm>
#ifdef _MSC_VER
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define _USE_MATH_DEFINES
#include <math.h>
#else
#include <cmath>
#endif
#include <cmath>
#include <vector>
#include <three/utils/osdecl.h>
#include "MathUtils.h"
#include <vector>
#include <limits>
#include <sstream>
//#include <three/cameras/Camera.h>

namespace three {

	class Camera;
	template<typename T>
	class BufferAttribute;

	namespace math {
		/*
		Vector3 operator + (const Vector3& left, const Vector3& right);

		Vector3 operator -(const Vector3& left, const Vector3& right);

		Vector3 operator *(const Vector3& left, const Vector3& right);

		Vector3 operator *(const Vector3& vector, float scalar);

		Vector3 operator *(const Vector3& vector, const Matrix4& matrix);

		Vector3 operator / (const Vector3& vector, float scalar);
		*/

		class Matrix3;

		class Matrix4;

		class Quaternion;

		class Spherical;
		
		class Cylindrical;



		class Euler;

		class Vector3
		{

		public:
			union {
				struct {					
					float x, y, z;
				};
				float elements[3];
			};

			Vector3();

			explicit Vector3(float s);

			Vector3(float _x, float _y, float _z);

			Vector3(const Vector3& v);

			Vector3& set(float _x, float _y, float _z) {
				x = _x, y = _y, z = _z;
				return *this;
			}
			/**
			 * Sets all values of this vector.
			 */
			Vector3& setScalar(float number)
			{
				x = number;
				y = number;
				z = number;

				return *this;
			}

			Vector3& setX(float x)
			{
				this->x = x;

				return *this;
			}

			Vector3& setY(float y)
			{
				this->y = y;

				return *this;
			}

			Vector3& setZ(float z)
			{
				this->z = z;

				return *this;
			}

			Vector3& setComponent(unsigned index, float value)
			{
				assert(index < 3);

				switch (index) {
				case 0: x = value; break;
				case 1: y = value; break;
				case 2: z = value; break;
				}

				return *this;
			}

			float getComponent(unsigned index)
			{
				assert(index < 3);

				switch (index) {
				case 0: return x;
				case 1: return y;
				case 2: return z;
				}
			}

			Vector3& clone(Vector3* v) {
				
				v->copy(*this);

				return *v;
			}
			/**
			 * Clones this vector.
			 */
			Vector3& clone() 
			{
				return *this;
			}

			/**
			 * Copies value of v to this vector.
			 */
			Vector3& copy(const Vector3& v) 
			{
				x = v.x;
				y = v.y;
				z = v.z;

				return *this;
			}

			/**
			 * Adds v to this vector.
			 */
			Vector3& add(const Vector3& v)
			{
				x += v.x;
				y += v.y;
				z += v.z;

				return *this;
			}

			Vector3& addScalar(float s)
			{
				x += s;
				y += s;
				z += s;

				return *this;
			}

			Vector3& addScaledVector(const Vector3& v, float s)
			{
				x += v.x * s;
				y += v.y * s;
				z += v.z * s;

				return *this;
			}

			/**
			 * Sets this vector to a + b.
			 */
			Vector3& addVectors(const Vector3& a, const Vector3& b)
			{
				x = a.x + b.x;
				y = a.y + b.y;
				z = a.z + b.z;

				return *this;
			}

			/**
			 * Subtracts v from this vector.
			 */
			Vector3& sub(const Vector3& a)
			{
				x -= a.x;
				y -= a.y;
				z -= a.z;

				return *this;
			}

			/**
			 * Sets this vector to a - b.
			 */

			Vector3& subVectors(const Vector3& a, const Vector3& b)
			{
				x = a.x - b.x;
				y = a.y - b.y;
				z = a.z - b.z;

				return *this;
			}

			Vector3& subScalar(float s)
			{
				x -= s;
				y -= s;
				z -= s;

				return *this;
			}

			Vector3& multiply(const Vector3& v)
			{
				x *= v.x;
				y *= v.y;
				z *= v.z;

				return *this;
			}

			/**
			 * Multiplies this vector by scalar s.
			 */
			Vector3& multiplyScalar(float s)
			{
				x *= s;
				y *= s;
				z *= s;

				return *this;
			}

			Vector3& multiplyVectors(const Vector3& a, const Vector3& b)
			{
				x = a.x * b.x;
				y = a.y * b.y;
				z = a.z * b.z;

				return *this;
			}

			Vector3& applyEuler(const Euler& euler);


			Vector3& applyAxisAngle(const Vector3& axis, float angle);

			Vector3& applyMatrix3(const Matrix3& m);


			Vector3& applyNormalMatrix(const Matrix3& m)
			{
				return applyMatrix3(m).normalize();
			}

			Vector3& applyMatrix4(const Matrix4& m);

			Vector3& applyQuaternion(const Quaternion& q);

			Vector3& project(const Camera& camera);

			Vector3& unproject(const Camera& camera);

			Vector3& transformDirection(const Matrix4& m);


			Vector3& divide(const Vector3& v)
			{
				x /= v.x;
				y /= v.y;
				z /= v.z;

				return *this;
			}

			/**
			 * Divides this vector by scalar s.
			 * Set vector to ( 0, 0, 0 ) if s == 0.
			 */
			Vector3& divideScalar(float s)
			{
				return multiplyScalar(1 / s);
			}


			Vector3& min(const Vector3& v);			

			Vector3& max(const Vector3& v);			

			Vector3& clamp(const Vector3& min, const Vector3& max);			

			Vector3& clampScalar(float min, float max);			

			Vector3& clampLength(float min, float max);			

			Vector3& floor() {
				x = std::floor(x);
				y = std::floor(y);
				z = std::floor(z);

				return *this;
			}

			Vector3& ceil() {
				x = std::ceil(x);
				y = std::ceil(y);
				z = std::ceil(z);

				return *this;
			}

			Vector3& round() {
				x = std::round(x);
				y = std::round(y);
				z = std::round(z);

				return *this;
			}

			Vector3& roundToZero() {
				x = (x < 0) ? std::ceil(x) : std::floor(x);
				y = (y < 0) ? std::ceil(y) : std::floor(y);
				z = (z < 0) ? std::ceil(z) : std::floor(z);

				return *this;
			}

			/**
			 * Inverts this vector.
			 */
			Vector3& negate() {
				x = -x;
				y = -y;
				z = -z;

				return *this;
			}

			/**
			 * Computes dot product of this vector and v.
			 */
			float dot(const Vector3& v) const {
				return x * v.x + y * v.y + z * v.z;
			}

			/**
			 * Computes squared length of this vector.
			 */
			float lengthSq() const {
				return x * x + y * y + z * z;
			}

			/**
			 * Computes length of this vector.
			 */
			float length() const {
				return std::sqrt(x * x + y * y + z * z);
			}


			/**
			 * Computes the Manhattan length of this vector.
			 *
			 * @return {number}
			 *
			 * @see {@link http://en.wikipedia.org/wiki/Taxicab_geometry|Wikipedia: Taxicab Geometry}
			 */
			float manhattanLength() {
				return std::abs(x) + std::abs(y) + std::abs(z);
			}

			/**
			 * Computes the Manhattan length (distance) from this vector to the given vector v
			 *
			 * @param {Vector3} v
			 *
			 * @return {number}
			 *
			 * @see {@link http://en.wikipedia.org/wiki/Taxicab_geometry|Wikipedia: Taxicab Geometry}
			 */
			float manhattanDistanceTo(const Vector3& v) {
				return std::abs(x - v.x) + std::abs(y - v.y) + std::abs(z - v.z);
			}

			/**
			 * Normalizes this vector.
			 */
			Vector3& normalize() {
				return divideScalar(length() != 0 ? length() : 1);
			}

			/**
			 * Normalizes this vector and multiplies it by l.
			 */
			Vector3& setLength(float length) {
				return normalize().multiplyScalar(length);
			}

			Vector3& lerp(const Vector3& v, float alpha) {
				x += (v.x - x) * alpha;
				y += (v.y - y) * alpha;
				z += (v.z - z) * alpha;

				return *this;
			}

			Vector3& lerpVectors(const Vector3& v1, const Vector3& v2, float alpha) {
				return subVectors(v2, v1).multiplyScalar(alpha).add(v1);
			}

			/**
			 * Sets this vector to cross product of itself and v.
			 */
			Vector3& cross(const Vector3& a) {
				return crossVectors(*this, a);
			}

			/**
			 * Sets this vector to cross product of a and b.
			 */
			Vector3& crossVectors(const Vector3& a, const Vector3& b) {
				float ax = a.x, ay = a.y, az = a.z;
				float bx = b.x, by = b.y, bz = b.z;

				x = ay * bz - az * by;
				y = az * bx - ax * bz;
				z = ax * by - ay * bx;

				return *this;
			}

			Vector3& projectOnVector(const Vector3& v) {
				float denominator = v.lengthSq();

				if (denominator == 0) return set(0, 0, 0);

				float scalar = v.dot(*this) / denominator;

				return copy(v).multiplyScalar(scalar);
			}

			Vector3& projectOnPlane(const Vector3& planeNormal) {
				Vector3 _vector;

				_vector.copy(*this).projectOnVector(planeNormal);

				return sub(_vector);
			}

			Vector3& reflect(const Vector3& vector) {
				Vector3 _vector;
				return sub(_vector.copy(vector).multiplyScalar(2 * dot(vector)));
			}

			float angleTo(const Vector3& v) {
				float denominator = std::sqrt(lengthSq() * v.lengthSq());

				if (denominator == 0) return PI / 2;

				float theta = dot(v) / denominator;

				// clamp, to handle numerical problems

				return three::math::acos(three::math::clamp(theta, -1.0f, 1.0f));
			}

			/**
			 * Computes distance of this vector to v.
			 */
			float distanceTo(const Vector3& v) const {
				return sqrt(distanceToSquared(v));
			}

			/**
			 * Computes squared distance of this vector to v.
			 */
			float distanceToSquared(const Vector3& v) const {
				float dx = x - v.x, dy = y - v.y, dz = z - v.z;				
				return (float)(dx * dx) + (float)(dy * dy) + (float)(dz * dz);
			}

			Vector3& setFromSpherical(const Spherical& s);

			Vector3& setFromSphericalCoords(float r, float phi, float theta);

			Vector3& setFromCylindrical(const Cylindrical& c);

			Vector3& setFromCylindricalCoords(float radius, float theta, float y);

			Vector3& setFromMatrixPosition(const Matrix4& m);

			Vector3& setFromMatrixScale(const Matrix4& m);

			Vector3& setFromMatrixColumn(const Matrix4& matrix, unsigned index);

			Vector3& setFromMatrix3Column(const Matrix3& matrix, unsigned index);

			/**
			 * Checks for strict equality of this vector and v.
			 */
			bool equals(const Vector3& v) const {
				return ((v.x == x) && (v.y == y) && (v.z == z));
			}

			/**
			 * Sets this vector's x, y and z value from the provided array.
			 * @param array the source array.
			 * @param offset (optional) offset into the array. Default is 0.
			 */
			Vector3& fromArray(const float* array, unsigned arrayLength,unsigned offset = 0) {			
				x = offset <arrayLength ? array[offset] : std::numeric_limits<float>::quiet_NaN();
				y = offset < arrayLength ? array[offset + 1] : std::numeric_limits<float>::quiet_NaN();
				z = offset < arrayLength ? array[offset + 2] : std::numeric_limits<float>::quiet_NaN();

				return *this;
			}

			Vector3& fromArray(const std::vector<float> array, unsigned offset = 0) {
				unsigned arrayLength = array.size();
				x = offset < arrayLength ? array[offset] : std::numeric_limits<float>::quiet_NaN();
				y = offset < arrayLength ? array[offset + 1] : std::numeric_limits<float>::quiet_NaN();
				z = offset < arrayLength ? array[offset + 2] : std::numeric_limits<float>::quiet_NaN();

				return *this;
			}


			/**
			 * Returns an array [x, y, z], or copies x, y and z into the provided array.
			 * @param array (optional) array to store the vector to. If this is not provided, a new array will be created.
			 * @param offset (optional) optional offset into the array.
			 * @return The created or provided array.
			 */
			const float* toArray(float* array, unsigned offset = 0) const {
				float a[3];
				if (array == nullptr) array = a;

				array[offset] = x;
				array[offset + 1] = y;
				array[offset + 2] = z;

				return array;
			}


			template<typename T>
			Vector3& fromBufferAttribute(three::BufferAttribute<T>& attribute, unsigned index, unsigned offset = 0) {
				{
					x = attribute.getX(index);
					y = attribute.getY(index);
					z = attribute.getZ(index);
					return *this;
				}
			}

			void writeTo(float* target, unsigned offset) {
				target[offset] = x;
				target[offset + 1] = y;
				target[offset + 2] = z;
			}

			Vector3& operator /= (const Vector3& v)
			{
				x /= v.x;
				y /= v.y;
				z /= v.z;

				return *this;
			}

			Vector3& operator /=(float scalar)
			{
				return *this *= (1.0f / scalar);
			}

			Vector3& operator =(float scalar)
			{
				x = y = z = scalar;
				return *this;
			}

			Vector3& operator +=(const Vector3& vector)
			{
				x += vector.x;
				y += vector.y;
				z += vector.z;

				return *this;
			}

			Vector3& operator +=(float scalar)
			{
				x += scalar;
				y += scalar;
				z += scalar;

				return *this;
			}

			Vector3& operator -=(const Vector3& v)
			{
				x -= v.x;
				y -= v.y;
				z -= v.z;

				return *this;
			}

			Vector3& operator -=(float scalar)
			{
				x -= scalar;
				y -= scalar;
				z -= scalar;

				return *this;
			}

			Vector3& operator *=(const Vector3& v)
			{
				x *= v.x;
				y *= v.y;
				z *= v.z;

				return *this;
			}

			Vector3& operator *=(float scalar)
			{
				x *= scalar;
				y *= scalar;
				z *= scalar;

				return *this;
			}

			std::string to_string() {
				std::stringstream stream;
				stream << "x=" << std::to_string(x) << ",y=" << std::to_string(y) << ",z=" << std::to_string(z);
				return stream.str();
			}

			bool operator !=(const Vector3& v) {
				return !equals(v);
			}

			bool operator == (const Vector3& v) {
				return equals(v);
			}
			float& operator [] (unsigned char ch) {
				switch (ch) {
				case 'x':
					return x;
				case 'y':
					return y;
				case 'z':
					return z;
				}
			}

		};
		inline Vector3 operator + (const Vector3& left, const Vector3& right) {
			Vector3 result(left);
			result += right;
			return result;
		}
		inline Vector3 operator -(const Vector3& left, const Vector3& right)
		{
			Vector3 result(left);
			result -= right;
			return result;
		}

		inline Vector3 operator *(const Vector3& left, const Vector3& right)
		{
			Vector3 result(left);
			result *= right;
			return result;
		}

		inline Vector3 operator *(const Vector3& vector, float scalar)
		{
			Vector3 result(vector);
			result *= scalar;
			return result;
		}

		inline Vector3 operator *(const Vector3& vector, Matrix4& matrix)
		{
			return Vector3(vector).applyMatrix4(matrix);
		}

		inline Vector3 operator / (const Vector3& v1, const Vector3& v2)
		{
			Vector3 vector(v1);
			vector /= v2;

			return vector;
		}

		inline Vector3 operator / (const Vector3& vector, float scalar)
		{
			return vector * (1.0f / scalar);
		}

		inline Vector3 cross(const Vector3& a, const Vector3& b)
		{
			float ax = a.x, ay = a.y, az = a.z;
			float bx = b.x, by = b.y, bz = b.z;

			return Vector3(ay * bz - az * by,
				az * bx - ax * bz,
				ax * by - ay * bx);
		}

		inline float dot(const Vector3& a, const Vector3& b)
		{
			return a.x * b.x + a.y * b.y + a.z * b.z;
		}

		inline Vector3 operator +(const Vector3& vector, float scalar)
		{
			Vector3 v(vector);
			return v += scalar;
		}

		inline Vector3 operator *(float scalar, const Vector3& vector)
		{
			Vector3 v(vector);
			return v *= scalar;
		}
		
	}
}
#endif
