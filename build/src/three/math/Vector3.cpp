#include "vector3.h"
#include "Matrix3.h"
#include "Matrix4.h"
#include "Euler.h"
#include "Quaternion.h"
#include <three/core/BufferAttribute.h>
#include "Spherical.h"
#include "Cylindrical.h"
#include <three/cameras/Camera.h>
//#include <three/cameras/Camera.h>
namespace three {
	namespace math {
		Vector3::Vector3() :x(0), y(0), z(0) {}

		Vector3::Vector3(float s) : x(s), y(s), z(s) {}

		Vector3::Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

		Vector3::Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {}



		Vector3& Vector3::setFromSpherical(const Spherical& s)
		{
			return setFromSphericalCoords(s.radius, s.phi, s.theta);
		}

		Vector3& Vector3::setFromSphericalCoords(float r, float phi, float theta)
		{
			float sinPhiRadius = sin(phi) * r;

			x = sinPhiRadius * sin(theta);
			y = cos(phi) * r;
			z = sinPhiRadius * cos(theta);

			return *this;
		}

		Vector3& Vector3::setFromCylindrical(const Cylindrical& c)
		{
			return setFromCylindricalCoords(c.radius, c.theta, c.y);
		}


		Vector3& Vector3::setFromCylindricalCoords(float radius, float theta, float y)
		{
			this->x = radius * sin(theta);
			this->y = y;
			this->z = radius * cos(theta);

			return *this;
		}

		Vector3& Vector3::setFromMatrixPosition(const Matrix4& m)
		{

			x = m.elements[12];
			y = m.elements[13];
			z = m.elements[14];

			return *this;
		}

		Vector3& Vector3::setFromMatrixScale(const Matrix4& m)
		{
			float sx = setFromMatrixColumn(m, 0).length();
			float sy = setFromMatrixColumn(m, 1).length();
			float sz = setFromMatrixColumn(m, 2).length();

			x = sx;
			y = sy;
			z = sz;

			return *this;
		}

		Vector3& Vector3::setFromMatrixColumn(const Matrix4& matrix, unsigned index)
		{
			return this->fromArray(matrix.elements, 16,index * 4);
		}

		Vector3& Vector3::setFromMatrix3Column(const Matrix3& matrix, unsigned index)
		{
			return this->fromArray(matrix.elements, 9,index * 3);
		}		

		Vector3& Vector3::applyQuaternion(const Quaternion& q)
		{
			const float qx = q.getX(), qy = q.getY(), qz = q.getZ(), qw = q.getW();

			// calculate quat * vector

			float ix = qw * x + qy * z - qz * y,
				iy = qw * y + qz * x - qx * z,
				iz = qw * z + qx * y - qy * x,
				iw = -qx * x - qy * y - qz * z;

			// calculate result * inverse quat

			x = ix * qw + iw * -qx + iy * -qz - iz * -qy;
			y = iy * qw + iw * -qy + iz * -qx - ix * -qz;
			z = iz * qw + iw * -qz + ix * -qy - iy * -qx;

			return *this;
		}	

		Vector3& Vector3::applyEuler(const Euler& euler) {
			Quaternion _quaternion;
			return applyQuaternion(_quaternion.setFromEuler(euler));
		}

		Vector3& Vector3::applyAxisAngle(const Vector3& axis, float angle)
		{
			Quaternion _quaternion;
			return applyQuaternion(_quaternion.setFromAxisAngle(axis, angle));
		}
		Vector3& Vector3::applyMatrix3(const Matrix3& m)
		{
			float _x = x, _y = y, _z = z;
			const float* e = m.elements;

			x = e[0] * _x + e[3] * _y + e[6] * _z;
			y = e[1] * _x + e[4] * _y + e[7] * _z;
			z = e[2] * _x + e[5] * _y + e[8] * _z;

			return *this;
		}
		Vector3& Vector3::applyMatrix4(const Matrix4& m)
		{
			float _x = x, _y = y, _z = z;
			const float* e = m.elements;

			float _w = 1 / (e[3] * x + e[7] * y + e[11] * z + e[15]);

			x = (e[0] * _x + e[4] * _y + e[8] * _z + e[12]) * _w;
			y = (e[1] * _x + e[5] * _y + e[9] * _z + e[13]) * _w;
			z = (e[2] * _x + e[6] * _y + e[10] * _z + e[14]) * _w;

			return *this;
		}
		Vector3& Vector3::project(const Camera& camera)
		{
			return applyMatrix4(camera.matrixWorldInverse).applyMatrix4(camera.projectionMatrix);
		}

		Vector3& Vector3::unproject(const Camera& camera)
		{
			return applyMatrix4(camera.projectionMatrixInverse).applyMatrix4(camera.matrixWorld);

		}
		Vector3& Vector3::transformDirection(const Matrix4& m)
		{
			// input: THREE.Matrix4 affine matrix
			// vector interpreted as a direction

			float _x = x, _y = y, _z = z;
			const float* e = m.elements;

			x = e[0] * _x + e[4] * _y + e[8] * _z;
			y = e[1] * _x + e[5] * _y + e[9] * _z;
			z = e[2] * _x + e[6] * _y + e[10] * _z;

			return normalize();
		}
		Vector3& Vector3::min(const Vector3& v)
		{
			x = std::min(x, v.x);
			y = std::min(y, v.y);
			z = std::min(z, v.z);

			return *this;
		}
		Vector3& Vector3::max(const Vector3& v)
		{
			x = std::max(x, v.x);
			y = std::max(y, v.y);
			z = std::max(z, v.z);

			return *this;
		}
		Vector3& Vector3::clamp(const Vector3& min, const Vector3& max)
		{
			// assumes min < max, componentwise

			x = std::max(min.x, std::min(max.x, x));
			y = std::max(min.y, std::min(max.y, y));
			z = std::max(min.z, std::min(max.z, z));

			return *this;
		}
		Vector3& Vector3::clampScalar(float min, float max)
		{
			x = std::max(min, std::min(max, x));
			y = std::max(min, std::min(max, y));
			z = std::max(min, std::min(max, z));

			return *this;
		}
		Vector3& Vector3::clampLength(float min, float max)
		{
			float len = length();

			len = len == 0 ? 1 : len;

			return divideScalar(len).multiplyScalar(std::max(min, std::min(max, len)));
		}
	}
}