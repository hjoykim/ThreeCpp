#ifndef EXTENSIONS_THREE_H
#define EXTENSIONS_THREE_H
#include <unordered_map>
#include <memory>
#include <three/Constants.h>
#include <three/Types.h>
namespace three {
	namespace gl {
		

		class UseExtension
		{
			uint16_t bits;

			template<typename Ret> 
			static Ret decl() { return (Ret)0; }
			
			template<typename Ret,Extension e,Extension... Args>
			static Ret decl() {
				return (Ret)e | decl<Ret, Args...>();
			}

		public :
			UseExtension(uint16_t bits = 0) : bits(bits) {}

			template<Extension... extensions>
			static UseExtension use() {
				return UseExtension(decl<uint16_t, extensions...>());
			}

			UseExtension& add(Extension ext) {
				bits |= (uint16_t)ext;
				return *this;
			}

			bool get(Extension ext) const {
				return (bits & (size_t)ext) != 0;
			}
		};

		class GLExtensions {
			enum_map<Extension, bool> extensions;
		public:
			using ptr = std::shared_ptr<GLExtensions>;
			GLExtensions() {}
			
			bool get(Extension extension) {
				switch (extension) {
#ifdef OPENGL_ES_3_2
				case Extension::OES_texture_float:
					extensions[extension] = glGetString(GL_OES_texture_float)!=NULL;
					break;
				case Extension::OES_texture_float_linear:
					extensions[extension] = glGetString(GL_OES_texture_float_linear)!=NULL;
					break;
				case Extension::OES_texture_half_float:
					extensions[extension] = glGetString(GL_OES_texture_half_float)!=NULL;
					break;
				case Extension::OES_texture_half_float_linear:
					extensions[extension] = glGetString(GL_OES_texture_half_float_linear)!=NULL;
					break;
				case Extension::OES_standard_derivatives:
					extensions[extension] = glGetString(GL_OES_standard_derivatives)!=NULL;
					break;
			
				case Extension::OES_element_index_uint:
					extensions[extension] = glGetString(GL_OES_element_index_uint)!=NULL;
					break;
				case Extension::EXT_frag_depth:
					extensions[extension] = glGetString(GL_EXT_frag_depth) != NULL;
					break;
#endif				

#ifdef IOS
				case Extension::OES_standard_derivatives:
					extensions[extension] = glGetString(GL_OES_standard_derivatives) != NULL;
					break;

				case Extension::OES_element_index_uint:
					extensions[extension] = glGetString(GL_OES_element_index_uint) != NULL;
					break;
#endif		
#ifdef GLAD
				case Extension::OES_texture_float:
					extensions[extension] = glGetString(GL_ARB_texture_float) != NULL;
					break;
				case Extension::OES_texture_float_linear:
					extensions[extension] = glGetString(GL_ARB_texture_float) != NULL;
					break;
				case Extension::OES_texture_half_float:
					extensions[extension] = glGetString(GL_ARB_texture_float) != NULL;
					break;
				case Extension::OES_texture_half_float_linear:
					extensions[extension] = glGetString(GL_ARB_texture_float) != NULL;
					break;
				case Extension::ANGLE_instanced_arrays:
					extensions[extension] = glGetString(GL_ANGLE_instanced_arrays) != NULL;

				break;case Extension::EXT_shader_texture_lod:
					extensions[extension] = glGetString(GL_EXT_shader_texture_lod)!=NULL;
					break;				
				case Extension::ARB_instanced_arrays:
					extensions[extension] = glGetString(GL_ARB_instanced_arrays) != NULL;
					break;
				case Extension::ARB_ES3_compatibility:
					extensions[extension] = glGetString(GL_ARB_ES3_compatibility) != NULL;
				case Extension::EXT_texture_compression_s3tc:
					extensions[extension] = glGetString(GL_EXT_texture_compression_s3tc) != NULL;
					break;
				case Extension::ARB_depth_texture:
					extensions[extension] = glGetString(GL_ARB_depth_texture) != NULL;
					break;
				case Extension::EXT_texture_filter_anisotropic:
#ifdef GLEW
					extensions[extension] = glGetString(GL_EXT_texture_filter_anisotropic) != NULL;
#else
					extensions[extension] = false;
#endif
					break;
				case Extension::EXT_blend_minmax:
					extensions[extension] = glGetString(GL_EXT_blend_minmax) != NULL;
#endif
				
				default:
					extensions[extension] = false;
				}
				checkError();
				return extensions[extension];
			}

			bool operator[](Extension extension) {
				return get(extension);
			}
		};
	}
}
#endif