#ifndef GL_ATTRIBUTES_THREE_H
#define GL_ATTRIBUTES_THREE_H
#include <three/Constants.h>
#include <three/core/Attribute.h>
#include <three/core/BufferAttribute.h>
#include <three/core/InterleavedBufferAttribute.h>
namespace three {
	namespace gl {
		class GLAttributes {
			bool isGL2 = false;
			
			std::unordered_map<sole::uuid, Attribute> buffers;
		public:
			using ptr = std::shared_ptr<GLAttributes>;

			GLAttributes(bool isGL2=false):isGL2(isGL2) 
			{
				
			}
			~GLAttributes() {			
				for (auto attr = buffers.begin();attr != buffers.end();) {
					GLuint buffer = attr->second.buffer;
					glDeleteBuffers(1, &buffer);
					attr++;
				}
			}			

			template<typename T>
			void createBuffer(Attribute& attributeBuffer,BufferAttribute<T>& attribute, unsigned bufferType)
			{
				T* array;
				bool isInterleavedBuffer = false;
				InterleavedBufferAttribute<T>* interleaved;

				if (instanceOf<InterleavedBufferAttribute<T>>(&attribute)) {					
					interleaved = dynamic_cast<InterleavedBufferAttribute<T> *>(&attribute);
					array = &(interleaved->data->_array[0]);
					isInterleavedBuffer = true;
				}
				else {
					array = &(attribute.array[0]);
				}
				Usage usage = attribute.usage;

				GLuint buffer;
				int bytePerElement = sizeof(T);
				GLsizeiptr ptr;
				glGenBuffers(1, &buffer);
				glBindBuffer(bufferType, buffer);
				
				unsigned type = (unsigned)BufferUsageHint::Float;

				if (typeid(float)==typeid(T)) {
					if (!isInterleavedBuffer) {
						glBufferData(bufferType, attribute.size * sizeof(float), &(attribute.array[0]), (GLenum)usage);
					}
					else {
						glBufferData(bufferType, interleaved->data->_array.size() * sizeof(float), array, (GLenum)usage);
					}
					type = (unsigned)BufferUsageHint::Float;

				}
				else if (typeid(UINT16) == typeid(T)) {
					glBufferData(bufferType, attribute.size * sizeof(UINT16), array, (GLenum)usage);
					type = (unsigned)BufferUsageHint::UnsignedShort;
				}
				else if (typeid(INT16) == typeid(T)) {
					glBufferData(bufferType, attribute.size * sizeof(INT16), array, (GLenum)usage);
					type = (unsigned)BufferUsageHint::Short;
				}
				else if (typeid(UINT32) == typeid(T)) {
					glBufferData(bufferType, attribute.size * sizeof(UINT32), array, (GLenum)usage);
					type = (unsigned)BufferUsageHint::UnsignedInt;
				}
				else if (typeid(INT32)==typeid(T)) {
					glBufferData(bufferType, attribute.size * sizeof(INT32), array, (GLenum)usage);
					type = (unsigned)BufferUsageHint::Int;
				}
				else if (typeid(INT8)== typeid(T)) {
					glBufferData(bufferType, attribute.size * sizeof(INT8), array, (GLenum)usage);
					type = (unsigned)BufferUsageHint::Byte;
				}
				else if (typeid(UINT8)==typeid(T)) {
					glBufferData(bufferType, attribute.size * sizeof(UINT8), array, (GLenum)usage);
					type = (unsigned)BufferUsageHint::UnsignedByte;
				}

				attributeBuffer.set(buffer, type, bytePerElement, attribute.version, false);
			}
			
			template<typename T>
			void updateBuffer(unsigned buffer, BufferAttribute<T>& attribute, unsigned bufferType)
			{
				T* array;

				if (instanceOf<InterleavedBufferAttribute<T>>(&attribute)) {
					InterleavedBufferAttribute<T>* interleaved;
					interleaved = dynamic_cast<InterleavedBufferAttribute<T>*>(&attribute);
					array = &(interleaved->data->_array[0]);
				}
				else {
					array = &(attribute.array[0]);
				}

				UpdateRange updateRange = attribute.updateRange;

				glBindBuffer(bufferType, buffer);
							

				if (updateRange.count == -1) {

					glBufferSubData(bufferType, 0, attribute.size * sizeof(T), array);

				}
				else {

					glBufferSubData(bufferType, updateRange.start * sizeof(T), updateRange.count * sizeof(T), &array[updateRange.start]);

					attribute.updateRange = -1;
				}


			}

			template<typename T>
			Attribute& get(const BufferAttribute<T>& attribute)
			{				
				return buffers[attribute.uuid];
			}

			//void remove(std::string attribute);

			template<typename T>
			void update(BufferAttribute<T>& attribute, unsigned bufferType)
			{
				Attribute data;

				auto found = buffers.find(attribute.uuid);

				if (found == buffers.end()) {
					createBuffer(buffers[attribute.uuid], attribute, bufferType);
				}
				else {
					Attribute& buffer = found->second;
					if (buffer.version < attribute.version) {
						updateBuffer<T>(buffer.buffer, attribute, bufferType);
						buffer.version = attribute.version;
					}
				}				
			}

			template<typename T>
			void remove(BufferAttribute<T>& attribute) {
				if (buffers.find(attribute.uuid) != buffers.end()) {
					const Attribute& data = buffers[attribute.uuid];
					glDeleteBuffers(1, &data.buffer);
					buffers.erase(attribute.uuid);
				}
			}
		};
	}
}
#endif