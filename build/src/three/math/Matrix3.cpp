#include "Matrix3.h"
#include "Vector3.h"
#include "Matrix4.h"
#include "Quaternion.h"
#include "Euler.h"
#include <three/cameras/Camera.h>
namespace three
{
	namespace math {
		Matrix3::Matrix3()
		{
			identity();
		}
		Matrix3& Matrix3::set(float n11, float n12, float n13, float n21, float n22, float n23, float n31, float n32, float n33)
		{
			float* te = elements;

			te[0] = n11; te[3] = n12; te[6] = n13;
			te[1] = n21; te[4] = n22; te[7] = n23;
			te[2] = n31; te[5] = n32; te[8] = n33;

			return *this;
		}

		Matrix3::Matrix3(const Matrix3& matrix)
		{
			std::memcpy(elements, matrix.elements, sizeof(elements));
		}

		Matrix3& Matrix3::identity()
		{
			return set(1, 0, 0,
				0, 1, 0,
				0, 0, 1);
		}

		Matrix3& Matrix3::clone(Matrix3* target)
		{
			*target = copy(*this);
			return *target;
		}

		Matrix3& Matrix3::copy(const Matrix3& m)
		{
			std::memcpy(elements, m.elements, sizeof(elements));

			return *this;
		}

		Matrix3& Matrix3::extractBasis(Vector3& xAxis, Vector3& yAxis, Vector3& zAxis)
		{
			xAxis.setFromMatrix3Column(*this, 0);
			yAxis.setFromMatrix3Column(*this, 1);
			zAxis.setFromMatrix3Column(*this, 2);

			return *this;
		}

		Matrix3& Matrix3::setFromMatrix4(const Matrix4& m)
		{
			const float* me = m.elements;

			set(

				me[0], me[4], me[8],
				me[1], me[5], me[9],
				me[2], me[6], me[10]

			);

			return *this;
		}

		Matrix3& Matrix3::multiplyScalar(float s)
		{
			float* te = elements;

			te[0] *= s; te[3] *= s; te[6] *= s;
			te[1] *= s; te[4] *= s; te[7] *= s;
			te[2] *= s; te[5] *= s; te[8] *= s;

			return *this;
		}

		float Matrix3::determinant()
		{
			float* te = elements;

			float a = te[0], b = te[1], c = te[2],
				d = te[3], e = te[4], f = te[5],
				g = te[6], h = te[7], i = te[8];

			return a * e * i - a * f * h - b * d * i + b * f * g + c * d * h - c * e * g;
		}

		Matrix3& Matrix3::getInverse(const Matrix3& matrix)
		{
			const float* me = matrix.elements;
			float* te = elements;

			float n11 = me[0], n21 = me[1], n31 = me[2],
				n12 = me[3], n22 = me[4], n32 = me[5],
				n13 = me[6], n23 = me[7], n33 = me[8],

				t11 = n33 * n22 - n32 * n23,
				t12 = n32 * n13 - n33 * n12,
				t13 = n23 * n12 - n22 * n13,

				det = n11 * t11 + n21 * t12 + n31 * t13;

			if (det == 0) return set(0, 0, 0, 0, 0, 0, 0, 0, 0);

			float detInv = 1 / det;

			te[0] = t11 * detInv;
			te[1] = (n31 * n23 - n33 * n21) * detInv;
			te[2] = (n32 * n21 - n31 * n22) * detInv;

			te[3] = t12 * detInv;
			te[4] = (n33 * n11 - n31 * n13) * detInv;
			te[5] = (n31 * n12 - n32 * n11) * detInv;

			te[6] = t13 * detInv;
			te[7] = (n21 * n13 - n23 * n11) * detInv;
			te[8] = (n22 * n11 - n21 * n12) * detInv;

			return *this;
		}

		Matrix3& Matrix3::transpose()
		{
			float tmp;
			float* m = elements;

			tmp = m[1]; m[1] = m[3]; m[3] = tmp;
			tmp = m[2]; m[2] = m[6]; m[6] = tmp;
			tmp = m[5]; m[5] = m[7]; m[7] = tmp;

			return *this;
		}

		Matrix3& Matrix3::getNormalMatrix(const Matrix4& matrix4)
		{
			return setFromMatrix4(matrix4).getInverse(*this).transpose();
		}

		Matrix3& Matrix3::transposeIntoArray(float* r)
		{
			float* m = elements;

			r[0] = m[0];
			r[1] = m[3];
			r[2] = m[6];
			r[3] = m[1];
			r[4] = m[4];
			r[5] = m[7];
			r[6] = m[2];
			r[7] = m[5];
			r[8] = m[8];

			return *this;
		}

		Matrix3& Matrix3::setUvTransform(float tx, float ty, float sx, float sy, float rotation, float cx, float cy)
		{
			float c = cos(rotation);
			float s = sin(rotation);

			set(
				sx * c, sx * s, -sx * (c * cx + s * cy) + cx + tx,
				-sy * s, sy * c, -sy * (-s * cx + c * cy) + cy + ty,
				0, 0, 1
			);

			return *this;
		}

		Matrix3& Matrix3::scale(float sx, float sy)
		{
			float* te = elements;

			te[0] *= sx; te[3] *= sx; te[6] *= sx;
			te[1] *= sy; te[4] *= sy; te[7] *= sy;

			return *this;
		}

		Matrix3& Matrix3::rotate(float theta)
		{
			float c = cos(theta);
			float s = sin(theta);

			float* te = elements;

			float a11 = te[0], a12 = te[3], a13 = te[6];
			float a21 = te[1], a22 = te[4], a23 = te[7];

			te[0] = c * a11 + s * a21;
			te[3] = c * a12 + s * a22;
			te[6] = c * a13 + s * a23;

			te[1] = -s * a11 + c * a21;
			te[4] = -s * a12 + c * a22;
			te[7] = -s * a13 + c * a23;

			return *this;
		}

		Matrix3& Matrix3::translate(float tx, float ty)
		{
			float* te = elements;

			te[0] += tx * te[2]; te[3] += tx * te[5]; te[6] += tx * te[8];
			te[1] += ty * te[2]; te[4] += ty * te[5]; te[7] += ty * te[8];

			return *this;
		}

		bool Matrix3::equals(const Matrix3& matrix)
		{
			float* te = elements;
			const float* me = matrix.elements;

			for (int i = 0; i < 9; i++) {

				if (te[i] != me[i]) return false;
			}

			return true;
		}

		Matrix3& Matrix3::fromArray(float* array, unsigned offset)
		{
			for (int i = 0; i < 9; i++) {

				elements[i] = array[i + offset];
			}

			return *this;
		}

		float* Matrix3::toArray(float* array, unsigned offset)
		{
			float a[9];

			if (array == nullptr) array = a;

			float* te = elements;

			array[offset] = te[0];
			array[offset + 1] = te[1];
			array[offset + 2] = te[2];

			array[offset + 3] = te[3];
			array[offset + 4] = te[4];
			array[offset + 5] = te[5];

			array[offset + 6] = te[6];
			array[offset + 7] = te[7];
			array[offset + 8] = te[8];

			return array;

		}

		Matrix3& Matrix3::multiply(const Matrix3& m)
		{
			return multiplyMatrices(*this, m);
		}

		Matrix3& Matrix3::premultiply(const Matrix3& m)
		{
			return multiplyMatrices(m, *this);
		}

		Matrix3& Matrix3::multiplyMatrices(const Matrix3& a, const Matrix3& b)
		{
			const float* ae = a.elements;
			const float* be = b.elements;
			float* te = elements;

			float a11 = ae[0], a12 = ae[3], a13 = ae[6];
			float a21 = ae[1], a22 = ae[4], a23 = ae[7];
			float a31 = ae[2], a32 = ae[5], a33 = ae[8];

			float b11 = be[0], b12 = be[3], b13 = be[6];
			float b21 = be[1], b22 = be[4], b23 = be[7];
			float b31 = be[2], b32 = be[5], b33 = be[8];

			te[0] = a11 * b11 + a12 * b21 + a13 * b31;
			te[3] = a11 * b12 + a12 * b22 + a13 * b32;
			te[6] = a11 * b13 + a12 * b23 + a13 * b33;

			te[1] = a21 * b11 + a22 * b21 + a23 * b31;
			te[4] = a21 * b12 + a22 * b22 + a23 * b32;
			te[7] = a21 * b13 + a22 * b23 + a23 * b33;

			te[2] = a31 * b11 + a32 * b21 + a33 * b31;
			te[5] = a31 * b12 + a32 * b22 + a33 * b32;
			te[8] = a31 * b13 + a32 * b23 + a33 * b33;

			return *this;
		}

		Matrix3& Matrix3::getInverse(const Matrix4& matrix)
		{
			const float* me = matrix.elements;
			float* te = elements;

			float n11 = me[0], n21 = me[1], n31 = me[2],
				n12 = me[3], n22 = me[4], n32 = me[5],
				n13 = me[6], n23 = me[7], n33 = me[8],

				t11 = n33 * n22 - n32 * n23,
				t12 = n32 * n13 - n33 * n12,
				t13 = n23 * n12 - n22 * n13,

				det = n11 * t11 + n21 * t12 + n31 * t13;

			if (det == 0) return set(0, 0, 0, 0, 0, 0, 0, 0, 0);

			float detInv = 1 / det;

			te[0] = t11 * detInv;
			te[1] = (n31 * n23 - n33 * n21) * detInv;
			te[2] = (n32 * n21 - n31 * n22) * detInv;

			te[3] = t12 * detInv;
			te[4] = (n33 * n11 - n31 * n13) * detInv;
			te[5] = (n31 * n12 - n32 * n11) * detInv;

			te[6] = t13 * detInv;
			te[7] = (n21 * n13 - n23 * n11) * detInv;
			te[8] = (n22 * n11 - n21 * n12) * detInv;

			return *this;
		}

	}
}
