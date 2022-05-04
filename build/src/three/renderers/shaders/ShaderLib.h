#ifndef SHADERLIB_THREE_H
#define SHADERLIB_THREE_H
#include <string>
#include <unordered_map>
#include <memory>
#include "UniformsLib.h"
namespace three {
	namespace gl {
		class GLShader;		
		class UniformValues {
			std::unordered_map<std::string, std::shared_ptr<UniformValue>,StringHash> values;
		public:
			using ptr = std::shared_ptr<UniformValues>;

			UniformValues() {}
			
			UniformValues(const LibUniformValues& libUniforms) : values(libUniforms.cloneValues()) {}
			
			virtual ~UniformValues() = default;

			template<typename T>
			UniformValues& set(const std::string& name, const T& t)
			{
				if (values.count(name) > 0) {
					*values[name] = t;
				}
				else {
					values[name] = UniformValueT<T>::create(name, t);
				}
				return *this;
			}
			
			bool contains(const std::string& name) {
				return values.count(name) > 0;
			}
			void needsUpdate(const std::string& name, bool value) {
				if (values.count(name) > 0) values[name]->needsUpdate = value;
			}
			
			UniformValue& get(const std::string& name) {
				return *values.at(name);
			}

			UniformValue& operator[] (const std::string& name) {
				return *values.at(name);
			}
		
		};
		GLShader& getShader(const std::string& id);
		
		

	}
}
#endif