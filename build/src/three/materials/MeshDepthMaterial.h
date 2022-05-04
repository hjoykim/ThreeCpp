#ifndef MESH_DEPTH_MATERIAL_THREE_H
#define MESH_DEPTH_MATERIAL_THREE_H

#include <three/math/Color.h>
#include <three/textures/Texture.h>
#include <string>
#include <three/materials/Material.h>
namespace three {
	class MeshDepthMaterial : public Material {
	public:
		using ptr = std::shared_ptr<MeshDepthMaterial>;
		MeshDepthMaterial() :Material() {
			name = "MeshDepthMaterial";
			type = "MeshDepthMaterial";

			shaderId = "depth";

			depthPacking = DepthPackingStrategies::BasicDepthPacking;

			skinning = false;
			morphTargets = false;

			//map = null;

			//alphaMap = null;

			//displacementMap = null;
			displacementScale = 1;
			displacementBias = 0;

			wireframe = false;
			wireframeLinewidth = 1;

			fog = false;

		}
		MeshDepthMaterial(const MeshDepthMaterial& source) : Material(source) {
			shaderId = "depth";
			depthPacking = source.depthPacking;

			skinning = source.skinning;
			morphTargets = source.morphTargets;
						
			map = source.map;

			alphaMap = source.alphaMap;

			displacementMap = source.displacementMap;
			displacementScale = source.displacementScale;
			displacementBias = source.displacementBias;

			wireframe = source.wireframe;
			wireframeLinewidth = source.wireframeLinewidth;
		}
		virtual ~MeshDepthMaterial() = default;

		virtual MeshDepthMaterial* clone() {
			return new MeshDepthMaterial(*this);
		}
		virtual MeshDepthMaterial& copy(const MeshDepthMaterial& source) {
			Material::copy(source);
			depthPacking = source.depthPacking;
			skinning = source.skinning;
			morphTargets = source.morphTargets;
			map = source.map;

			alphaMap = source.alphaMap;

			displacementMap = source.displacementMap;
			displacementScale = source.displacementScale;
			displacementBias = source.displacementBias;

			wireframe = source.wireframe;
			wireframeLinewidth = source.wireframeLinewidth;

			return *this;
		}	

	};
}
#endif