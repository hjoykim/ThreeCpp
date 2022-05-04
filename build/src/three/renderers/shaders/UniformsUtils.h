//#ifndef UNIFORMS_UTILS_THREE_H
//#define UNIFORMS_UTILS_THREE_H
//#include <three/renderers/gl/GLUniforms.h>
//namespace three {
//	namespace gl {
//		namespace uniformsutils {
//			GLUniforms::ptr cloneUniforms(const HashmapTable& source) {
//				GLUniforms::ptr target = GLUniforms::create();
//				
//				for (auto& entry : source) {
//					target->insert({ entry.first,entry.second });
//				}
//				//target->copy(source);
//				return  target;
//			}
//			GLUniforms::ptr merge(const std::vector<GLUniforms> uniforms) {
//				/*auto merged = GLUniforms::create();
//
//				for (unsigned i = 0; i < uniforms.size(); i++) {
//					GLUniforms uniform = uniforms[i];
//
//					std::copy(uniform.getTables().begin(), uniform.getTables().end(), merged->getTables().end());
//				}*/
//
//				return nullptr;
//			}
//
//			
//		}
//	}
//}
//
//#endif