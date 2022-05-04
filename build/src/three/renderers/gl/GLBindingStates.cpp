#include "GLBindingStates.h"
#include <three/core/BufferGeometry.h>
#include <three/objects/InstancedMesh.h>
#include <three/materials/ShaderMaterial.h>
#include <three/core/InstancedInterleavedBuffer.h>
#include <three/core/InstancedBufferGeometry.h>
#include <three/core/InstancedBufferAttribute.h>
namespace three {

	namespace gl {
		void GLBindingStates::createVertexArrayObject(GLuint* vao)
		{
			glGenVertexArrays(1, vao);
		}

		void GLBindingStates::bindVertexArrayObject(GLuint vao)
		{
			glBindVertexArray(vao);
		}

		void GLBindingStates::deleteVertexArrayObject(GLuint* vao)
		{
			glDeleteVertexArrays(1, vao);
		}

		BindingStateStruct& GLBindingStates::getBindingState(const Geometry::ptr& geometry, const GLProgram::ptr& program, const Material::ptr& material)
		{
			bool wireframe = material->wireframe;			

			if (bindingStates.count(geometry->id) == 0) {
				
				bindingStates.insert({ geometry->id,ProgramStruct()});
			}
			ProgramStruct& programMap = bindingStates[geometry->id];
			
			if (programMap.count(program->id) == 0) {
				programMap.insert({ program->id,StateStruct() });
			}

			StateStruct& stateMap = programMap[program->id];

			if (stateMap.count(wireframe) == 0) {
				GLuint vao;
				createVertexArrayObject(&vao);
				BindingStateStruct state = createBindingState(vao);
				stateMap.insert({ wireframe,state });
			}

			return stateMap[wireframe];

		}

		BindingStateStruct GLBindingStates::createBindingState(GLuint vao)
		{
			vector<GLuint> newAttributes = vector<GLuint>(maxVertexAttributes,0);
			vector<GLuint> enabledAttributes = vector<GLuint>(maxVertexAttributes, 0);
			vector<GLuint> attributeDivisors= vector<GLuint>(maxVertexAttributes, 0);

			BindingStateStruct state;

			state.uuid = sole::uuid1();
			state.newAttributes = newAttributes;
			state.enabledAttributes = enabledAttributes;
			state.attributeDivisors = attributeDivisors;
			state.vao = vao;			

			return state;			

		}

		bool GLBindingStates::needsUpdate(const BufferGeometry::ptr& bufferGeometry, const BufferAttribute<unsigned>::ptr index)
		{
			std::unordered_map<AttributeNameKey, CacheData> cachedAttributes = currentState.attributes;

			for(auto items : bufferGeometry->attributes) {

				if (cachedAttributes.count(items.first) == 0) return true;

				auto cachedAttribute = cachedAttributes[items.first];
				auto geometryAttribute = bufferGeometry->attributes[items.first];

				if (cachedAttribute.attribute != geometryAttribute) return true;

				if (instanceOf<InterleavedBufferAttribute<float>>(geometryAttribute.get())) {
					auto interAttr = dynamic_pointer_cast<InterleavedBufferAttribute<float>>(geometryAttribute);
					if (cachedAttribute.data != interAttr->data) return true;
				}
			}

			if (currentState.index != index) return true;
			
			return false;
		}

		void GLBindingStates::saveCache(const BufferGeometry::ptr& bufferGeometry, const BufferAttribute<unsigned>::ptr& index)
		{

			std::unordered_map<AttributeNameKey, CacheData> cache;
			for (auto item : bufferGeometry->attributes) {
				auto attribute = item.second;
				CacheData data;
				data.attribute = attribute;
				if (instanceOf<InterleavedBufferAttribute<float>>(attribute.get())) {
					auto interAttr = dynamic_pointer_cast<InterleavedBufferAttribute<float>>(attribute);
					data.data = dynamic_pointer_cast<InterleavedBuffer<float>>(interAttr->data);
				}				
				cache.insert({ item.first,data });
			}
			currentState.attributes = cache;
			currentState.index = index;
		}
		void GLBindingStates::enableAttributeAndDivisor(GLuint attribute, GLuint meshPerAttribute)
		{
			currentState.newAttributes[attribute] = 1;

			if (currentState.enabledAttributes[attribute] == 0) {
				glEnableVertexAttribArray(attribute);
				currentState.enabledAttributes[attribute] = 1;
			}
			if (currentState.attributeDivisors[attribute] != meshPerAttribute) {
				glVertexAttribDivisor(attribute, meshPerAttribute);
				currentState.attributeDivisors[attribute] = meshPerAttribute;
			}
		}
		void GLBindingStates::vertexAttribPointer(GLuint index, GLint size, GLenum type, bool normalized, GLuint stride, GLuint offset)
		{
			if (isGL2 && (type == GL_INT || type == GL_UNSIGNED_INT)) {
				glVertexAttribIPointer(index, size, type, stride, (const void*)0);
			}
			else {
				glVertexAttribPointer(index, size, type, normalized, stride, (void*)offset);
			}
		}
		void GLBindingStates::setupVertexAttributes(const Object3D::ptr& object, const Material::ptr& material, const GLProgram::ptr& program, const BufferGeometry::ptr& bufferGeometry)
		{
			if (isGL2 == false && (object->type == "InstancedMesh" || bufferGeometry->type == "InstancedBufferGeometry")) {
				if (extensions->get(Extension::ARB_instanced_arrays) == false) return;

			}

			initAttributes();


			auto geometryAttributes = bufferGeometry->attributes;

			auto programAttributes = program->getAttributes();

			std::unordered_map<std::string, std::vector<float>> materialDefaultAttributeValues;

			if (material->type == "ShaderMaterial") {
				ShaderMaterial::ptr shaderMaterial = std::dynamic_pointer_cast<ShaderMaterial>(material);
				materialDefaultAttributeValues = shaderMaterial->defaultAttributeValues;
			}

			for (auto& it : programAttributes) {
				std::string nameKey = it.first;
				int programAttribute = programAttributes[nameKey];
				if (programAttribute >= 0) {

					BufferAttribute<float>::ptr geometryAttribute = bufferGeometry->getAttribute(nameKey);
					//std::any_cast<std::shared_ptr<BufferAttribute<float>>>(geometryAttributes[nameKey]);
					if (geometryAttribute != nullptr) {


						auto normalized = geometryAttribute->normalized;
						auto size = geometryAttribute->itemSize;

						Attribute attribute = attributes->get<float>(*geometryAttribute);

						// TODO Attribute may not be available on context restore

						if (attribute.isEmpty) continue;

						auto buffer = attribute.buffer;
						auto type = attribute.type;
						auto bytesPerElement = attribute.bytesPerElement;

						if (instanceOf<InterleavedBufferAttribute<float>>(geometryAttribute.get())) {
							InterleavedBufferAttribute<float>::ptr interleavedBufferAttribute = std::dynamic_pointer_cast<InterleavedBufferAttribute<float>>(geometryAttribute);
							InterleavedBuffer<float>::ptr data = interleavedBufferAttribute->data;
							auto stride = data->stride;
							auto offset = interleavedBufferAttribute->offset;

							if (data->_array.size() > 0 && instanceOf<InstancedInterleavedBuffer<float>>(data.get())) {

								InstancedInterleavedBuffer<float>::ptr instancedBuffer = std::dynamic_pointer_cast<InstancedInterleavedBuffer<float>>(data);

								enableAttributeAndDivisor(programAttribute, instancedBuffer->meshPerAttribute);
								InstancedBufferGeometry::ptr instancedBufferGeometry = std::dynamic_pointer_cast<InstancedBufferGeometry>(bufferGeometry);
								if (instancedBufferGeometry->maxInstancedCount == -1) {

									instancedBufferGeometry->maxInstancedCount = instancedBuffer->meshPerAttribute * instancedBuffer->count;
								}
							}
							else {
								enableAttribute(programAttribute);
							}

							glBindBuffer(GL_ARRAY_BUFFER, buffer);
							vertexAttribPointer(programAttribute, size, type, normalized, stride * bytesPerElement, offset * bytesPerElement);

						}
						else {

							if (instanceOf<InstancedBufferAttribute<float>>(geometryAttribute.get())) {

								InstancedBufferAttribute<float>::ptr instanceAttribute = std::dynamic_pointer_cast<InstancedBufferAttribute<float>>(geometryAttribute);
								enableAttributeAndDivisor(programAttribute, instanceAttribute->meshPerAttribute);

								InstancedBufferGeometry::ptr instancedBufferGeometry = std::dynamic_pointer_cast<InstancedBufferGeometry>(bufferGeometry);
								if (instancedBufferGeometry->maxInstancedCount == -1) {

									instancedBufferGeometry->maxInstancedCount = instanceAttribute->meshPerAttribute * instanceAttribute->count;

								}
							}
							else {

								enableAttribute(programAttribute);

							}

							glBindBuffer(GL_ARRAY_BUFFER, buffer);

							vertexAttribPointer(programAttribute, size, type, normalized, 0, 0);
							//glVertexAttribPointer(programAttribute, size, type, normalized, 0, (void*)0);

						}

					}
					else if (nameKey == "instanceMatrix") {

						InstancedMesh::ptr instancedMesh = std::dynamic_pointer_cast<InstancedMesh>(object);
						Attribute attribute = attributes->get<float>(instancedMesh->instanceMatrix);

						// TODO Attribute may not be available on context restore

						if (attribute.isEmpty) continue;

						auto buffer = attribute.buffer;
						auto type = attribute.type;

						enableAttributeAndDivisor(programAttribute + 0, 1);
						enableAttributeAndDivisor(programAttribute + 1, 1);
						enableAttributeAndDivisor(programAttribute + 2, 1);
						enableAttributeAndDivisor(programAttribute + 3, 1);

						glBindBuffer(GL_ARRAY_BUFFER, buffer);

						glVertexAttribPointer(programAttribute + 0, 4, type, false, 64, (const void*)0);
						glVertexAttribPointer(programAttribute + 1, 4, type, false, 64, (const void*)16);
						glVertexAttribPointer(programAttribute + 2, 4, type, false, 64, (const void*)32);
						glVertexAttribPointer(programAttribute + 3, 4, type, false, 64, (const void*)48);
					}
					else if (nameKey == "instanceColor") {
						Attribute attribute = attributes->get<float>(*geometryAttribute);

						if (attribute.isEmpty) continue;

						auto buffer = attribute.buffer;
						auto type = attribute.type;

						enableAttributeAndDivisor(programAttribute, 1);
						glBindBuffer(GL_ARRAY_BUFFER, buffer);
						glVertexAttribPointer(programAttribute, 3, type, false, 12, (void*)0);
					}
					else if (materialDefaultAttributeValues.size() > 0) {

						std::vector<float> value = materialDefaultAttributeValues[nameKey];

						if (value.size() > 0) {

							switch (value.size()) {

							case 2:
								glVertexAttrib2fv(programAttribute, &value[0]);
								break;

							case 3:
								glVertexAttrib3fv(programAttribute, &value[0]);
								break;

							case 4:
								glVertexAttrib4fv(programAttribute, &value[0]);
								break;

							default:
								glVertexAttrib1fv(programAttribute, &value[0]);
							}
						}
					}
				}
			}

			disableUnusedAttributes();

		}
		GLBindingStates::GLBindingStates(const GLExtensions::ptr& extensions, const GLAttributes::ptr& attributes, const GLCapabilities::ptr& capabilities) : extensions(extensions), attributes(attributes), capabilities(capabilities)
		{
			glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttributes);
			vaoAvailable = capabilities->isGL2;

			defaultState.geometry = -1;
			defaultState.program = -1;
			defaultState.wireframe = false;
			defaultState.vao = 0;

		}
		GLBindingStates::~GLBindingStates()
		{
			reset();
			for (auto iter = bindingStates.begin(); iter != bindingStates.end();) {
				ProgramStruct& programMap = iter->second;
				for (auto programId = programMap.begin(); programId != programMap.end();) {
					StateStruct& stateMap = programId->second;
					for (auto stateId = stateMap.begin(); stateId != stateMap.end();) {
						BindingStateStruct& binding = stateId->second;
						deleteVertexArrayObject(&binding.vao);
						stateMap.erase(stateId);
						stateId++;
					}
					programMap.erase(programId);
					programId++;
				}
				bindingStates.erase(iter);
				iter++;
			}
		}
		void GLBindingStates::initAttributes()
		{
			if (currentState.newAttributes.size() == 0) {
				currentState.newAttributes = vector<GLuint>(maxVertexAttributes, 0);
				currentState.enabledAttributes = vector<GLuint>(maxVertexAttributes, 0);
				currentState.attributeDivisors = vector<GLuint>(maxVertexAttributes, 0);

			}
			for (int i = 0; i < currentState.newAttributes.size(); i++) {
				currentState.newAttributes[i] = 0;
			}		
		}
		void GLBindingStates::setUp(const Object3D::ptr& object, const Material::ptr& material, const GLProgram::ptr& program, const BufferGeometry::ptr& geometry, const BufferAttribute<unsigned>::ptr& index)
		{
			bool updateBuffers = false;

			if (vaoAvailable) {
				BindingStateStruct state = getBindingState(geometry, program, material);
				if (!currentState.equals(state)) {
					currentState = state;
					bindVertexArrayObject(currentState.vao);
				}
				
				updateBuffers = needsUpdate(geometry, index);

				if (updateBuffers) saveCache(geometry, index);

			}
			else {
				bool wireframe = material->wireframe;

				if (currentState.geometry != geometry->id ||
					currentState.program != program->id ||
					currentState.wireframe != wireframe) {
					currentState.geometry = geometry->id;
					currentState.program = program->id;
					currentState.wireframe = wireframe;

					updateBuffers = true;
				}

			}

			if(instanceOf<InstancedMesh>(object.get())) {
				updateBuffers = true;
			}

			if (index != nullptr) {
				attributes->update<unsigned>(*index, GL_ELEMENT_ARRAY_BUFFER);
			}

			if (updateBuffers) {
				setupVertexAttributes(object, material, program, geometry);

				if (index != nullptr) {
					auto attribute = attributes->get<unsigned>(*index);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, attribute.buffer);
				}
			}

		}
		void GLBindingStates::reset()
		{
			resetDefaultState();

			if (currentState.equals(defaultState)) return;

			currentState = defaultState;

			bindVertexArrayObject(currentState.vao);
		}

		void GLBindingStates::resetDefaultState()
		{
			defaultState.geometry = -1;
			defaultState.program = -1;
			defaultState.wireframe = false;
			defaultState.vao = 0;
		}

		void GLBindingStates::releaseStatesOfGeometry(const Geometry::ptr& geometry)
		{
			if (bindingStates.count(geometry->id) == 0) return;

			ProgramStruct& programMap = bindingStates[geometry->id];

			for (auto programId = programMap.begin(); programId != programMap.end();) {
				StateStruct& stateMap = programId->second;
				for (auto& boolId = stateMap.begin(); boolId != stateMap.end();) {
					BindingStateStruct& binding = boolId->second;
					deleteVertexArrayObject(&binding.vao);
					stateMap.erase(boolId);
					boolId++;
				}
				programMap.erase(programId);
				programId++;
			}
			bindingStates.erase(geometry->id);
		}

		void GLBindingStates::releaseStatesOfProgram(const GLProgram::ptr& program)
		{
			for (auto iter = bindingStates.begin(); iter != bindingStates.end();) {
				ProgramStruct& programMap = iter->second;
				if (programMap.count(program->id) == 0) continue;

				StateStruct& stateMap = programMap[program->id];
				
				for (auto boolId = stateMap.begin(); boolId != stateMap.end();) {
					BindingStateStruct& binding = boolId->second;
					deleteVertexArrayObject(&binding.vao);
					stateMap.erase(boolId);
					boolId++;
				}
				programMap.erase(program->id);
				iter++;
			}
		}

		void GLBindingStates::enableAttribute(GLuint attribute)
		{
			enableAttributeAndDivisor(attribute, 0);
		}

		void GLBindingStates::disableUnusedAttributes()
		{
			for (int i = 0; i < currentState.enabledAttributes.size(); i++) {
				if (currentState.enabledAttributes[i] != currentState.newAttributes[i]) {
					glDisableVertexAttribArray(i);
					currentState.enabledAttributes[i] = 0;
				}
			}
		}
	}
}
