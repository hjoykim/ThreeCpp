#ifndef INTERLEAVED_BUFFER_THREE_H
#define INTERLEAVED_BUFFER_THREE_H
#include <vector>
#include <three/Constants.h>
#include <three/core/BufferAttribute.h>
namespace three {

	template<typename Type>
	class InterleavedBuffer : public BufferAttribute<Type> {
	public:
		using ptr = std::shared_ptr<InterleavedBuffer>;
		std::vector<Type> _array;

		unsigned stride;

		InterleavedBuffer() : BufferAttribute(), stride(0) {
			this->usage = Usage::StaticDrawUsage;			
		}
		InterleavedBuffer(const InterleavedBuffer& source) : BufferAttribute(source) {			
			stride = source.stride;
		}
		InterleavedBuffer(const std::vector<Type>& array,unsigned stride) : _array(array),stride(stride){
			this->updateRange.start = 0;
			this->updateRange.count = -1;
			this->count = array.size() / stride;
		}

		static ptr create() {
			return std::make_shared<InterleavedBuffer>();
		}

		static ptr create(const std::vector<Type>& array, unsigned stride) {
			return std::make_shared<InterleavedBuffer>(array, stride);
		}
		InterleavedBuffer& copyAt(unsigned index1, const InterleavedBuffer& attribute, unsigned index2) {
			index1 *= stride;
			index2 *= attribute.stride;

			if (_array.size() < index1 + stride) {
				_array.resize(index1 + stride);
			}
			for (unsigned i = 0; i < stride; i++) {

				_array[index1 + i] = attribute._array[index2 + i];
			}
			return *this;
		}

		InterleavedBuffer& set(std::vector<Type> value, unsigned offset) {
			for (unsigned i = offset; i < value.size(); i++) {
				_array.push_back(value[i - offset]);
			}

			return *this;
		}

		InterleavedBuffer* clone() {
			return new InterleavedBuffer(*this);
		}

		InterleavedBuffer& copy(const InterleavedBuffer& source) {
			BufferAttribute::copy(source);
			if (source._array.size() > 0) {
				_array.reset(source._array.size());
				std::copy(source._array.begin(), source._array.end(), _array.begin());
			}
			stride = source = stride;
			return *this;
		}
	};
}
#endif