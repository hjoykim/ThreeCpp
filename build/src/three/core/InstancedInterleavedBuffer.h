#ifndef INSTANCEDINTERLEAVEDBUFFER_THREE_H
#define INSTANCEDINTERLEAVEDBUFFER_THREE_H
#include <three/core/InterleavedBuffer.h>
namespace three {
	template<typename Type>
	class InstancedInterleavedBuffer : public InterleavedBuffer<Type> {
	public :
		using ptr = std::shared_ptr<InstancedInterleavedBuffer>;
		int meshPerAttribute = 1;

		InstancedInterleavedBuffer() : InterleavedBuffer(), meshPerAttribute(1) {
		}
		InstancedInterleavedBuffer(const std::vector<Type>& array, unsigned stride,int meshPerAttribute=1) : InterleavedBuffer(array,stride) {
		}
		InstancedInterleavedBuffer(const InstancedInterleavedBuffer& source) : InterleavedBuffer(source) {
			meshPerAttribute = source.meshPerAttribute;
		}
		InstancedInterleavedBuffer* clone() {
			return new InstancedInterleavedBuffer(*this);
		}
		InstancedInterleavedBuffer& copy(const InstancedInterleavedBuffer& source) {
			InterleavedBuffer::copy(source);
			meshPerAttribute = source.meshPerAttribute;
			return *this;
		}
	};
}
#endif