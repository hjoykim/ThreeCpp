#ifndef INTERLEAVED_BUFFERATTRIBUTE_THREE_H
#define INTERLEAVED_BUFFERATTRIBUTE_THREE_H
#include <three/core/BufferAttribute.h>
#include <three/core/InterleavedBuffer.h>

namespace three {
	namespace math {
		class Matrix4;
	}
	using namespace math;

	template<typename Type>
	class InterleavedBufferAttribute : public BufferAttribute<Type> {
	private : 
		Vector3 _vector;
	public:

		using ptr = std::shared_ptr<InterleavedBufferAttribute>;
		unsigned offset;
		
		unsigned stride;
				
		std::shared_ptr<InterleavedBuffer<Type>> data;
		InterleavedBufferAttribute() :BufferAttribute<Type>(), offset(0), stride(0){}

		InterleavedBufferAttribute(const std::shared_ptr<InterleavedBuffer<Type>>& interleavedBuffer, unsigned itemSize, unsigned offset, bool normalized) :BufferAttribute<Type>(),offset(offset),stride(0) {
			data = interleavedBuffer;
			this->itemSize = itemSize;
			this->normalized = normalized = true;
			this->count = interleavedBuffer->count;
		}
		InterleavedBufferAttribute(const std::shared_ptr<InterleavedBufferAttribute<Type>>& source) : BufferAttribute<Type>(source) {
			data = std::make_shared<InterleavedBuffer<Type>>(*source.data);
			offset = source.offset;
			stride = source.stride;

		}

		static ptr create() {
			return std::make_shared<InterleavedBufferAttribute>();
		}

		static ptr create(const std::shared_ptr<InterleavedBuffer<Type>>& interleavedBuffer, unsigned itemSize, unsigned offset, bool normalized) {
			return std::make_shared<InterleavedBufferAttribute>(interleavedBuffer, itemSize, offset, normalized);
		}

		InterleavedBufferAttribute& applyMatrix4(const Matrix4& m);

		Type getX(unsigned index) const;

		InterleavedBufferAttribute& setX(unsigned index, Type x);

		Type getY(unsigned index) const;

		InterleavedBufferAttribute& setY(unsigned index, Type y);

		Type getZ(unsigned index) const;

		InterleavedBufferAttribute& setZ(unsigned index, Type z);

		Type getW(unsigned index) const;

		InterleavedBufferAttribute& setW(unsigned index, Type w);


		InterleavedBufferAttribute& setXY(unsigned index, Type x, Type y);

		InterleavedBufferAttribute& setXYZ(unsigned index, Type x, Type y, Type z);

		InterleavedBufferAttribute& setXYZW(unsigned index, Type x, Type y, Type z, Type w);


	};
	
	
	template<typename Type>
	inline InterleavedBufferAttribute<Type>& InterleavedBufferAttribute<Type>::applyMatrix4(const Matrix4& m) {

		for (unsigned i = 0; i < this->data.count; i++) {

			_vector.x = getX(i);
			_vector.y = getY(i);
			_vector.z = getZ(i);

			_vector.applyMatrix4(m);

			setXYZ(i, _vector.x, _vector.y, _vector.z);

		}
		return *this;
	}
	template<typename Type>
	inline Type InterleavedBufferAttribute<Type>::getX(unsigned index) const
	{
		return data._array[index * data.stride + offset];
	}
	template<typename Type>
	inline InterleavedBufferAttribute<Type>& InterleavedBufferAttribute<Type>::setX(unsigned index, Type x)
	{
		data._array[index * data.stride + offset] = x;

		return *this;
	}
	template<typename Type>
	inline Type InterleavedBufferAttribute<Type>::getY(unsigned index) const
	{
		return data._array[index * data.stride + offset + 1];
	}
	template<typename Type>
	inline InterleavedBufferAttribute<Type>& InterleavedBufferAttribute<Type>::setY(unsigned index, Type y)
	{
		data._array[index * data.stride + offset + 1] = y;
	}
	template<typename Type>
	inline Type InterleavedBufferAttribute<Type>::getZ(unsigned index) const
	{
		return data._array[index * data.stride + offset + 2];
	}
	template<typename Type>
	inline InterleavedBufferAttribute<Type>& InterleavedBufferAttribute<Type>::setZ(unsigned index, Type z)
	{
		data._array[index * data.stride + offset + 2] = z;
	}
	template<typename Type>
	inline Type InterleavedBufferAttribute<Type>::getW(unsigned index) const
	{
		return data._array[index * data.stride + offset + 3];
	}
	template<typename Type>
	inline InterleavedBufferAttribute<Type>& InterleavedBufferAttribute<Type>::setW(unsigned index, Type w)
	{
		data._array[index * data.stride + offset + 3] = w;
	}
	template<typename Type>
	inline InterleavedBufferAttribute<Type>& InterleavedBufferAttribute<Type>::setXY(unsigned index, Type x, Type y)
	{
		index = index * data.stride + offset;
		data._array[index + 0] = x;
		data._array[index + 1] = y;

		return *this;
	}
	template<typename Type>
	inline InterleavedBufferAttribute<Type>& InterleavedBufferAttribute<Type>::setXYZ(unsigned index, Type x, Type y, Type z)
	{
		index = index * data.stride + offset;
		data._array[index + 0] = x;
		data._array[index + 1] = y;
		data._array[index + 2] = z;

		return *this;
	}
	template<typename Type>
	inline InterleavedBufferAttribute<Type>& InterleavedBufferAttribute<Type>::setXYZW(unsigned index, Type x, Type y, Type z, Type w)
	{
		index = index * data.stride + offset;
		data._array[index + 0] = x;
		data._array[index + 1] = y;
		data._array[index + 2] = z;
		data._array[index + 3] = w;

		return *this;
	}
}
#endif