#ifndef THREE_BUFFERATTRIBUTE_THREE_H
#define THREE_BUFFERATTRIBUTE_THREE_H
#include <three/utils/osdecl.h>
#include <three/utils/simplesignal.h>
#include <three/Types.h>
#include <three/math/Color.h>
#include <three/Constants.h>
#include <three/utils/sole.h>
#include <string>
#include <vector>
#include <memory>
#include <three/math/Vector2.h>
#include <three/math/Vector3.h>
#include <three/math/Vector4.h>
#include <three/math/Matrix3.h>
#include <three/math/Matrix4.h>

namespace three {
	using namespace math;
	/*namespace math {
		class Vector2;
		class Vector3;
		class Vector4;
		class Matrix3;
		class Matrix4;
	}*/

	template<typename Type>
	class BufferAttribute {
	private:
		bool _needsUpdate;


	public:
		BufferAttribute()
		{
			this->version = 0;

			this->count = 0;

			this->itemSize = 0;

			this->normalized = false;

			this->size = 0;

			this->usage = Usage::StaticDrawUsage;

			this->_needsUpdate = false;

			//this->array = nullptr;
		}

		BufferAttribute(Type* array1, unsigned size, unsigned _itemSize, bool normalized = false)
		{

			this->version = 0;

			this->size = size;

			this->itemSize = _itemSize;

			this->normalized = normalized;

			this->usage = Usage::StaticDrawUsage;

			this->_needsUpdate = false;

			this->count = this->size / this->itemSize;

			this->array.resize(size);

			if(array1!=nullptr)
				memcpy(&this->array[0], array1, sizeof(Type) * size);
		}

		BufferAttribute(const std::vector<Type>& array1, unsigned _itemSize, bool normalized = false) {
			this->version = 0;

			this->size = array1.size();

			this->itemSize = _itemSize;

			this->normalized = normalized;

			this->usage = Usage::StaticDrawUsage;

			this->_needsUpdate = false;

			this->count = this->size / this->itemSize;

			this->array.resize(size);

			std::copy(array1.begin(), array1.end(), this->array.begin());
		}

		BufferAttribute(const BufferAttribute& source) {
			this->name = source.name;

			if (source.array.size() > 0) {
				if (array.size() != source.array.size())
					array.resize(source.array.size());
				std::copy(source.array.begin(), source.array.end(), this->array.begin());
			}
			else {
				array.clear();
			}

			this->itemSize = source.itemSize;

			this->size = source.size;

			this->count = source.count;

			this->normalized = source.normalized;

			this->usage = source.usage;
		}
		virtual ~BufferAttribute() = default;

		using ptr = std::shared_ptr<BufferAttribute>;

		static ptr create() {
			return ptr(new BufferAttribute());
		}
		static ptr create(unsigned size, unsigned itemsize, bool normalized = false) {
			return ptr(new BufferAttribute(nullptr, size, itemsize, normalized));
		}

		static ptr create(Type* array, unsigned size, unsigned itemSize, bool normalized = false) {
			return ptr(new BufferAttribute(array, size, itemSize, normalized));
		}

		static ptr create(const std::vector<Type>&	array, unsigned itemSize, bool normalized = false) {
			return ptr(new BufferAttribute(array, itemSize, normalized));
		}
		std::string name;

		std::vector<Type> array;

		unsigned itemSize;

		sole::uuid uuid = sole::uuid1();

		unsigned size;

		Usage usage;

		UpdateRange updateRange;

		unsigned version;

		bool normalized;

		unsigned count;

		void setNeedsUpdate(bool value) {
			version++;
			_needsUpdate = value;
		}

		bool needsUpdate() {
			return _needsUpdate;
		}

		Signal<void(const BufferAttribute&)> onUpload;
		
		BufferAttribute& setUsage(Usage usage)
		{
			this->usage = usage;
			return *this;
		}

		BufferAttribute& copy(const BufferAttribute& source);
		
		BufferAttribute* clone();

		BufferAttribute& copyAt(unsigned index1, BufferAttribute& attribute, unsigned  index2);

		BufferAttribute& copyArray(Type* array, unsigned length);

		BufferAttribute& copyVector2sArray(const std::vector<Vector2>& vectors);

		BufferAttribute& copyVector3sArray(const std::vector<Vector3>& vectors);

		BufferAttribute& copyColor3sArray(const std::vector<Color>& colors);
		
		BufferAttribute& copyVector4sArray(const std::vector<Vector4> vectors);
		
		BufferAttribute& applyMatrix3(const Matrix3& m);

		BufferAttribute& applyMatrix4(const Matrix4& m);

		BufferAttribute& applyNormalMatrix(const Matrix3& m);

		BufferAttribute& transformDirection(const Matrix4& m);

		BufferAttribute& set(Type* value, unsigned length, unsigned offset = 0);
			
		Type getX(unsigned index) const;

		BufferAttribute& setX(unsigned index, Type x);

		Type getY(unsigned index) const;

		BufferAttribute& setY(unsigned index, Type y);

		Type getZ(unsigned index) const;

		BufferAttribute& setZ(unsigned index, Type z);

		Type getW(unsigned index) const;

		BufferAttribute& setW(unsigned index, Type w);
		

		BufferAttribute& setXY(unsigned index, Type x, Type y);

		BufferAttribute& setXYZ(unsigned index, Type x, Type y, Type z);

		BufferAttribute& setXYZW(unsigned index, Type x, Type y, Type z, Type w);

		void convertBufferAttributeToNonIndexed(const BufferAttribute& attribute, const std::vector<unsigned>& indices);
	};
	template<typename Type>
	inline BufferAttribute<Type>& BufferAttribute<Type>::copy(const BufferAttribute& source)
	{
		this->name = source.name;

		if (source.array.size() > 0) {
			if (array.size() != source.array.size())
				array.resize(source.array.size());
			std::copy(source.array.begin(), source.array.end(), this->array.begin());
		}
		else {
			array.clear();
		}

		this->itemSize = source.itemSize;

		this->size = source.size;

		this->count = source.count;

		this->normalized = source.normalized;

		this->usage = source.usage;

		return *this;
		
	}
	template<typename Type>
	inline BufferAttribute<Type>* BufferAttribute<Type>::clone()
	{
		return new BufferAttribute<Type>(*this);
	}
	template<typename Type>
	inline BufferAttribute<Type>& BufferAttribute<Type>::copyAt(unsigned index1, BufferAttribute& attribute, unsigned index2)
	{
		index1 *= this->itemSize;
		index2 *= attribute.itemSize;

		for (unsigned i = 0; i < this->itemSize; i++) {

			this->array[index1 + i] = attribute.array[index2 + i];

		}

		return *this;
	}
	template<typename Type>
	inline BufferAttribute<Type>& BufferAttribute<Type>::copyArray(Type* array, unsigned length)
	{
		std::memcpy(&this->array[0], array, sizeof(Type) * length);

		return *this;
	}
	
	template<typename Type>
	inline BufferAttribute<Type>& BufferAttribute<Type>::copyVector2sArray(const std::vector<Vector2>& vectors)
	{
		//Type* array = this->array;
		/*if (this->array.size() == 0 || this->array.size() != length)
			this->array.resize(length);*/
		unsigned offset = 0;

		for (unsigned i = 0; i < vectors.size(); i++) {

			auto vector = vectors[i];


			array[offset++] = vector.x;
			array[offset++] = vector.y;

		}
		return *this;
	}
	template<typename Type>
	inline BufferAttribute<Type>& BufferAttribute<Type>::copyVector3sArray(const std::vector<Vector3>& vectors)
	{
		//Type* array = this->array;
		//if (this->array.size() == 0 || this->array.size() != length)
		//	this->array.resize(length);
		unsigned offset = 0;

		for (unsigned i = 0; i < vectors.size(); i++) {

			auto vector = vectors[i];


			array[offset++] = vector.x;
			array[offset++] = vector.y;
			array[offset++] = vector.z;

		}
		return *this;
	}

	template<typename Type>
	inline BufferAttribute<Type>& BufferAttribute<Type>::copyColor3sArray(const std::vector<Color>& colors)
	{
		//Type* array = this->array;
		/*if (this->array.size() == 0 || this->array.size() != length)
			this->array.resize(length);*/
		unsigned offset = 0;

		for (unsigned i = 0; i < colors.size(); i++) {

			auto color = colors[i];


			array[offset++] = color.r;
			array[offset++] = color.g;
			array[offset++] = color.b;

		}
		return *this;
	}
	
	template<typename Type>
	inline BufferAttribute<Type>& BufferAttribute<Type>::copyVector4sArray(std::vector<Vector4> vectors)
	{
		//Type* array = this->array;
		/*if (this->array.size() == 0 || this->array.size() != length)
			this->array.resize(length);*/
		unsigned offset = 0;

		for (unsigned i = 0; i < vectors.size(); i++) {

			auto vector = vectors[i];


			array[offset++] = vector.x;
			array[offset++] = vector.y;
			array[offset++] = vector.z;
			array[offset++] = vector.w;

		}
		return *this;
	}
	template<typename Type>
	inline BufferAttribute<Type>& BufferAttribute<Type>::applyMatrix3(const Matrix3& m)
	{
		Vector3 _vector;
		for (int i = 0; i < this->count; i++) {

			_vector.x = this->getX(i);
			_vector.y = this->getY(i);
			_vector.z = this->getZ(i);

			_vector.applyMatrix3(m);

			this->setXYZ(i, _vector.x, _vector.y, _vector.z);

		}

		return *this;
	}
	template<typename Type>
	inline BufferAttribute<Type>& BufferAttribute<Type>::applyMatrix4(const Matrix4& m)
	{
		Vector3 _vector;
		for (int i = 0; i < this->count; i++) {

			_vector.x = this->getX(i);
			_vector.y = this->getY(i);
			_vector.z = this->getZ(i);

			_vector.applyMatrix4(m);

			this->setXYZ(i, _vector.x, _vector.y, _vector.z);

		}

		return *this;
	}
	template<typename type>
	inline BufferAttribute<type>& BufferAttribute<type>::applyNormalMatrix(const Matrix3& m)
	{
		Vector3 _vector;
		for (int i = 0; i < this->count; i++) {

			_vector.x = this->getX(i);
			_vector.y = this->getY(i);
			_vector.z = this->getZ(i);

			_vector.applyNormalMatrix(m);

			this->setXYZ(i, _vector.x, _vector.y, _vector.z);

		}

		return *this;
	}
	template<typename Type>
	inline BufferAttribute<Type>& BufferAttribute<Type>::transformDirection(const Matrix4& m)
	{
		Vector3 _vector;
		for (int i = 0; i < this->count; i++) {

			_vector.x = this->getX(i);
			_vector.y = this->getY(i);
			_vector.z = this->getZ(i);

			_vector.transformDirection(m);

			this->setXYZ(i, _vector.x, _vector.y, _vector.z);

		}

		return *this;
	}
	template<typename Type>
	inline BufferAttribute<Type>& BufferAttribute<Type>::set(Type* value, unsigned length, unsigned offset)
	{
		/*if (this->array.size() == 0 || this->array.size() != length)
			this->array.resize(length);*/

		std::memcpy(&this->array[0]+offset, value, sizeof(Type) * length);



		return *this;
	}
	template<typename Type>
	inline Type BufferAttribute<Type>::getX(unsigned index) const
	{
		return this->array[index * this->itemSize];
	}
	template<typename Type>
	inline BufferAttribute<Type>& BufferAttribute<Type>::setX(unsigned index, Type x)
	{
		this->array[index * this->itemSize] = x;
		return *this;
	}
	template<typename Type>
	inline Type BufferAttribute<Type>::getY(unsigned index) const
	{
		return this->array[index * this->itemSize + 1];
	}
	template<typename Type>
	inline BufferAttribute<Type>& BufferAttribute<Type>::setY(unsigned index, Type y)
	{
		this->array[index * this->itemSize + 1] = y;
		return *this;
	}
	template<typename Type>
	inline Type BufferAttribute<Type>::getZ(unsigned index) const
	{
		return this->array[index * this->itemSize + 2];
	}
	template<typename Type>
	inline BufferAttribute<Type>& BufferAttribute<Type>::setZ(unsigned index, Type z)
	{
		this->array[index * this->itemSize + 2] = z;
		return *this;
	}
	template<typename Type>
	inline Type BufferAttribute<Type>::getW(unsigned index) const
	{
		return this->array[index * this->itemSize + 3];
	}
	template<typename Type>
	inline BufferAttribute<Type>& BufferAttribute<Type>::setW(unsigned index, Type w)
	{
		this->array[index * this->itemSize + 3] = w;
		return *this;
	}
	template<typename Type>
	inline BufferAttribute<Type>& BufferAttribute<Type>::setXY(unsigned index, Type x, Type y)
	{
		index *= this->itemSize;

		this->array[index + 0] = x;
		this->array[index + 1] = y;

		return *this;
	}
	template<typename Type>
	inline BufferAttribute<Type>& BufferAttribute<Type>::setXYZ(unsigned index, Type x, Type y, Type z)
	{
		index *= this->itemSize;

		this->array[index + 0] = x;
		this->array[index + 1] = y;
		this->array[index + 2] = z;

		return *this;
	}
	template<typename Type>
	inline BufferAttribute<Type>& BufferAttribute<Type>::setXYZW(unsigned index, Type x, Type y, Type z, Type w)
	{
		index *= this->itemSize;

		this->array[index + 0] = x;
		this->array[index + 1] = y;
		this->array[index + 2] = z;
		this->array[index + 3] = w;

		return *this;
	}
	template<typename Type>
	inline void BufferAttribute<Type>::convertBufferAttributeToNonIndexed(const BufferAttribute<Type>& attribute, const std::vector<unsigned>& indices)
	{
		auto array = attribute.array;
		auto itemSize = attribute.itemSize;

		std::vector<float> array2(indices.size() * itemSize, 0.0f);
		//float[] array2 = new float[indices.Length * itemSize];

		int index = 0;
		int index2 = 0;

		for (unsigned int i = 0; i < indices.size(); i++)
		{
			index = indices[i] * itemSize;
			for (unsigned int j = 0; j < itemSize; j++)
			{
				array2[index2++] = array[index++];
			}
		}
		this->size = array2.size();
		this->itemSize = attribute.itemSize;
		this->count = this->size / this->itemSize;
		
		this->array.clear();
		for (unsigned i = 0; i < array2.size(); i++)
			this->array.push_back(array2[i]);

		//std::copy(array2.begin(), array2.end(), this->array.begin());
	}
}
#endif
