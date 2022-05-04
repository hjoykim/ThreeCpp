#ifndef THREE_MATRIX4_H
#define THREE_MATRIX4_H
#include <sstream>
namespace three
{
	namespace math {

		class Vector3;		
		class Euler;
		class Quaternion;
		class Matrix4
		{
		public:
			float elements[16];

			Matrix4();

			Matrix4(const Matrix4& matrix);

			Matrix4(
				const float n11, const float n12, const float n13, const float n14,
				const float n21, const float n22, const float n23, const float n24,
				const float n31, const float n32, const float n33, const float n34,
				const float n41, const float n42, const float n43, const float n44) {
				set(n11, n12, n13, n14,n21, n22, n23,n24, n31, n32, n33,n34,n41,n42,n43,n44);
			}

			Matrix4& set(
				const float n11, const float n12, const float n13, const float n14,
				const float n21, const float n22, const float n23, const float n24,
				const float n31, const float n32, const float n33, const float n34,
				const float n41, const float n42, const float n43, const float n44);

			float operator[] (unsigned index) const { return elements[index]; }

			Matrix4& identity();

			Matrix4& clone(Matrix4* target);

			Matrix4& copy(const Matrix4& m);

			Matrix4& copyPosition(const Matrix4& m);

			Matrix4& extractBasis(Vector3& xAxis, Vector3& yAxis, Vector3& zAxis);

			Matrix4& makeBasis(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);

			/**
			 * Copies the rotation component of the supplied matrix m into this matrix rotation component.
			 */
			Matrix4& extractRotation(const Matrix4& m);

			Matrix4& makeRotationFromEuler(const Euler& euler);
			
			Matrix4& makeRotationFromQuaternion(const Quaternion& q);
			/**
			 * Constructs a rotation matrix, looking from eye towards center with defined up vector.
			 */
			Matrix4& lookAt(const Vector3& eye, const Vector3& target, const Vector3& up);

			/**
			 * Multiplies this matrix by m.
			 */
			Matrix4& multiply(const Matrix4& m);

			Matrix4& premultiply(const Matrix4& m);

			/**
			 * Sets this matrix to a x b.
			 */
			Matrix4& multiplyMatrices(const Matrix4& a, const Matrix4& b);

			/**
			 * Multiplies this matrix by s.
			 */
			Matrix4& multiplyScalar(const float s);

			/**
			 * Computes determinant of this matrix.
			 * Based on http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/fourD/index.htm
			 */
			float determinant();

			/**
			 * Transposes this matrix.
			 */
			Matrix4& transpose();

			/**
			 * Sets the position component for this matrix from vector v.
			 */
			Matrix4& setPosition(const Vector3& v);
			
			Matrix4& setPosition(const float x, const float y, const float z);

			/**
			 * Sets this matrix to the inverse of matrix m.
			 * Based on http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/fourD/index.htm.
			 */
			Matrix4& getInverse(const Matrix4& m);

			/**
			 * Multiplies the columns of this matrix by vector v.
			 */
			Matrix4& scale(const Vector3& v);

			float getMaxScaleOnAxis();
			/**
			 * Sets this matrix as translation transform.
			 */
			Matrix4& makeTranslation(const float x, const float y, const float z);

			/**
			 * Sets this matrix as rotation transform around x axis by theta radians.
			 *
			 * @param theta Rotation angle in radians.
			 */
			Matrix4& makeRotationX(const float theta);

			/**
			 * Sets this matrix as rotation transform around y axis by theta radians.
			 *
			 * @param theta Rotation angle in radians.
			 */
			Matrix4& makeRotationY(const float theta);

			/**
			 * Sets this matrix as rotation transform around z axis by theta radians.
			 *
			 * @param theta Rotation angle in radians.
			 */
			Matrix4& makeRotationZ(const float theta);

			/**
			 * Sets this matrix as rotation transform around axis by angle radians.
			 * Based on http://www.gamedev.net/reference/articles/article1199.asp.
			 *
			 * @param axis Rotation axis.
			 * @param theta Rotation angle in radians.
			 */
			Matrix4& makeRotationAxis(const Vector3& axis, const float angle);

			/**
			 * Sets this matrix as scale transform.
			 */
			Matrix4& makeScale(const float x, const float y, const float z);

			Matrix4& makeShear(const float x, const float y, const float z);

			/**
			 * Sets this matrix to the transformation composed of translation, rotation and scale.
			 */
			Matrix4& compose(const Vector3& position, const Quaternion& quaternion, const Vector3& scale);

			/**
			 * Decomposes this matrix into the translation, rotation and scale components.
			 * If parameters are not passed, new instances will be created.
			 */
			Matrix4& decompose(Vector3* position, Quaternion* quaternion, Vector3* scale);

			/**
			 * Creates a frustum matrix.
			 */
			Matrix4& makePerspective(const float left, const float right, const float top, const float bottom, const float _near, const float _far);

			/**
			 * Creates an orthographic projection matrix.
			 */
			Matrix4& makeOrthographic(const float left, const float right, const float top, const float bottom, const float _near, const float _far);
			
			bool equals(const Matrix4& matrix);

			/**
			 * Sets the values of this matrix from the provided array.
			 * @param array the source array.
			 * @param offset (optional) offset into the array. Default is 0.
			 */
			Matrix4& fromArray(const float* array, unsigned offset=0);

			

			/**
			 * Returns an array with the values of this matrix, or copies them into the provided array.
			 * @param array (optional) array to store the matrix to. If this is not provided, a new array will be created.
			 * @param offset (optional) optional offset into the array.
			 * @return The created or provided array.
			 */
			float* toArray(float* array, unsigned offset=0);
			
			std::string to_string() {
				std::stringstream stream;
				stream << "Matrix4=" << std::to_string(determinant());
				return stream.str();
			}


		};
	}
}
#endif