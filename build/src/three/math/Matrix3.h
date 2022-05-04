#ifndef THREE_MATRIX3_H
#define THREE_MATRIX3_H
#include <three/utils/osdecl.h>
#include <sstream>
namespace three
{
	namespace math {
		class Vector3;

		class Matrix4;

		class Quaternion;

		class Euler;

		class Matrix3
		{
		public:

			float elements[9];

			Matrix3();

			Matrix3(float n11, float n12, float n13, float n21, float n22, float n23, float n31, float n32, float n33) {
				set(n11, n12, n13, n21, n22, n23, n31, n32, n33);
			}

			Matrix3& set(float n11, float n12, float n13, float n21, float n22, float n23, float n31, float n32, float n33);

			Matrix3(const Matrix3& matrix);

			float operator[] (unsigned index) const { return elements[index]; }

			Matrix3& identity();

			Matrix3& clone(Matrix3* target);

			Matrix3& copy(const Matrix3& m);

			Matrix3& extractBasis(Vector3& xAxis, Vector3& yAxis, Vector3& zAxis);

			Matrix3& setFromMatrix4(const Matrix4& m);	

			Matrix3& multiplyScalar(float s);

			float determinant();

			Matrix3& getInverse(const Matrix3& matrix);

			/**
			 * Transposes this matrix in place.
			 */
			Matrix3& transpose();

			Matrix3& getNormalMatrix(const Matrix4& matrix4);

			/**
			 * Transposes this matrix into the supplied array r, and returns itself.
			 */
			Matrix3& transposeIntoArray(float* r);

			Matrix3& setUvTransform(float tx, float ty, float sx, float sy, float rotation, float cx, float cy);

			Matrix3& scale(float sx, float sy);

			Matrix3& rotate(float theta);

			Matrix3& translate(float tx, float ty);

			bool equals(const Matrix3& matrix);

			/**
			 * Sets the values of this matrix from the provided array.
			 * @param array the source array.
			 * @param offset (optional) offset into the array. Default is 0.
			 */
			Matrix3& fromArray(float* array, unsigned offset = 0);



			/**
			 * Returns an array with the values of this matrix, or copies them into the provided array.
			 * @param array (optional) array to store the matrix to. If this is not provided, a new array will be created.
			 * @param offset (optional) optional offset into the array.
			 * @return The created or provided array.
			 */
			float* toArray(float* array, unsigned offset = 0);

			std::string to_string() {
				std::stringstream stream;
				stream << "Matrix3=" << std::to_string(determinant());
				return stream.str();
			}
			/**
			 * Multiplies this matrix by m.
			 */
			Matrix3& multiply(const Matrix3& m);

			Matrix3& premultiply(const Matrix3& m);

			/**
			 * Sets this matrix to a x b.
			 */
			Matrix3& multiplyMatrices(const Matrix3& a, const Matrix3& b);


			Matrix3& getInverse(const Matrix4& matrix);

		};
	}
}
#endif
