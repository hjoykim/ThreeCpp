#pragma once
#ifndef THREECPP_MATHUTILS
#define THREECPP_MATHUTILS
#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#include <math.h>
#endif

#include <cmath>
#include <random>

extern "C"
{
#ifdef _MSC_VER
#include <rpc.h>
#pragma comment(lib,"Rpcrt4.lib")
#else
#include <uuid/uuid.h>
#endif

}
namespace three {
	namespace math {

		inline const float PI() { return 3.1415926535897932384f; }//std::atan(1.f)/4;

		inline const float LN2() { return 0.6931471805599453094f; }

		inline const float INF() { return std::numeric_limits<float>::infinity(); }//std::numeric_limits<float>::infinity();

		template < typename T > inline T sqrt(T t) { return std::sqrt(t); }

		template < typename T > inline T abs(T t) { return std::abs(t); }

		template < typename T > inline T acos(T t) { return std::acos(t); }

		template < typename T > inline T asin(T t) { return std::asin(t); }

		template < typename T > inline T atan2(T y, T x) { return std::atan2(y, x); }

		template < typename T > inline T atan(T t) { return std::atan(t); }

		template < typename T > inline T cos(T t) { return std::cos(t); }

		template < typename T > inline T sin(T t) { return std::sin(t); }

		template < typename T > inline T tan(T t) { return std::tan(t); }

		template < typename T > inline T log(T t) { return std::log(t); }

		template < typename T, typename U >  inline T pow(T a, U b) { return std::pow(a, b); }

#if defined(_MSC_VER)
		template < typename T > inline T round(T n) { return (n > (T)0) ? std::floor(n + (T)0.5) : std::ceil(n - (T)0.5); }
#else
		template < typename T > inline T round(T t) { return std::round(t); }
#endif
		template < typename T > inline T ceil(T t) { return std::ceil(t); }
		template < typename T > inline T floor(T t) { return std::floor(t); }

		template < typename T > inline T fmod(T a, T b) { return std::fmod(a, b); }



		template < typename T > inline T clamp(T x, T a, T b) { return x < a ? a : ((x > b) ? b : x); }
		template < typename T > inline T clampBottom(T x, T a) { return x < a ? a : x; }

		// Linear mapping from range <a1, a2> to range <b1, b2>
		template < typename T >

		inline T mapLinear(T x, T a1, T a2, T b1, T b2) {
			return b1 + (x - a1) * (b2 - b1) / (a2 - a1);
		}

		// MinGW crashes on std::random_device initialization
#if !defined(__MINGW32__)

		template < typename T >

		inline typename std::enable_if<std::is_floating_point<T>::value, T>::type
			randomT(T low, T high) {
			std::random_device rd;
			std::mt19937 rng(rd());
			std::uniform_real_distribution<T> dis(low, high);
			return dis(rng);
		}

		template < typename T >
		inline typename std::enable_if<!std::is_floating_point<T>::value, T>::type
			randomT(T low, T high) {
			std::random_device rd;
			std::mt19937 rng(rd());
			std::uniform_int_distribution<T> dis(low, high);
			return dis(rng);
		}

#else

		template< typename T >
		inline T randomT(T low, T high) {
			return (low + _cast<double>(rand()) / ((unsigned long long)RAND_MAX + 1) * (high - low));
		}

#endif // !defined(__MINGW32__)

		inline float random(float low = 0, float high = 1) {
			return randomT(low, high);
		}

		// Random float from <0, 1> with 16 bits of randomness
		// (standard Math.random() creates repetitive patterns when applied over larger space)
		inline float random16() {
			return (65280 * random() + 255 * random()) / 65535;
		}

		// Random integer from <low, high> interval
		inline int randInt(int low, int high) {
			return low + (int)floor(random() * (high - low + 1));
		}

		// Random float from <low, high> interval
		inline float randFloat(float low, float high) {
			return random(low, high);
		}

		// Random float from <-range/2, range/2> interval
		inline float randFloatSpread(float range) {
			return range * (0.5f - random());
		}

		template < typename T >
		inline int sign(T x) {
			return (x < 0) ? -1 : ((x > 0) ? 1 : 0);
		}

		inline bool isPowerOfTwo(int value) {
			return (value != 0) && ((value & (value - 1)) == 0);
		}

		inline int upperPowerOfTwo(int value) {
			return (int)pow(2.f, ceil(log((float)value) / LN2()));
		}

		inline int lowerPowerOfTwo(int value) {
			return (int)pow(2.f, floor(log((float)value) / LN2()));
		}

		inline int nearestPowerOfTwo(int value) {
			return (int)pow(2.f, round(log((float)value) / LN2()));
		}

		inline std::string generateUUID()
		{
#ifdef _MSC_VER
			UUID id;

			if (RPC_S_OK != UuidCreate(&id))
				id = GUID_NULL;

			unsigned char* str;

			if (UuidToStringA(&id, (RPC_CSTR*)&str))
				return std::string();

			std::string s((char*)str);

			RpcStringFreeA(&str);
#else
			uuid_t uuid;
			uuid_generate_random(uuid);
			char s[37];
			uuid_unparse(uuid, s);
#endif
			std::transform(s.begin(), s.end(), s.begin(), ::toupper);
			return s;
		}

		inline int euclideanModulo(int n, int m)
		{
			if (m == 0) return (int)NAN;
			return ((n % m) + m) % m;
		}

		template <typename T> T lerp(T x, T y, T t)
		{
			return (1 - t) * x + t * y;
		}

		template <typename T> T smoothstep(T x, T min, T max) {

			if (x <= min) return 0;
			if (x >= max) return 1;

			x = (x - min) / (max - min);

			return x * x * (3 - 2 * x);
		}

		template <typename T> T smootherstep(T x, T min, T max)
		{
			if (x <= min) return 0;
			if (x >= max) return 1;

			x = (x - min) / (max - min);

			return x * x * x * (x * (x * 6 - 15) + 10);
		}

		inline double degToRad(double degrees)
		{
			return degrees * M_PI / 180.0;
		}

		inline double radToDeg(double radians)
		{
			return radians * 180.0 / M_PI;
		}

		inline int ceilPowerOfTwo(double value)
		{
			return (int)std::pow(2, std::ceil(std::log(value) / M_LN2));
		}

		inline int floorPowerOfTwo(double value)
		{
			return (int)std::pow(2, std::floor(std::log(value) / M_LN2));
		}
	}
}
#endif
