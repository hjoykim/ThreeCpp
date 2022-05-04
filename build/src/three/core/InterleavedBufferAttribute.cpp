//#include "InterleavedBufferAttribute.h"
//#include <three/math/Vector3.h>
//#include <three/math/Matrix4.h>
//namespace three {
//	namespace interleavedbufferattribute {
//		Vector3 _vector;
//	}
//	using namespace interleavedbufferattribute;
//	template<typename Type>
//	InterleavedBufferAttribute<Type>& InterleavedBufferAttribute<Type>::applyMatrix4(const Matrix4& m) {
//		
//		for (unsigned i = 0;i< this->data.count; i++) {
//
//			_vector.x = getX(i);
//			_vector.y = getY(i);
//			_vector.z = getZ(i);
//
//			_vector.applyMatrix4(m);
//
//			setXYZ(i, _vector.x, _vector.y, _vector.z);
//
//		}
//		return *this;
//	}
//	template<typename Type>
//	Type InterleavedBufferAttribute<Type>::getX(unsigned index) const
//	{
//		return data._array[index * data.stride + offset];
//	}
//	template<typename Type>
//	InterleavedBufferAttribute<Type>& InterleavedBufferAttribute<Type>::setX(unsigned index, Type x)
//	{
//		data._array[index * data.stride + offset] = x;
//
//		return *this;
//	}
//	template<typename Type>
//	Type InterleavedBufferAttribute<Type>::getY(unsigned index) const
//	{
//		return data._array[index * data.stride + offset+1];
//	}
//	template<typename Type>
//	InterleavedBufferAttribute<Type>& InterleavedBufferAttribute<Type>::setY(unsigned index, Type y)
//	{
//		data._array[index * data.stride + offset+1] = y;
//	}
//	template<typename Type>
//	Type InterleavedBufferAttribute<Type>::getZ(unsigned index) const
//	{
//		return data._array[index * data.stride + offset+2];
//	}
//	template<typename Type>
//	InterleavedBufferAttribute<Type>& InterleavedBufferAttribute<Type>::setZ(unsigned index, Type z)
//	{
//		data._array[index * data.stride + offset+2] = z;
//	}
//	template<typename Type>
//	Type InterleavedBufferAttribute<Type>::getW(unsigned index) const
//	{
//		return data._array[index * data.stride + offset+3];
//	}
//	template<typename Type>
//	InterleavedBufferAttribute<Type>& InterleavedBufferAttribute<Type>::setW(unsigned index, Type w)
//	{
//		data._array[index * data.stride + offset+3] = w;
//	}
//	template<typename Type>
//	InterleavedBufferAttribute<Type>& InterleavedBufferAttribute<Type>::setXY(unsigned index, Type x, Type y)
//	{
//		index = index * data.stride + offset;
//		data._array[index + 0] = x;
//		data._array[index + 1] = y;
//
//		return *this;
//	}
//	template<typename Type>
//	InterleavedBufferAttribute<Type>& InterleavedBufferAttribute<Type>::setXYZ(unsigned index, Type x, Type y, Type z)
//	{
//		index = index * data.stride + offset;
//		data._array[index + 0] = x;
//		data._array[index + 1] = y;
//		data._array[index + 2] = z;
//
//		return *this;
//	}
//	template<typename Type>
//	InterleavedBufferAttribute<Type>& InterleavedBufferAttribute<Type>::setXYZW(unsigned index, Type x, Type y, Type z, Type w)
//	{
//		index = index * data.stride + offset;
//		data._array[index + 0] = x;
//		data._array[index + 1] = y;
//		data._array[index + 2] = z;
//		data._array[index + 3] = w;
//
//		return *this;
//	}
//}