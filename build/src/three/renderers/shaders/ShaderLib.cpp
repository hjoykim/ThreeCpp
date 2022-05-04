#include "ShaderLib.h"
#include "UniformsLib.h"
#include <three/renderers/shaders/ShaderChunk.h>
#include <three/renderers/shaders/ShaderChunk.h>
#include <three/renderers/gl/GLShader.h>
#include <three/Types.h>
using namespace std;
namespace three {
	namespace gl {
		class ShaderLib {
		private:
			void add(const std::string& shaderId, const GLShader& shader) {
				shaderLib.emplace(shaderId, shader);
			}
		protected:
			std::unordered_map<std::string, GLShader,StringHash> shaderLib;

		public:
			ShaderLib() {
				Texture::ptr emptyTexture;

				add("basic",
					GLShader(getShaderChunk(ShaderLibID::meshbasic_vert), getShaderChunk(ShaderLibID::meshbasic_frag),
						merged({
							"common",
							"specularmap",
							"envmap",
							"aomap",
							"lightmap",
							"fog"
							})
					)
				);

				add("lambert",
					GLShader(getShaderChunk(ShaderLibID::meshlambert_vert), getShaderChunk(ShaderLibID::meshlambert_frag),
						merged({
							"common",
							"specularmap",
							"envmap",
							"aomap",
							"lightmap",
							"emissivemap",
							"fog",
							"lights"
							}).add({
									{"emissive",Color(0x000000)}
								}
							)
					)
				);

				add("phong",
					GLShader(getShaderChunk(ShaderLibID::meshphong_vert), getShaderChunk(ShaderLibID::meshphong_frag),
						merged({
						"common",
						"specularmap",
						"envmap",
						"aomap",
						"lightmap",
						"emissivemap",
						"bumpmap",
						"normalmap",
						"displacementmap",
						"fog",
						"lights"}).add({
							{"emissive",Color(0x000000)},
							{"specular",Color(0x111111)},
							{"shininess",30.0f}
							})
					)
				);


				add("standard",
					GLShader(getShaderChunk(ShaderLibID::meshphysical_vert), getShaderChunk(ShaderLibID::meshphysical_frag),
						merged({
						"common",
						"envmap",
						"aomap",
						"lightmap",
						"emissivemap",
						"bumpmap",
						"normalmap",
						"displacementmap",
						"roughnessmap",
						"metalnessmap",
						"fog",
						"lights" }).add({
							{"emissive",Color(0x000000)},
							{"roughness",0.5f},
							{"metalness",0.5f},
							{"envMapIntensity",1.0f}
							})
					)
				);

				add("toon",
					GLShader(getShaderChunk(ShaderLibID::meshtoon_vert), getShaderChunk(ShaderLibID::meshtoon_frag),
						merged({
						"common",
						"specularmap",
						"aomap",
						"lightmap",
						"emissivemap",
						"bumpmap",
						"normalmap",
						"displacementmap",
						"gradientmap",
						"fog",
						"lights"}).add({
							{"emissive",Color(0x000000)},
							{"specular",Color(0x111111)},
							{"shininess",30.0f}
							})
					)
				);

				add("matcap",
					GLShader(getShaderChunk(ShaderLibID::meshmatcap_vert), getShaderChunk(ShaderLibID::meshmatcap_frag),
						merged({
						"common",
						"bumpmap",
						"normalmap",
						"displacementmap",
						"fog"
							}).add({
								{"matcap",emptyTexture}
								})
					)
				);


				add("points",
					GLShader(getShaderChunk(ShaderLibID::points_vert), getShaderChunk(ShaderLibID::points_frag),
						merged({
							"points",
							"fog" })
							)
				);

				add("dashed",
					GLShader(getShaderChunk(ShaderLibID::linedashed_vert), getShaderChunk(ShaderLibID::linedashed_frag),
						merged({
						"common",
						"fog" }).add({
							{"scale",1.0f},
							{"dashSize",1.0f},
							{"totalSize",2.0f}
							})
					)
				);

				add("depth",
					GLShader(getShaderChunk(ShaderLibID::depth_vert), getShaderChunk(ShaderLibID::depth_frag),
						merged({
						"common",
						"displacementmap"})
					)
				);

				add("normal",
					GLShader(getShaderChunk(ShaderLibID::normal_vert), getShaderChunk(ShaderLibID::normal_frag),
						merged({
						"common",
						"bumpmap",
						"normalmap",
						"displacementmap"}).add({
							{"opacity",1.0f}
							})
					)
				);

				add("sprite",
					GLShader(getShaderChunk(ShaderLibID::sprite_vert), getShaderChunk(ShaderLibID::sprite_frag),
						merged({
						"sprite",
						"fog"
							})
					)
				);

				add("background",
					GLShader(getShaderChunk(ShaderLibID::background_vert), getShaderChunk(ShaderLibID::background_frag),
						merged({}).add({
							{"uvTransform", Matrix3()},
							{"t2D",emptyTexture}
						})
					)
				);


				add("cube",
					GLShader(getShaderChunk(ShaderLibID::cube_vert), getShaderChunk(ShaderLibID::cube_frag),
						merged({
						"envmap"
							}).add({
								{"opacity",1.0f}
								})
					)
				);

				add("equirect",
					GLShader(getShaderChunk(ShaderLibID::equirect_vert), getShaderChunk(ShaderLibID::equirect_frag),
						merged({}).add({
							{"tEquirect",emptyTexture}
						})
					)
				);



				add("distanceRGBA",
					GLShader(getShaderChunk(ShaderLibID::distanceRGBA_vert), getShaderChunk(ShaderLibID::distanceRGBA_frag),
						merged({
						"common",
						"displacementmap"
							}).add({
								{"referencePosition", Vector3()},
								{"nearDistance",1.0f},
								{"farDistance",1000.0f}
								})
					)
				);

				add("shadow",
					GLShader(getShaderChunk(ShaderLibID::shadow_vert), getShaderChunk(ShaderLibID::shadow_frag),
						merged({
						"lights",
						"fog"}).add({
							{"color",Color(0x000000)},
							{"opacity",1.0f}
							})
					)
				);



				add("physical",
					GLShader(getShaderChunk(ShaderLibID::meshphysical_vert), getShaderChunk(ShaderLibID::meshphysical_frag),
						merged({
						"common",
						"envmap",
						"aomap",
						"lightmap",
						"emissivemap",
						"bumpmap",
						"normalmap",
						"displacementmap",
						"roughnessmap",
						"metalnessmap",
						"fog",
						"lights"}).add({
							{"emissive",Color(0x000000)},
							{"roughness",0.5f},
							{"metalness",0.5f},
							{"envMapIntensity",1.0f},
							{"clearcoat",0.0f},
							{"clearcoatMap",  emptyTexture},
							{"clearcoatRoughness", 0.0f},
							{"clearcoatRoughnessMap",emptyTexture},
							{"clearcoatNormalScale", Vector2(1, 1)},
							{"clearcoatNormalMap", emptyTexture},
							{"sheen", Color(0x000000)},
							{"transparency",1.0f}
							})
					)
				);
			}
			virtual ~ShaderLib() = default;

			GLShader& getShader(const std::string& id) {
				
				return shaderLib[id];
			}

		};		

		static ShaderLib _shaderLib;
		GLShader& getShader(const std::string& id )
		{
			//static ShaderLib _shaderLib;			
			return _shaderLib.getShader(id);
		}			
		
}
}
