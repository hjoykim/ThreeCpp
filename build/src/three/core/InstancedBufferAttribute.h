#ifndef INSTANCED_BUFFER_ATTRIBUTE_THREE_H
#define INSTANCED_BUFFER_ATTRIBUTE_THREE_H
#include <three/core/BufferAttribute.h>
namespace three {
	template<typename T>
	class InstancedBufferAttribute : public BufferAttribute<T> {
	public :
		using ptr = std::shared_ptr<InstancedBufferAttribute>;
		
		int meshPerAttribute = 1;

		InstancedBufferAttribute() : BufferAttribute(), meshPerAttribute(1) {}

		InstancedBufferAttribute(std::vector<T> array, unsigned itemSize, bool normalized, int meshPerAttribute) : BufferAttribute(array, itemSize, normalized), meshPerAttribute(meshPerAttribute){}

		InstancedBufferAttribute(const InstancedBufferAttribute& source) : BufferAttribute(source) {
			meshPerAttribute = source.meshPerAttribute;
		}
		InstancedBufferAttribute* clone() {
			return new InstancedBufferAttribute(*this);
		}
		InstancedBufferAttribute* copy(const InstancedBufferAttribute& source) {
			BufferAttribute::copy(source);
			meshPerAttribute = source.meshPerAttribute;
			return *this;
		}
		
	};
}
#endif