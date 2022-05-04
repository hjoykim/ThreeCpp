#ifndef UNIFORMS_H
#define UNIFORMS_H
#include <vector>
#include <string>
#include <three/Constants.h>
#include <three/ThreeMath.h>
#include <utility>
#include <unordered_map>
#include <sstream>
#include <memory>
#include <array>
#include <three/core/Attribute.h>
#include <iostream>
#include <stdio.h>
#include <three/Types.h>
#include <three/textures/CubeTexture.h>
namespace three {
	namespace gl	{		
		/*namespace uniformname {
			static inline bool isRegistered(UniformName name) {
				return name >= UniformName::registered_name_1 && name <= UniformName::registered_name_16;
			}
			static inline UniformName registered(unsigned count) {
				unsigned regname = (unsigned)UniformName::registered_name_1 + count;
				if (regname > (unsigned)UniformName::registered_name_16)
					throw std::out_of_range("too many user-defined uniforms");
				return (UniformName)regname;
			}
			UniformName get(const std::string& name);
		}*/
		class GLTextures;
		class GLUniformContainer;
		class GLUniforms;
		class UniformValues;
		class UniformValue;
		class GLUniform 
		{
		public:
			using GLTexturesPtr = std::shared_ptr<GLTextures>;
		private:
			std::vector<GLuint> allocTextUnits(GLuint n);

		protected:

			GLTexturesPtr textures;

		public:	

			UniformClassType uniformClassType = UniformClassType::SingleUniform;
			std::string id;

			GLint addr;
			UniformType type;
			using ptr = std::shared_ptr<GLUniform>;

			GLUniform() : id(""), type(UniformType::None), addr(-1) {}

			GLUniform(const GLTexturesPtr& textures,std::string& id, UniformType type, const GLint addr) : textures(textures),id(id), type(type), addr(addr) {}
					
			
			static ptr create() {
				return std::make_shared<GLUniform>();
			}

			static ptr create(const GLTexturesPtr& textures,std::string& id, UniformType type, const GLint addr) {
				return std::make_shared<GLUniform>(textures,id, type, addr);
			}
			virtual ptr clone(std::string& id) {
				return GLUniform::create(this->textures,id, this->type, this->addr);
			}
						
			virtual ~GLUniform() = default;

			void setValue(bool b);

			void setValue(const std::vector<unsigned char>& b);

			void setValue(GLfloat f);

			void setValue(GLint i);

			void setValue(GLuint u);

			void setValue(const math::Color& c);

			void setValue(const math::Vector2& v);


			void setValue(const math::Vector3& v);

			void setValue(const math::Vector4& v);

			void setValue(const math::Matrix3& v);

			void setValue(const math::Matrix4& v);

			void setValue(const std::vector<float>& v);

			void setValue(const std::vector<GLuint>& v);

			void setValue(const Texture::ptr& t);

			void setValue(const std::vector<math::Vector2>& v);

			void setValue(const std::vector<math::Vector3>& v);

			void setValue(const std::vector<math::Vector4>& v);

			void setValue(const std::vector<math::Matrix3>& v);

			void setValue(const std::vector<math::Matrix4>& v);

			void setValue(const std::vector<Texture::ptr>& t);

			
			virtual GLUniform* asGLUniform() {
				return this;
			}
			virtual GLUniformContainer* asGLContainer() {
				return nullptr;
			}
		
		};
		class PureArrayUniform : public GLUniform
		{
		public:
			GLint index;
			using ptr = std::shared_ptr<PureArrayUniform>;

			PureArrayUniform(const GLUniform::GLTexturesPtr& textures, std::string& id, UniformType type, const GLint addr) : GLUniform(textures,id, type, addr), index(0) {
				uniformClassType = UniformClassType::PureArrayUniform;
			}

			static ptr create(const GLUniform::GLTexturesPtr& textures, std::string& id, UniformType type, const GLint addr)
			{
				return std::make_shared<PureArrayUniform>(textures,id, type, addr);
			}

			virtual GLUniform::ptr clone(std::string& id) override {
				return PureArrayUniform::create(this->textures,id, this->type, this->addr);
			}
		};

		class GLUniformContainer {
			friend class GLUniforms;
		protected:
			

			//std::unordered_map<std::string, std::string> nameRegistry;
		public :
			std::unordered_map<std::string, GLUniform::ptr,StringHash> seq;
			virtual ~GLUniformContainer() = default;

			/*UniformName registered(std::string name) {
				auto found = nameRegistry.find(name);
				if (found != nameRegistry.end()) return found->second;
				auto uname = uniformname::registered(nameRegistry.size());
				nameRegistry[name] = uname;
				return uname;
			}*/

			void add(GLUniform::ptr uniform) {
				seq[uniform->id] = uniform;
			}
			template <typename T>
			void setUniformValue(std::string& name, const T& value) {
				if (seq.count(name) > 0) {
					seq[name]->setValue(value);
				}
			}

			GLUniform::ptr get(const std::string& name) {
				return seq.count(name) ? seq[name] : nullptr;
			}
			/*GLUniform::ptr get(const size_t index) {
				switch (index) {
				case 0: return get(UniformName::registered_name_1);
				case 1: return get(UniformName::registered_name_2);
				case 2: return get(UniformName::registered_name_3);
				case 3: return get(UniformName::registered_name_4);
				case 4: return get(UniformName::registered_name_5);
				case 5: return get(UniformName::registered_name_6);
				case 6: return get(UniformName::registered_name_7);
				case 7: return get(UniformName::registered_name_8);
				case 8: return get(UniformName::registered_name_9);
				case 9: return get(UniformName::registered_name_10);
				case 10: return get(UniformName::registered_name_11);
				case 11: return get(UniformName::registered_name_12);
				case 12: return get(UniformName::registered_name_13);
				case 13: return get(UniformName::registered_name_14);
				case 14: return get(UniformName::registered_name_15);
				case 15: return get(UniformName::registered_name_16);
				default:
					throw std::logic_error("unsupported index");
				}
			}
			int getIndex(UniformName uniformName) {
				switch (uniformName) {
				case UniformName::registered_name_1: return 0;
				case UniformName::registered_name_2: return 1;
				case UniformName::registered_name_3: return 2;
				case UniformName::registered_name_4: return 3;
				case UniformName::registered_name_5: return 4;
				case UniformName::registered_name_6: return 5;
				case UniformName::registered_name_7: return 6;
				case UniformName::registered_name_8: return 7;
				case UniformName::registered_name_9: return 8;
				case UniformName::registered_name_10: return 9;
				case UniformName::registered_name_11: return 10;
				case UniformName::registered_name_12: return 11;
				case UniformName::registered_name_13: return 12;
				case UniformName::registered_name_14: return 13;
				case UniformName::registered_name_15: return 14;
				case UniformName::registered_name_16: return 15;
				default:
					return -1;
				}
			}*/
		};

		class StructuredUniform : public GLUniform, public GLUniformContainer
		{
		public:
			using ptr = std::shared_ptr<StructuredUniform>;

			StructuredUniform(const GLUniform::GLTexturesPtr& textures, std::string& id, UniformType type, const GLint addr) : GLUniform(textures,id, type, addr) {
				uniformClassType = UniformClassType::StructuredUniform;
			}

			static ptr create(const GLUniform::GLTexturesPtr& textures, std::string& id, UniformType type, const GLint addr) {
				return std::make_shared<StructuredUniform>(textures,id, type, addr);
			}

			virtual GLUniform::ptr clone(std::string& id) override {
				auto cloned = create(this->textures,this->id, this->type, this->addr);
				cloned->seq = this->seq;
				return cloned;
			}
			virtual GLUniform* asGLUniform() override {
				return nullptr;
			}

			virtual GLUniformContainer* asGLContainer() override {
				return this;
			}
			void setValue(UniformValue& v,int arrayIndex=0);
		};

		class GLUniforms : public GLUniformContainer {
		public:
			using GLTexturesPtr = std::shared_ptr<GLTextures>;
		
		protected:	

			GLTexturesPtr textures;

			GLuint program = 0;
			
			void initGLUniforms(const GLuint program);
			
		public:
			using ptr = std::shared_ptr<GLUniforms>;
			
			const std::string id = "GLUniforms";

			GLUniforms(const GLTexturesPtr& textures,const GLuint program) :textures(textures) {
				this->program = program;				
				initGLUniforms(program);
			}

			static ptr create(const GLTexturesPtr& textures,const GLuint program) {
				return std::make_shared<GLUniforms>(textures,program);
			}

			template <typename T>
			void setUniformValue(const std::string& name, const T& value) {
				if (seq.count(name)) seq[name]->setValue(value);
			}
	

			void parseUniform(const std::string& name, UniformType activeInfo, const GLint addr, GLUniformContainer* container);

			void setProjectionMatrix(const math::Matrix4& projMatrix);

			bool contains(const std::string& name) {
				return this->seq.count(name) > 0;
			}

			template<typename T>
			void setOptional(std::unordered_map<std::string,T>& objects, std::string name,GLTextures* textures=nullptr)
			{
				if (objects.count(name)) {
					T value = objects[name];
					setValue(name, value,textures);
				}
			}
			static void upload(const std::vector<GLUniform::ptr>& seq, UniformValues& values);

			std::vector<GLUniform::ptr> seqWithValue(UniformValues& values);

			
		};

		//template<typename T>
		//inline std::shared_ptr<GLUniforms> cloneUniforms(const GLUniforms& source) {
		//	GLUniforms::ptr target = GLUniforms::create();

		//	for (auto& entry : source) {
		//		target->insert({ entry.first,entry.second });
		//	}
		//	//target->copy(source);
		//	return  target;
		//}
	}
}
#endif