#include "ShaderChunk.h"

#include <three/renderers/shaders/ShaderChunk/alphamap_fragment.h>
#include <three/renderers/shaders/ShaderChunk/alphamap_pars_fragment.h>
#include <three/renderers/shaders/ShaderChunk/alphatest_fragment.h>
#include <three/renderers/shaders/ShaderChunk/aomap_fragment.h>
#include <three/renderers/shaders/ShaderChunk/aomap_pars_fragment.h>
#include <three/renderers/shaders/ShaderChunk/beginnormal_vertex.h>
#include <three/renderers/shaders/ShaderChunk/begin_vertex.h>
#include <three/renderers/shaders/ShaderChunk/bsdfs.h>
#include <three/renderers/shaders/ShaderChunk/bumpmap_pars_fragment.h>
#include <three/renderers/shaders/ShaderChunk/clearcoat_normal_fragment_begin.h>
#include <three/renderers/shaders/ShaderChunk/clearcoat_normal_fragment_maps.h>
#include <three/renderers/shaders/ShaderChunk/clearcoat_pars_fragment.h>
#include <three/renderers/shaders/ShaderChunk/clipping_planes_fragment.h>
#include <three/renderers/shaders/ShaderChunk/clipping_planes_pars_fragment.h>
#include <three/renderers/shaders/ShaderChunk/clipping_planes_pars_vertex.h>
#include <three/renderers/shaders/ShaderChunk/clipping_planes_vertex.h>
#include <three/renderers/shaders/ShaderChunk/color_fragment.h>
#include <three/renderers/shaders/ShaderChunk/color_pars_fragment.h>
#include <three/renderers/shaders/ShaderChunk/color_pars_vertex.h>
#include <three/renderers/shaders/ShaderChunk/color_vertex.h>
#include <three/renderers/shaders/ShaderChunk/common.h>
#include <three/renderers/shaders/ShaderChunk/cube_uv_reflection_fragment.h>
#include <three/renderers/shaders/ShaderChunk/defaultnormal_vertex.h>
#include <three/renderers/shaders/ShaderChunk/default_fragment.h>
#include <three/renderers/shaders/ShaderChunk/default_vertex.h>
#include <three/renderers/shaders/ShaderChunk/displacementmap_pars_vertex.h>
#include <three/renderers/shaders/ShaderChunk/displacementmap_vertex.h>
#include <three/renderers/shaders/ShaderChunk/dithering_fragment.h>
#include <three/renderers/shaders/ShaderChunk/dithering_pars_fragment.h>
#include <three/renderers/shaders/ShaderChunk/emissivemap_fragment.h>
#include <three/renderers/shaders/ShaderChunk/emissivemap_pars_fragment.h>
#include <three/renderers/shaders/ShaderChunk/encodings_fragment.h>
#include <three/renderers/shaders/ShaderChunk/encodings_pars_fragment.h>
#include <three/renderers/shaders/ShaderChunk/envmap_common_pars_fragment.h>
#include <three/renderers/shaders/ShaderChunk/envmap_fragment.h>
#include <three/renderers/shaders/ShaderChunk/envmap_pars_fragment.h>
#include <three/renderers/shaders/ShaderChunk/envmap_pars_vertex.h>
#include <three/renderers/shaders/ShaderChunk/envmap_physical_pars_fragment.h>
#include <three/renderers/shaders/ShaderChunk/envmap_vertex.h>
#include <three/renderers/shaders/ShaderChunk/fog_fragment.h>
#include <three/renderers/shaders/ShaderChunk/fog_pars_fragment.h>
#include <three/renderers/shaders/ShaderChunk/fog_pars_vertex.h>
#include <three/renderers/shaders/ShaderChunk/fog_vertex.h>
#include <three/renderers/shaders/ShaderChunk/gradientmap_pars_fragment.h>
#include <three/renderers/shaders/ShaderChunk/lightmap_fragment.h>
#include <three/renderers/shaders/ShaderChunk/lightmap_pars_fragment.h>
#include <three/renderers/shaders/ShaderChunk/lights_fragment_begin.h>
#include <three/renderers/shaders/ShaderChunk/lights_fragment_end.h>
#include <three/renderers/shaders/ShaderChunk/lights_fragment_maps.h>
#include <three/renderers/shaders/ShaderChunk/lights_lambert_vertex.h>
#include <three/renderers/shaders/ShaderChunk/lights_pars_begin.h>
#include <three/renderers/shaders/ShaderChunk/lights_phong_fragment.h>
#include <three/renderers/shaders/ShaderChunk/lights_phong_pars_fragment.h>
#include <three/renderers/shaders/ShaderChunk/lights_physical_fragment.h>
#include <three/renderers/shaders/ShaderChunk/lights_physical_pars_fragment.h>
#include <three/renderers/shaders/ShaderChunk/lights_toon_fragment.h>
#include <three/renderers/shaders/ShaderChunk/lights_toon_pars_fragment.h>
#include <three/renderers/shaders/ShaderChunk/logdepthbuf_fragment.h>
#include <three/renderers/shaders/ShaderChunk/logdepthbuf_pars_fragment.h>
#include <three/renderers/shaders/ShaderChunk/logdepthbuf_pars_vertex.h>
#include <three/renderers/shaders/ShaderChunk/logdepthbuf_vertex.h>
#include <three/renderers/shaders/ShaderChunk/map_fragment.h>
#include <three/renderers/shaders/ShaderChunk/map_pars_fragment.h>
#include <three/renderers/shaders/ShaderChunk/map_particle_fragment.h>
#include <three/renderers/shaders/ShaderChunk/map_particle_pars_fragment.h>
#include <three/renderers/shaders/ShaderChunk/metalnessmap_fragment.h>
#include <three/renderers/shaders/ShaderChunk/metalnessmap_pars_fragment.h>
#include <three/renderers/shaders/ShaderChunk/morphnormal_vertex.h>
#include <three/renderers/shaders/ShaderChunk/morphtarget_pars_vertex.h>
#include <three/renderers/shaders/ShaderChunk/morphtarget_vertex.h>
#include <three/renderers/shaders/ShaderChunk/normalmap_pars_fragment.h>
#include <three/renderers/shaders/ShaderChunk/normal_fragment_begin.h>
#include <three/renderers/shaders/ShaderChunk/normal_fragment_maps.h>
#include <three/renderers/shaders/ShaderChunk/packing.h>
#include <three/renderers/shaders/ShaderChunk/premultiplied_alpha_fragment.h>
#include <three/renderers/shaders/ShaderChunk/project_vertex.h>
#include <three/renderers/shaders/ShaderChunk/roughnessmap_fragment.h>
#include <three/renderers/shaders/ShaderChunk/roughnessmap_pars_fragment.h>
#include <three/renderers/shaders/ShaderChunk/shadowmap_pars_fragment.h>
#include <three/renderers/shaders/ShaderChunk/shadowmap_pars_vertex.h>
#include <three/renderers/shaders/ShaderChunk/shadowmap_vertex.h>
#include <three/renderers/shaders/ShaderChunk/shadowmask_pars_fragment.h>
#include <three/renderers/shaders/ShaderChunk/skinbase_vertex.h>
#include <three/renderers/shaders/ShaderChunk/skinning_pars_vertex.h>
#include <three/renderers/shaders/ShaderChunk/skinning_vertex.h>
#include <three/renderers/shaders/ShaderChunk/skinnormal_vertex.h>
#include <three/renderers/shaders/ShaderChunk/specularmap_fragment.h>
#include <three/renderers/shaders/ShaderChunk/specularmap_pars_fragment.h>
#include <three/renderers/shaders/ShaderChunk/tonemapping_fragment.h>
#include <three/renderers/shaders/ShaderChunk/tonemapping_pars_fragment.h>
#include <three/renderers/shaders/ShaderChunk/uv2_pars_fragment.h>
#include <three/renderers/shaders/ShaderChunk/uv2_pars_vertex.h>
#include <three/renderers/shaders/ShaderChunk/uv2_vertex.h>
#include <three/renderers/shaders/ShaderChunk/uv_pars_fragment.h>
#include <three/renderers/shaders/ShaderChunk/uv_pars_vertex.h>
#include <three/renderers/shaders/ShaderChunk/uv_vertex.h>
#include <three/renderers/shaders/ShaderChunk/worldpos_vertex.h>
#include <three/renderers/shaders/ShaderLib/background_frag.h>
#include <three/renderers/shaders/ShaderLib/background_vert.h>
#include <three/renderers/shaders/ShaderLib/cube_frag.h>
#include <three/renderers/shaders/ShaderLib/cube_vert.h>
#include <three/renderers/shaders/ShaderLib/depth_frag.h>
#include <three/renderers/shaders/ShaderLib/depth_vert.h>
#include <three/renderers/shaders/ShaderLib/distanceRGBA_frag.h>
#include <three/renderers/shaders/ShaderLib/distanceRGBA_vert.h>
#include <three/renderers/shaders/ShaderLib/equirect_frag.h>
#include <three/renderers/shaders/ShaderLib/equirect_vert.h>
#include <three/renderers/shaders/ShaderLib/linedashed_frag.h>
#include <three/renderers/shaders/ShaderLib/linedashed_vert.h>
#include <three/renderers/shaders/ShaderLib/meshbasic_frag.h>
#include <three/renderers/shaders/ShaderLib/meshbasic_vert.h>
#include <three/renderers/shaders/ShaderLib/meshlambert_frag.h>
#include <three/renderers/shaders/ShaderLib/meshlambert_vert.h>
#include <three/renderers/shaders/ShaderLib/meshmatcap_frag.h>
#include <three/renderers/shaders/ShaderLib/meshmatcap_vert.h>
#include <three/renderers/shaders/ShaderLib/meshphong_frag.h>
#include <three/renderers/shaders/ShaderLib/meshphong_vert.h>
#include <three/renderers/shaders/ShaderLib/meshphysical_frag.h>
#include <three/renderers/shaders/ShaderLib/meshphysical_vert.h>
#include <three/renderers/shaders/ShaderLib/meshtoon_frag.h>
#include <three/renderers/shaders/ShaderLib/meshtoon_vert.h>
#include <three/renderers/shaders/ShaderLib/normal_frag.h>
#include <three/renderers/shaders/ShaderLib/normal_vert.h>
#include <three/renderers/shaders/ShaderLib/points_frag.h>
#include <three/renderers/shaders/ShaderLib/points_vert.h>
#include <three/renderers/shaders/ShaderLib/shadow_frag.h>
#include <three/renderers/shaders/ShaderLib/shadow_vert.h>
#include <three/renderers/shaders/ShaderLib/sprite_frag.h>
#include <three/renderers/shaders/ShaderLib/sprite_vert.h>
#include <three/renderers/shaders/ShaderLib/vsm_frag.h>
#include <three/renderers/shaders/ShaderLib/vsm_vert.h>
#include <three/renderers/shaders/ShaderChunk/transmissionmap_fragment.h>
#include <three/renderers/shaders/ShaderChunk/transmissionmap_pars_fragment.h>
#include <unordered_map>
#include <three/Types.h>
#include <vector>
#include <three\Constants.h>

namespace three {
	namespace gl {		
		namespace shaderlib_name {
			#define MATCH_FILE_NAME(nm) {#nm,ShaderLibID::nm}

			ShaderLibID get(std::string name)
			{
				static const std::unordered_map<std::string, ShaderLibID> string_file_to_name{
					MATCH_FILE_NAME(alphamap_fragment),
					MATCH_FILE_NAME(alphamap_pars_fragment),
					MATCH_FILE_NAME(alphatest_fragment),
					MATCH_FILE_NAME(aomap_fragment),
					MATCH_FILE_NAME(aomap_pars_fragment),
					MATCH_FILE_NAME(begin_vertex),
					MATCH_FILE_NAME(beginnormal_vertex),
					MATCH_FILE_NAME(bsdfs),
					MATCH_FILE_NAME(bumpmap_pars_fragment),
					MATCH_FILE_NAME(clipping_planes_fragment),
					MATCH_FILE_NAME(clipping_planes_pars_fragment),
					MATCH_FILE_NAME(clipping_planes_pars_vertex),
					MATCH_FILE_NAME(clipping_planes_vertex),
					MATCH_FILE_NAME(color_fragment),
					MATCH_FILE_NAME(color_pars_fragment),
					MATCH_FILE_NAME(color_pars_vertex),
					MATCH_FILE_NAME(color_vertex),
					MATCH_FILE_NAME(common),
					MATCH_FILE_NAME(cube_uv_reflection_fragment),
					MATCH_FILE_NAME(default_vertex),
					MATCH_FILE_NAME(default_fragment),
					MATCH_FILE_NAME(defaultnormal_vertex),
					MATCH_FILE_NAME(displacementmap_pars_vertex),
					MATCH_FILE_NAME(displacementmap_vertex),
					MATCH_FILE_NAME(emissivemap_fragment),
					MATCH_FILE_NAME(emissivemap_pars_fragment),
					MATCH_FILE_NAME(encodings_fragment),
					MATCH_FILE_NAME(encodings_pars_fragment),
					MATCH_FILE_NAME(envmap_fragment),
					MATCH_FILE_NAME(envmap_common_pars_fragment),
					MATCH_FILE_NAME(envmap_pars_fragment),
					MATCH_FILE_NAME(envmap_pars_vertex),
					MATCH_FILE_NAME(envmap_physical_pars_fragment),
					MATCH_FILE_NAME(envmap_vertex),
					MATCH_FILE_NAME(fog_vertex),
					MATCH_FILE_NAME(fog_pars_vertex),
					MATCH_FILE_NAME(fog_fragment),
					MATCH_FILE_NAME(fog_pars_fragment),
					MATCH_FILE_NAME(gradientmap_pars_fragment),
					MATCH_FILE_NAME(lightmap_fragment),
					MATCH_FILE_NAME(lightmap_pars_fragment),
					MATCH_FILE_NAME(lights_lambert_vertex),
					MATCH_FILE_NAME(lights_pars_begin),
					MATCH_FILE_NAME(lights_toon_fragment),
					MATCH_FILE_NAME(lights_toon_pars_fragment),
					MATCH_FILE_NAME(lights_phong_fragment),
					MATCH_FILE_NAME(lights_phong_pars_fragment),
					MATCH_FILE_NAME(lights_physical_fragment),
					MATCH_FILE_NAME(lights_physical_pars_fragment),
					MATCH_FILE_NAME(lights_fragment_begin),
					MATCH_FILE_NAME(lights_fragment_maps),
					MATCH_FILE_NAME(lights_fragment_end),
					MATCH_FILE_NAME(logdepthbuf_fragment),
					MATCH_FILE_NAME(logdepthbuf_pars_fragment),
					MATCH_FILE_NAME(logdepthbuf_pars_vertex),
					MATCH_FILE_NAME(logdepthbuf_vertex),
					MATCH_FILE_NAME(map_fragment),
					MATCH_FILE_NAME(map_pars_fragment),
					MATCH_FILE_NAME(map_particle_fragment),
					MATCH_FILE_NAME(map_particle_pars_fragment),
					MATCH_FILE_NAME(metalnessmap_fragment),
					MATCH_FILE_NAME(metalnessmap_pars_fragment),
					MATCH_FILE_NAME(morphnormal_vertex),
					MATCH_FILE_NAME(morphtarget_pars_vertex),
					MATCH_FILE_NAME(morphtarget_vertex),
					MATCH_FILE_NAME(normal_fragment_begin),
					MATCH_FILE_NAME(normal_fragment_maps),
					MATCH_FILE_NAME(normalmap_pars_fragment),
					MATCH_FILE_NAME(clearcoat_normal_fragment_begin),
					MATCH_FILE_NAME(clearcoat_normal_fragment_maps),
					MATCH_FILE_NAME(clearcoat_pars_fragment),
					MATCH_FILE_NAME(packing),
					MATCH_FILE_NAME(premultiplied_alpha_fragment),
					MATCH_FILE_NAME(project_vertex),
					MATCH_FILE_NAME(dithering_fragment),
					MATCH_FILE_NAME(dithering_pars_fragment),
					MATCH_FILE_NAME(roughnessmap_fragment),
					MATCH_FILE_NAME(roughnessmap_pars_fragment),
					MATCH_FILE_NAME(shadowmap_pars_fragment),
					MATCH_FILE_NAME(shadowmap_pars_vertex),
					MATCH_FILE_NAME(shadowmap_vertex),
					MATCH_FILE_NAME(shadowmask_pars_fragment),
					MATCH_FILE_NAME(skinbase_vertex),
					MATCH_FILE_NAME(skinning_pars_vertex),
					MATCH_FILE_NAME(skinning_vertex),
					MATCH_FILE_NAME(skinnormal_vertex),
					MATCH_FILE_NAME(specularmap_fragment),
					MATCH_FILE_NAME(specularmap_pars_fragment),
					MATCH_FILE_NAME(tonemapping_fragment),
					MATCH_FILE_NAME(tonemapping_pars_fragment),
					MATCH_FILE_NAME(transmissionmap_fragment),
					MATCH_FILE_NAME(transmissionmap_pars_fragment),
					MATCH_FILE_NAME(uv_pars_fragment),
					MATCH_FILE_NAME(uv_pars_vertex),
					MATCH_FILE_NAME(uv_vertex),
					MATCH_FILE_NAME(uv2_pars_fragment),
					MATCH_FILE_NAME(uv2_pars_vertex),
					MATCH_FILE_NAME(uv2_vertex),
					MATCH_FILE_NAME(worldpos_vertex),
					MATCH_FILE_NAME(background_frag),
					MATCH_FILE_NAME(background_vert),
					MATCH_FILE_NAME(cube_frag),
					MATCH_FILE_NAME(cube_vert),
					MATCH_FILE_NAME(depth_frag),
					MATCH_FILE_NAME(depth_vert),
					MATCH_FILE_NAME(distanceRGBA_frag),
					MATCH_FILE_NAME(distanceRGBA_vert),
					MATCH_FILE_NAME(equirect_frag),
					MATCH_FILE_NAME(equirect_vert),
					MATCH_FILE_NAME(linedashed_frag),
					MATCH_FILE_NAME(linedashed_vert),
					MATCH_FILE_NAME(meshbasic_frag),
					MATCH_FILE_NAME(meshbasic_vert),
					MATCH_FILE_NAME(meshlambert_frag),
					MATCH_FILE_NAME(meshlambert_vert),
					MATCH_FILE_NAME(meshmatcap_frag),
					MATCH_FILE_NAME(meshmatcap_vert),
					MATCH_FILE_NAME(meshtoon_frag),
					MATCH_FILE_NAME(meshtoon_vert),
					MATCH_FILE_NAME(meshphong_frag),
					MATCH_FILE_NAME(meshphong_vert),
					MATCH_FILE_NAME(meshphysical_frag),
					MATCH_FILE_NAME(meshphysical_vert),
					MATCH_FILE_NAME(normal_frag),
					MATCH_FILE_NAME(normal_vert),
					MATCH_FILE_NAME(points_frag),
					MATCH_FILE_NAME(points_vert),
					MATCH_FILE_NAME(shadow_frag),
					MATCH_FILE_NAME(shadow_vert),
					MATCH_FILE_NAME(sprite_frag),
					MATCH_FILE_NAME(sprite_vert),
					MATCH_FILE_NAME(vsm_vert),
					MATCH_FILE_NAME(vsm_frag)
				};

				auto found = string_file_to_name.find(name);
				if (found != string_file_to_name.end()) return found->second;
				else return ShaderLibID::unknown;
			}
		}

		class ShaderChunks
		{		
			
		public:
			std::unordered_map<ShaderLibID, const char*> shader_chunks;
			
			ShaderChunks()
			{
				shader_chunks = {
						{ShaderLibID::alphamap_fragment,alphamap_fragment},
						{ShaderLibID::alphamap_pars_fragment,alphamap_pars_fragment},
						{ShaderLibID::alphatest_fragment,alphatest_fragment},
						{ShaderLibID::aomap_fragment,aomap_fragment},
						{ShaderLibID::aomap_pars_fragment,aomap_pars_fragment},
						{ShaderLibID::begin_vertex,begin_vertex},
						{ShaderLibID::beginnormal_vertex,beginnormal_vertex},
						{ShaderLibID::bsdfs,bsdfs},
						{ShaderLibID::bumpmap_pars_fragment,bumpmap_pars_fragment},
						{ShaderLibID::clipping_planes_fragment,clipping_planes_fragment},
						{ShaderLibID::clipping_planes_pars_fragment,clipping_planes_pars_fragment},
						{ShaderLibID::clipping_planes_pars_vertex,clipping_planes_pars_vertex},
						{ShaderLibID::clipping_planes_vertex,clipping_planes_vertex},
						{ShaderLibID::color_fragment,color_fragment},
						{ShaderLibID::color_pars_fragment,color_pars_fragment},
						{ShaderLibID::color_pars_vertex,color_pars_vertex},
						{ShaderLibID::color_vertex,color_vertex},
						{ShaderLibID::common,common},
						{ShaderLibID::cube_uv_reflection_fragment,cube_uv_reflection_fragment},
						{ShaderLibID::default_vertex,default_vertex},
						{ShaderLibID::default_fragment,default_fragment},
						{ShaderLibID::defaultnormal_vertex,defaultnormal_vertex},
						{ShaderLibID::displacementmap_pars_vertex,displacementmap_pars_vertex},
						{ShaderLibID::displacementmap_vertex,displacementmap_vertex},
						{ShaderLibID::emissivemap_fragment,emissivemap_fragment},
						{ShaderLibID::emissivemap_pars_fragment,emissivemap_pars_fragment},
						{ShaderLibID::encodings_fragment,encodings_fragment},
						{ShaderLibID::encodings_pars_fragment,encodings_pars_fragment},
						{ShaderLibID::envmap_fragment,envmap_fragment},
						{ShaderLibID::envmap_common_pars_fragment,envmap_common_pars_fragment},
						{ShaderLibID::envmap_pars_fragment,envmap_pars_fragment},
						{ShaderLibID::envmap_pars_vertex,envmap_pars_vertex},
						{ShaderLibID::envmap_physical_pars_fragment,envmap_physical_pars_fragment},
						{ShaderLibID::envmap_vertex,envmap_vertex},
						{ShaderLibID::fog_vertex,fog_vertex},
						{ShaderLibID::fog_pars_vertex,fog_pars_vertex},
						{ShaderLibID::fog_fragment,fog_fragment},
						{ShaderLibID::fog_pars_fragment,fog_pars_fragment},
						{ShaderLibID::gradientmap_pars_fragment,gradientmap_pars_fragment},
						{ShaderLibID::lightmap_fragment,lightmap_fragment},
						{ShaderLibID::lightmap_pars_fragment,lightmap_pars_fragment},
						{ShaderLibID::lights_lambert_vertex,lights_lambert_vertex},
						{ShaderLibID::lights_pars_begin,lights_pars_begin},
						{ShaderLibID::lights_toon_fragment,lights_toon_fragment},
						{ShaderLibID::lights_toon_pars_fragment,lights_toon_pars_fragment},
						{ShaderLibID::lights_phong_fragment,lights_phong_fragment},
						{ShaderLibID::lights_phong_pars_fragment,lights_phong_pars_fragment},
						{ShaderLibID::lights_physical_fragment,lights_physical_fragment},
						{ShaderLibID::lights_physical_pars_fragment,lights_physical_pars_fragment},
						{ShaderLibID::lights_fragment_begin,lights_fragment_begin},
						{ShaderLibID::lights_fragment_maps,lights_fragment_maps},
						{ShaderLibID::lights_fragment_end,lights_fragment_end},
						{ShaderLibID::logdepthbuf_fragment,logdepthbuf_fragment},
						{ShaderLibID::logdepthbuf_pars_fragment,logdepthbuf_pars_fragment},
						{ShaderLibID::logdepthbuf_pars_vertex,logdepthbuf_pars_vertex},
						{ShaderLibID::logdepthbuf_vertex,logdepthbuf_vertex},
						{ShaderLibID::map_fragment,map_fragment},
						{ShaderLibID::map_pars_fragment,map_pars_fragment},
						{ShaderLibID::map_particle_fragment,map_particle_fragment},
						{ShaderLibID::map_particle_pars_fragment,map_particle_pars_fragment},
						{ShaderLibID::metalnessmap_fragment,metalnessmap_fragment},
						{ShaderLibID::metalnessmap_pars_fragment,metalnessmap_pars_fragment},
						{ShaderLibID::morphnormal_vertex,morphnormal_vertex},
						{ShaderLibID::morphtarget_pars_vertex,morphtarget_pars_vertex},
						{ShaderLibID::morphtarget_vertex,morphtarget_vertex},
						{ShaderLibID::normal_fragment_begin,normal_fragment_begin},
						{ShaderLibID::normal_fragment_maps,normal_fragment_maps},
						{ShaderLibID::normalmap_pars_fragment,normalmap_pars_fragment},
						{ShaderLibID::clearcoat_normal_fragment_begin,clearcoat_normal_fragment_begin},
						{ShaderLibID::clearcoat_normal_fragment_maps,clearcoat_normal_fragment_maps},
						{ShaderLibID::clearcoat_pars_fragment,clearcoat_pars_fragment},
						{ShaderLibID::packing,packing},
						{ShaderLibID::premultiplied_alpha_fragment,premultiplied_alpha_fragment},
						{ShaderLibID::project_vertex,project_vertex},
						{ShaderLibID::dithering_fragment,dithering_fragment},
						{ShaderLibID::dithering_pars_fragment,dithering_pars_fragment},
						{ShaderLibID::roughnessmap_fragment,roughnessmap_fragment},
						{ShaderLibID::roughnessmap_pars_fragment,roughnessmap_pars_fragment},
						{ShaderLibID::shadowmap_pars_fragment,shadowmap_pars_fragment},
						{ShaderLibID::shadowmap_pars_vertex,shadowmap_pars_vertex},
						{ShaderLibID::shadowmap_vertex,shadowmap_vertex},
						{ShaderLibID::shadowmask_pars_fragment,shadowmask_pars_fragment},
						{ShaderLibID::skinbase_vertex,skinbase_vertex},
						{ShaderLibID::skinning_pars_vertex,skinning_pars_vertex},
						{ShaderLibID::skinning_vertex,skinning_vertex},
						{ShaderLibID::skinnormal_vertex,skinnormal_vertex},
						{ShaderLibID::specularmap_fragment,specularmap_fragment},
						{ShaderLibID::specularmap_pars_fragment,specularmap_pars_fragment},
						{ShaderLibID::tonemapping_fragment,tonemapping_fragment},
						{ShaderLibID::tonemapping_pars_fragment,tonemapping_pars_fragment},
						{ShaderLibID::transmissionmap_fragment,transmissionmap_fragment},
						{ShaderLibID::transmissionmap_pars_fragment,transmissionmap_pars_fragment},
						{ShaderLibID::vsm_frag,vsm_frag},
						{ShaderLibID::uv_pars_fragment,uv_pars_fragment},
						{ShaderLibID::uv_pars_vertex,uv_pars_vertex},
						{ShaderLibID::uv_vertex,uv_vertex},
						{ShaderLibID::uv2_pars_fragment,uv2_pars_fragment},
						{ShaderLibID::uv2_pars_vertex,uv2_pars_vertex},
						{ShaderLibID::uv2_vertex,uv2_vertex},
						{ShaderLibID::worldpos_vertex,worldpos_vertex},
						{ShaderLibID::background_frag,background_frag},
						{ShaderLibID::background_vert,background_vert},
						{ShaderLibID::cube_frag,cube_frag},
						{ShaderLibID::cube_vert,cube_vert},
						{ShaderLibID::depth_frag,depth_frag},
						{ShaderLibID::depth_vert,depth_vert},
						{ShaderLibID::distanceRGBA_frag,distanceRGBA_frag},
						{ShaderLibID::distanceRGBA_vert,distanceRGBA_vert},
						{ShaderLibID::equirect_frag,equirect_frag},
						{ShaderLibID::equirect_vert,equirect_vert},
						{ShaderLibID::linedashed_frag,linedashed_frag},
						{ShaderLibID::linedashed_vert,linedashed_vert},
						{ShaderLibID::meshbasic_frag,meshbasic_frag},
						{ShaderLibID::meshbasic_vert,meshbasic_vert},
						{ShaderLibID::meshlambert_frag,meshlambert_frag},
						{ShaderLibID::meshlambert_vert,meshlambert_vert},
						{ShaderLibID::meshmatcap_frag,meshmatcap_frag},
						{ShaderLibID::meshmatcap_vert,meshmatcap_vert},
						{ShaderLibID::meshtoon_frag,meshtoon_frag},
						{ShaderLibID::meshtoon_vert,meshtoon_vert},
						{ShaderLibID::meshphong_frag,meshphong_frag},
						{ShaderLibID::meshphong_vert,meshphong_vert},
						{ShaderLibID::meshphysical_frag,meshphysical_frag},
						{ShaderLibID::meshphysical_vert,meshphysical_vert},
						{ShaderLibID::normal_frag,normal_frag},
						{ShaderLibID::normal_vert,normal_vert},
						{ShaderLibID::points_frag,points_frag},
						{ShaderLibID::points_vert,points_vert},
						{ShaderLibID::shadow_frag,shadow_frag},
						{ShaderLibID::shadow_vert,shadow_vert},
						{ShaderLibID::sprite_frag,sprite_frag},
						{ShaderLibID::sprite_vert,sprite_vert},
						{ShaderLibID::vsm_vert,vsm_vert},
						{ShaderLibID::vsm_frag,vsm_frag}

					};
			}
		};
		const char* getShaderChunk(ShaderLibID id)
		{
			static ShaderChunks shader;
			return shader.shader_chunks[id];
		}
	}
}
