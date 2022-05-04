#ifndef SPHERICALHARMONICS_THREE_H
#define SPHERICALHARMONICS_THREE_H
#include <three/math/Vector3.h>
#include <vector>
namespace three {
	namespace math {

		class SphericalHarmonics3 {
		public:
			std::vector<Vector3> coefficients;

			SphericalHarmonics3() {}

			SphericalHarmonics3(std::vector<Vector3>& coefficients) :coefficients(coefficients) {};

			virtual ~SphericalHarmonics3() = default;

			SphericalHarmonics3& set(const std::vector<Vector3>& coefficients) {
				return *this;
			}

			SphericalHarmonics3& zero() {
				return *this;
			}

			SphericalHarmonics3& add(const SphericalHarmonics3& sh)
			{
				return *this;
			}

			SphericalHarmonics3& scale(float s) {
				return *this;
			}

			SphericalHarmonics3& lerp(const SphericalHarmonics3& sh, float alpha) {
				return *this;
			}

			bool equals(const SphericalHarmonics3& sh) {
				return false;
			}

			SphericalHarmonics3& copy(const SphericalHarmonics3& sh) {
				return *this;
			}

			SphericalHarmonics3* clone() {
				return nullptr;
			}

			/**
			 * Sets the values of this spherical harmonics from the provided array.
			 * @param array the source array.
			 * @param offset (optional) offset into the array. Default is 0.
			 */
			SphericalHarmonics3& fromArray(float* array, unsigned offset = 0) {
				return *this;
			}

			
			

			/**
			 * Returns an array with the values of this spherical harmonics, or copies them into the provided array.
			 * @param array (optional) array to store the spherical harmonics to. If this is not provided, a new array will be created.
			 * @param offset (optional) optional offset into the array.
			 * @return The created or provided array.
			 */
			void toArray(float* array, unsigned offset = 0) {}

			

			Vector3 getAt(const Vector3& normal, const Vector3& target) {
				return Vector3();
			}

			Vector3 getIrradianceAt(const Vector3& normal, const Vector3& target) {
				return Vector3();
			}

			static void getBasisAt(const Vector3& normal, float* shBasis) {}


		};
	}
}
#endif
