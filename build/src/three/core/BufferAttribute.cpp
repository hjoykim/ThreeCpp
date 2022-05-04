//#include "BufferAttribute.h"
//#include <three/math/Vector2.h>
//#include <three/math/Vector3.h>
//#include <three/math/Vector4.h>
//#include <three/math/Matrix3.h>
//#include <three/math/Matrix4.h>

//using namespace three::math;
//namespace three {
	
	//template<typename Type>
	//BufferAttribute<Type>& BufferAttribute<Type>::copy(BufferAttribute<Type>& source)
	//{
	//	this->name = source.name;
	//		

	//	//this->array = new Type[source.size];
	//	if (source.array.size() > 0) {
	//		if (array.size() != source.array.size())
	//			array.resize(source.array.size());
	//		std::copy(source.array.begin(), source.array.end(), this->array.begin());
	//	}
	//	else {
	//		array.clear();
	//	}
	//	

	//		

	//	//std::memcpy(this->array, source.array, sizeof(Type) * source.size);

	//	this->itemSize = source.itemSize;

	//	this->size = source.size;

	//	this->count = source.count;

	//	this->normalized = source.normalized;

	//	this->usage = source.usage;

	//	return *this;

	//}
	//template<typename Type>
	//BufferAttribute<Type>& BufferAttribute<Type>::clone(BufferAttribute* target)
	//{

	//	target->copy(*this);

	//	return *target;
	//}
	//template<typename Type>
	//BufferAttribute<Type>& BufferAttribute<Type>::copyAt(unsigned index1, BufferAttribute& attribute, unsigned index2)
	//{
	//	index1 *= this->itemSize;
	//	index2 *= attribute.itemSize;

	//	for (unsigned i = 0; i < this->itemSize; i++) {

	//		this->array[index1 + i] = attribute.array[index2 + i];

	//	}

	//	return *this;
	//}
	//template<typename Type>
	//BufferAttribute<Type>& BufferAttribute<Type>::copyArray(Type* array, unsigned length)
	//{
	//	std::memcpy(&this->array[0], array, sizeof(Type) * length);

	//	return *this;
	//}
	//
	//template<typename Type>
	//BufferAttribute<Type>& BufferAttribute<Type>::copyVector2sArray(Vector2* vectors, unsigned length)
	//{
	//	//Type* array = this->array;
	//	/*if (this->array.size() == 0 || this->array.size() != length)
	//		this->array.resize(length);*/
	//	unsigned offset = 0;

	//	for (unsigned i = 0; i < length; i++) {

	//		auto vector = vectors[i];


	//		array[offset++] = vector.x;
	//		array[offset++] = vector.y;

	//	}
	//	return *this;
	//}
	//template<typename Type>
	//BufferAttribute<Type>& BufferAttribute<Type>::copyVector3sArray(Vector3* vectors, unsigned length)
	//{
	//	//Type* array = this->array;
	//	//if (this->array.size() == 0 || this->array.size() != length)
	//	//	this->array.resize(length);
	//	unsigned offset = 0;

	//	for (unsigned i = 0; i < length; i++) {

	//		auto vector = vectors[i];


	//		array[offset++] = vector.x;
	//		array[offset++] = vector.y;
	//		array[offset++] = vector.z;

	//	}
	//	return *this;
	//}

	//template<typename Type>
	//BufferAttribute<Type>& BufferAttribute<Type>::copyColor3sArray(Color* colors, unsigned length)
	//{
	//	//Type* array = this->array;
	//	/*if (this->array.size() == 0 || this->array.size() != length)
	//		this->array.resize(length);*/
	//	unsigned offset = 0;

	//	for (unsigned i = 0; i < length; i++) {

	//		auto color = colors[i];


	//		array[offset++] = color.r;
	//		array[offset++] = color.g;
	//		array[offset++] = color.b;

	//	}
	//	return *this;
	//}
	//
	//template<typename Type>
	//BufferAttribute<Type>& BufferAttribute<Type>::copyVector4sArray(Vector4* vectors, unsigned length)
	//{
	//	//Type* array = this->array;
	//	/*if (this->array.size() == 0 || this->array.size() != length)
	//		this->array.resize(length);*/
	//	unsigned offset = 0;

	//	for (unsigned i = 0; i < length; i++) {

	//		auto vector = vectors[i];


	//		array[offset++] = vector.x;
	//		array[offset++] = vector.y;
	//		array[offset++] = vector.z;
	//		array[offset++] = vector.w;

	//	}
	//	return *this;
	//}
	//template<typename Type>
	//BufferAttribute<Type>& BufferAttribute<Type>::applyMatrix3(const Matrix3& m)
	//{
	//	Vector3 _vector;
	//	for (int i = 0; i < this->count; i++) {

	//		_vector.x = this->getX(i);
	//		_vector.y = this->getY(i);
	//		_vector.z = this->getZ(i);

	//		_vector.applyMatrix3(m);

	//		this->setXYZ(i, _vector.x, _vector.y, _vector.z);

	//	}

	//	return *this;
	//}
	//template<typename Type>
	//BufferAttribute<Type>& BufferAttribute<Type>::applyMatrix4(const Matrix4& m)
	//{
	//	Vector3 _vector;
	//	for (int i = 0; i < this->count; i++) {

	//		_vector.x = this->getX(i);
	//		_vector.y = this->getY(i);
	//		_vector.z = this->getZ(i);

	//		_vector.applyMatrix4(m);

	//		this->setXYZ(i, _vector.x, _vector.y, _vector.z);

	//	}

	//	return *this;
	//}
	//template<typename type>
	//BufferAttribute<type>& BufferAttribute<type>::applyNormalMatrix(const Matrix3& m)
	//{
	//	vector3 _vector;
	//	for (int i = 0; i < this->count; i++) {

	//		_vector.x = this->getx(i);
	//		_vector.y = this->gety(i);
	//		_vector.z = this->getz(i);

	//		_vector.applynormalmatrix(m);

	//		this->setxyz(i, _vector.x, _vector.y, _vector.z);

	//	}

	//	return *this;
	//}
	//template<typename Type>
	//BufferAttribute<Type>& BufferAttribute<Type>::transformDirection(const Matrix4& m)
	//{
	//	Vector3 _vector;
	//	for (int i = 0; i < this->count; i++) {

	//		_vector.x = this->getX(i);
	//		_vector.y = this->getY(i);
	//		_vector.z = this->getZ(i);

	//		_vector.transformDirection(m);

	//		this->setXYZ(i, _vector.x, _vector.y, _vector.z);

	//	}

	//	return *this;
	//}
	//template<typename Type>
	//BufferAttribute<Type>& BufferAttribute<Type>::set(Type* value, unsigned length, unsigned offset)
	//{
	//	/*if (this->array.size() == 0 || this->array.size() != length)
	//		this->array.resize(length);*/

	//	std::memcpy(&this->array[0]+offset, value, sizeof(Type) * length);



	//	return *this;
	//}
	//template<typename Type>
	//Type BufferAttribute<Type>::getX(unsigned index) const
	//{
	//	return this->array[index * this->itemSize];
	//}
	//template<typename Type>
	//BufferAttribute<Type>& BufferAttribute<Type>::setX(unsigned index, Type x)
	//{
	//	this->array[index * this->itemSize] = x;
	//	return *this;
	//}
	//template<typename Type>
	//Type BufferAttribute<Type>::getY(unsigned index) const
	//{
	//	return this->array[index * this->itemSize + 1];
	//}
	//template<typename Type>
	//BufferAttribute<Type>& BufferAttribute<Type>::setY(unsigned index, Type y)
	//{
	//	this->array[index * this->itemSize + 1] = y;
	//	return *this;
	//}
	//template<typename Type>
	//Type BufferAttribute<Type>::getZ(unsigned index) const
	//{
	//	return this->array[index * this->itemSize + 2];
	//}
	//template<typename Type>
	//BufferAttribute<Type>& BufferAttribute<Type>::setZ(unsigned index, Type z)
	//{
	//	this->array[index * this->itemSize + 2] = z;
	//	return *this;
	//}
	//template<typename Type>
	//Type BufferAttribute<Type>::getW(unsigned index) const
	//{
	//	return this->array[index * this->itemSize + 3];
	//}
	//template<typename Type>
	//BufferAttribute<Type>& BufferAttribute<Type>::setW(unsigned index, Type w)
	//{
	//	this->array[index * this->itemSize + 3] = w;
	//	return *this;
	//}
	//template<typename Type>
	//BufferAttribute<Type>& BufferAttribute<Type>::setXY(unsigned index, Type x, Type y)
	//{
	//	index *= this->itemSize;

	//	this->array[index + 0] = x;
	//	this->array[index + 1] = y;

	//	return *this;
	//}
	//template<typename Type>
	//BufferAttribute<Type>& BufferAttribute<Type>::setXYZ(unsigned index, Type x, Type y, Type z)
	//{
	//	index *= this->itemSize;

	//	this->array[index + 0] = x;
	//	this->array[index + 1] = y;
	//	this->array[index + 2] = z;

	//	return *this;
	//}
	//template<typename Type>
	//BufferAttribute<Type>& BufferAttribute<Type>::setXYZW(unsigned index, Type x, Type y, Type z, Type w)
	//{
	//	index *= this->itemSize;

	//	this->array[index + 0] = x;
	//	this->array[index + 1] = y;
	//	this->array[index + 2] = z;
	//	this->array[index + 3] = w;

	//	return *this;
	//}
//}