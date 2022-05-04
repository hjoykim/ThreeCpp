#ifndef MESH_MATCAP_MATERIAL_THREE_H
#define MESH_MATCAP_MATERIAL_THREE_H
#include <string>
#include <three/materials/Material.h>
#include <three/Constants.h>
namespace three {
	class MeshMatcapMaterial : public Material {
	public:

		MeshMatcapMaterial() : Material() {

			definesString.insert({"MATCAP",std::string() });
			type = "MeshMatcapMaterial";
			name = "MeshMatcapMaterial";
			
			shaderId = "matcap";

			color.set(0xffffff);

			//matcap = null;

			//map = null;

			//bumpMap = null;
			bumpScale = 1;

			//normalMap = null;

			normalMapType = NormalMapTypes::TangentSpaceNormalMap;

			normalScale.set(1.0f, 1.0f); // new Vector2(1, 1);

			//displacementMap = null;

			displacementScale = 1;

			displacementBias = 0;

			//alphaMap = null;

			skinning = false;

			morphTargets = false;

			morphNormals = false;
		}
		MeshMatcapMaterial(const MeshMatcapMaterial& source) : Material(source) {
			definesString.insert({ "MATCAP",std::string() });

			color.copy(source.color);

			matcap = source.matcap;

			map = source.map;

			bumpMap = source.bumpMap;
			bumpScale = source.bumpScale;

			normalMap = source.normalMap;
			normalMapType = source.normalMapType;
			normalScale.copy(source.normalScale);

			displacementMap = source.displacementMap;
			displacementScale = source.displacementScale;
			displacementBias = source.displacementBias;

			alphaMap = source.alphaMap;

			skinning = source.skinning;
			morphTargets = source.morphTargets;
			morphNormals = source.morphNormals;
		}
		virtual ~MeshMatcapMaterial() = default;

		virtual MeshMatcapMaterial* clone() {
			return new MeshMatcapMaterial(*this);
		}
		virtual MeshMatcapMaterial& copy(const MeshMatcapMaterial& source) {
			Material::copy(source);
			definesString.insert({ "MATCAP",std::string() });

			color.copy(source.color);

			matcap = source.matcap;

			map = source.map;

			bumpMap = source.bumpMap;
			bumpScale = source.bumpScale;

			normalMap = source.normalMap;
			normalMapType = source.normalMapType;
			normalScale.copy(source.normalScale);

			displacementMap = source.displacementMap;
			displacementScale = source.displacementScale;
			displacementBias = source.displacementBias;

			alphaMap = source.alphaMap;

			skinning = source.skinning;
			morphTargets = source.morphTargets;
			morphNormals = source.morphNormals;

			return *this;
		}
	};
}
#endif