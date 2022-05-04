#ifndef MESH_NORMAL_MATERIAL_THREE_H
#define MESH_NORMAL_MATERIAL_THREE_H
#include <string>
#include <three/materials/Material.h>
#include <three/Constants.h>
namespace three {
	class MeshNormalMaterial : public Material{
	public:

		MeshNormalMaterial() : Material() 
		{
			type = "MeshNormalMaterial";
			name = "MeshNormalMaterial";
			shaderId = "normal";
			//bumpMap = null;
			bumpScale = 1;

			//normalMap = null;

			normalMapType = NormalMapTypes::TangentSpaceNormalMap;

			normalScale.set(1.0f, 1.0f);//new Vector2(1, 1);

			//displacementMap = null;
			displacementScale = 1;

			displacementBias = 0;

			wireframe = false;

			wireframeLinewidth = 1;

			fog = false;

			skinning = false;

			morphTargets = false;

			morphNormals = false;
		}
		
		MeshNormalMaterial(const MeshNormalMaterial& source) : Material(source) {
			shaderId = "normal";
			bumpMap = source.bumpMap;
			bumpScale = source.bumpScale;

			normalMap = source.normalMap;
			normalMapType = source.normalMapType;
			normalScale.copy(source.normalScale);

			displacementMap = source.displacementMap;
			displacementScale = source.displacementScale;
			displacementBias = source.displacementBias;

			wireframe = source.wireframe;
			wireframeLinewidth = source.wireframeLinewidth;

			skinning = source.skinning;
			morphTargets = source.morphTargets;
			morphNormals = source.morphNormals;
		}

		virtual ~MeshNormalMaterial() = default;

		virtual MeshNormalMaterial* clone() {
			return new MeshNormalMaterial(*this);
		}

		virtual MeshNormalMaterial& copy(const MeshNormalMaterial& source) {
			Material::copy(source);
			bumpMap = source.bumpMap;
			bumpScale = source.bumpScale;

			normalMap = source.normalMap;
			normalMapType = source.normalMapType;
			normalScale.copy(source.normalScale);

			displacementMap = source.displacementMap;
			displacementScale = source.displacementScale;
			displacementBias = source.displacementBias;

			wireframe = source.wireframe;
			wireframeLinewidth = source.wireframeLinewidth;

			skinning = source.skinning;
			morphTargets = source.morphTargets;
			morphNormals = source.morphNormals;

			return *this;
		}
	};
}

#endif