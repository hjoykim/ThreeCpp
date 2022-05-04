#ifndef GLBINDING_STATES_H
#define GLBINDING_STATES_H
#include <three/utils/sole.h>
#include <three/Constants.h>
#include <three/core/BufferGeometry.h>
#include <vector>
#include <three/renderers/gl/GLAttributes.h>
#include <three/renderers/gl/GLExtensions.h>
#include <three/renderers/gl/GLCapabilities.h>
#include <three/renderers/gl/GLProgram.h>
#include <three/materials/Material.h>
#include <three/core/InterleavedBufferAttribute.h>
namespace three {
	namespace gl {
		using namespace std;		
		
		struct CacheData {
			BufferAttribute<float>::ptr attribute;
			InterleavedBuffer<float>::ptr data;
		};
		struct BindingStateStruct {
			sole::uuid uuid;
			size_t geometry;
			size_t program;
			bool wireframe;
			vector<GLuint> newAttributes;
			vector<GLuint> enabledAttributes;
			vector<GLuint> attributeDivisors;
			GLuint vao;
			std::unordered_map<AttributeNameKey, CacheData> attributes;
			BufferAttribute<unsigned>::ptr index;

			bool equals(const BindingStateStruct& other) {
				return uuid == other.uuid;
			}
		};

		using StateStruct = unordered_map<size_t, BindingStateStruct>;
		
		using ProgramStruct = unordered_map<size_t, StateStruct>;

		
		class GLBindingStates {
		private:
			GLExtensions::ptr extensions;
			GLCapabilities::ptr capabilities;
			GLAttributes::ptr attributes;
			bool vaoAvailable = false;
			bool isGL2 = false;
			GLint maxVertexAttributes;

			void createVertexArrayObject(GLuint* vao);
			
			void bindVertexArrayObject(GLuint vao);
			
			void deleteVertexArrayObject(GLuint* vao);
			
			BindingStateStruct& getBindingState(const Geometry::ptr& geometry, const GLProgram::ptr& program, const Material::ptr& material);

			BindingStateStruct createBindingState(GLuint vao);

			bool needsUpdate(const BufferGeometry::ptr& bufferGeometry, const BufferAttribute<unsigned>::ptr index);

			void saveCache(const BufferGeometry::ptr& bufferGeometry, const BufferAttribute<unsigned>::ptr& index);

			void enableAttributeAndDivisor(GLuint attribute, GLuint meshPerAttribute);

			void vertexAttribPointer(GLuint index, GLint size, GLenum type, bool normalized, GLuint stride, GLuint offset);

			void setupVertexAttributes(const Object3D::ptr& object, const Material::ptr& material, const GLProgram::ptr& program, const BufferGeometry::ptr& bufferGeometry);

		public :

			using ptr = std::shared_ptr<GLBindingStates>;

			unordered_map<size_t, ProgramStruct> bindingStates;
			BindingStateStruct defaultState;
			BindingStateStruct currentState;

			GLBindingStates(const GLExtensions::ptr& extensions, const GLAttributes::ptr& attributes, const GLCapabilities::ptr& capabilities);
			
			static ptr create(const GLExtensions::ptr& extensions, const GLAttributes::ptr& attributes, const GLCapabilities::ptr& capabilities) {
				return std::make_shared<GLBindingStates>(extensions, attributes, capabilities);
			}
			virtual ~GLBindingStates();

			void initAttributes();

			void setUp(const Object3D::ptr& object, const Material::ptr& material, const GLProgram::ptr& program, const BufferGeometry::ptr& geometry, const BufferAttribute<unsigned>::ptr& index);

			void reset();

			void resetDefaultState();

			void releaseStatesOfGeometry(const Geometry::ptr& geometry);

			void releaseStatesOfProgram(const GLProgram::ptr& program);

			void enableAttribute(GLuint attribute);

			void disableUnusedAttributes();


		};	
	}
}
#endif