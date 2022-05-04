#ifndef THREE_VECTOR2_H
#define THREE_VECTOR2_H
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
#include <three/utils/osdecl.h>
#include <three/math/MathUtils.h>
#include <vector>
#include <sstream>
namespace three {
	namespace math {

		class Matrix3;

		class Vector2
		{
		public:
			union {
				struct {
					float x, y;
				};
				float elements[2];
			};

			Vector2() : x(0), y(0) {}

			Vector2(float _x, float _y) : x(_x), y(_y) {}

			explicit Vector2(float value) : x(value), y(value) {}

			explicit Vector2(float* values) :x(values[0]), y(values[1]) {}

			const float width() const { return x; }

			const float height() const { return y; }


			Vector2& set(float _x, float _y)
			{

				x = _x;
				y = _y;

				return *this;
			}

			Vector2& setScalar(float scalar)
			{
				x = scalar;
				y = scalar;

				return *this;
			}

			Vector2& operator = (float scalar) {
				return setScalar(scalar);
			}

			Vector2& setX(float _x)
			{
				x = _x;

				return *this;
			}

			Vector2& setY(float _y)
			{
				y = _y;

				return *this;
			}

			Vector2& setComponent(int index, float value)
			{
				assert(index < 2);

				switch (index)
				{
				case 0: x = value; break;
				case 1: y = value; break;
				}

				return *this;
			}

			float getComponent(int index)
			{
				assert(index < 2);

				switch (index)
				{
				case 0: return x;
				case 1: return y;
				default:return x;
				}
			}

			Vector2& clone(Vector2* v)
			{
				v->x = x;
				v->y = y;

				return *v;
			}
			Vector2& clone()
			{
				return *this;
			}

			Vector2& copy(const Vector2& v)
			{
				x = v.x;
				y = v.y;

				return *this;
			}

			Vector2& add(const Vector2& v)
			{
				x += v.x;
				y += v.y;

				return *this;
			}

			Vector2& operator+=(const Vector2 a) {
				return add(a);
			}

			Vector2& addScalar(const float s)
			{
				x += s;
				y += s;

				return *this;
			}

			Vector2& operator +=(float scalar) {
				return addScalar(scalar);
			}

			Vector2& addVectors(const Vector2& a, const Vector2& b)
			{
				x = a.x + b.x;
				y = a.y + b.y;

				return *this;
			}

			Vector2& addScaledVector(const Vector2& v, float s)
			{
				x += v.x * s;
				y += v.y * s;

				return *this;
			}

			Vector2& sub(const Vector2& v)
			{
				x -= v.x;
				y -= v.y;

				return *this;
			}

			Vector2& operator -=(const Vector2 v) {
				return sub(v);
			}

			Vector2& subScalar(const float s)
			{
				x -= s;
				y -= s;

				return *this;
			}

			Vector2& operator -=(const float s) {
				return subScalar(s);
			}

			Vector2& subVectors(const Vector2& a, const Vector2& b)
			{
				x = a.x - b.x;
				y = a.y - b.y;

				return *this;
			}

			Vector2& multiply(const Vector2& v)
			{
				x *= v.x;
				y *= v.y;

				return *this;
			}
			Vector2& operator *= (const Vector2 v) {
				return multiply(v);
			}

			Vector2& multiplyScalar(const float scalar)
			{
				x *= scalar;
				y *= scalar;

				return *this;
			}

			Vector2& operator *= (const float scalar) {
				return multiplyScalar(scalar);
			}

			Vector2& divide(const Vector2& v)
			{
				x /= v.x;
				y /= v.y;

				return *this;
			}

			Vector2& operator /= (const Vector2& v) {
				return divide(v);
			}
			Vector2& divideScalar(const float scalar)
			{
				return multiplyScalar(1 / scalar);
			}

			Vector2& operator /= (const float scalar) {
				return divideScalar(scalar);
			}

			Vector2& applyMatrix3(const Matrix3& m);

			Vector2& min(const Vector2& v)
			{
				x = std::min(x, v.x);
				y = std::min(y, v.y);

				return *this;
			}

			Vector2& max(const Vector2& v)
			{
				x = std::max(x, v.x);
				y = std::max(y, v.y);

				return *this;
			}

			Vector2& clamp(const Vector2& min, const Vector2& max)
			{
				x = std::max(min.x, std::min(max.x, x));
				y = std::max(min.y, std::min(max.y, y));

				return *this;
			}

			Vector2& clampScalar(float minVal, float maxVal)
			{
				x = std::max(minVal, std::min(maxVal, x));
				y = std::max(minVal, std::min(maxVal, y));

				return *this;
			}

			Vector2& clampLength(float min, float max)
			{
				float len = length();

				return divideScalar(len > 0 ? len : 1).multiplyScalar(std::max(min, std::min(max, len)));
			}

			Vector2& floor()
			{
				x = std::floor(x);
				y = std::floor(y);

				return *this;
			}

			Vector2& ceil()
			{
				x = std::ceil(x);
				y = std::ceil(y);

				return *this;
			}

			Vector2& round()
			{
				x = std::round(x);
				y = std::round(y);

				return *this;
			}

			Vector2& roundToZero()
			{
				x = (x < 0) ? std::ceil(x) : std::floor(x);
				y = (y < 0) ? std::ceil(y) : std::floor(y);

				return *this;
			}

			Vector2& negate()
			{
				x = -x;
				y = -y;

				return *this;
			}

			float dot(const Vector2& v)
			{
				return x * v.x + y * v.y;
			}

			float cross(const Vector2& v)
			{
				return x * v.x - y * v.y;
			}

			const float lengthSq() const
			{
				return x * x + y * y;
			}

			const float length() const
			{
				return (float)std::sqrt(x * x + y * y);
			}

			const float manhattanLength()
			{
				return std::abs(x) + std::abs(y);
			}

			Vector2& normalize()
			{
				float len = length();

				return divideScalar(len > 0 ? len : 1);
			}

			float angle()
			{
				float angle = std::atan2(-y, -x) + (float)std::_Pi;
				return angle;
			}

			float distanceTo(const Vector2& v)
			{
				return std::sqrt(distanceToSquared(v));
			}

			float distanceToSquared(const Vector2& v)
			{
				float dx = x - v.x;
				float dy = y - v.y;

				return dx * dx + dy * dy;
			}

			float manhattanDistanceTo(const Vector2& v)
			{
				return std::abs(x - v.x) + std::abs(y - v.y);
			}

			Vector2& setLength(float length)
			{
				return normalize().multiplyScalar(length);
			}

			Vector2& lerp(const Vector2& v, float alpha)
			{
				x += (v.x - x) * alpha;
				y += (v.y - y) * alpha;

				return *this;
			}

			Vector2& lerpVectors(const Vector2& v1, const Vector2& v2, float alpha)
			{
				return subVectors(v2, v1).multiplyScalar(alpha).add(v1);
			}

			bool equals(const Vector2& v) const
			{
				return (v.x == x && v.y == y);
			}
			Vector2& fromArray(const std::vector<float> array, unsigned offset = 0)
			{
				x = offset < array.size() ? array[offset] : std::numeric_limits<float>::quiet_NaN();
				y = offset < array.size() ? array[offset + 1] : std::numeric_limits<float>::quiet_NaN();

				return *this;
			}

			Vector2& fromArray(const float* array, unsigned arrayLength,unsigned offset = 0)
			{
				x = offset < arrayLength ? array[offset] : std::numeric_limits<float>::quiet_NaN();
				y = offset < arrayLength ? array[offset + 1] : std::numeric_limits<float>::quiet_NaN();

				return *this;
			}

			const float* toArray(float* array, unsigned offset = 0) const
			{
				float a[2];
				if (array == nullptr) array = a;

				array[offset] = x;
				array[offset + 1] = y;

				return array;
			}
			
			/*Vector2& fromBufferAttribute(const BufferAttribute<float>& attribute, unsigned index)
			{

			}*/

			Vector2& rotateAround(const Vector2& center, float angle)
			{
				float c = std::cos(angle), s = std::sin(angle);

				float _x = x - center.x;
				float _y = y - center.y;

				x = _x * c - _y * s + center.x;
				y = _x * s + _y * c + center.y;

				return *this;
			}
			std::string to_string() {
				std::stringstream stream;
				stream << "x=" << std::to_string(x) << ",y=" << std::to_string(y);				
				return stream.str();
			}
		};
	}
}
#endif

