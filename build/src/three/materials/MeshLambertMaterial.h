#ifndef MESH_LAMBERT_MATERIAL_THREE_H
#define MESH_LAMBERT_MATERIAL_THREE_H
#include <three/math/Color.h>
#include <three/textures/Texture.h>
#include <string>
#include <three/materials/Material.h>
namespace three {
	class MeshLambertMaterial : public Material
	{
	public:

		MeshLambertMaterial() :Material()
		{
			type = "MeshLambertMaterial";
			name = "MeshLambertMaterial";

			shaderId = "lambert";

			color.set(0xffffff);
			

			map = nullptr;

			lightMap = nullptr;
			lightMapIntensity = 1.0f;

			aoMap = nullptr;
			aoMapIntensity = 1.0f;

			emissive.set(0x000000);
			emissiveIntensity = 1.0f;
			emissiveMap = nullptr;

			specularMap = nullptr;

			alphaMap = nullptr;

			envMap = nullptr;
			combine = Combine::MultiplyOperation;
			reflectivity = 1;
			refractionRatio = 0.98f;

			wireframe = false;
			wireframeLinewidth = 1;
			wireframeLinecap = "round";
			wireframeLinejoin = "round";

			skinning = false;
			morphTargets = false;
			morphNormals = false;
		}
		MeshLambertMaterial(const Color& color) : MeshLambertMaterial() {
			this->color =color;
		}
		MeshLambertMaterial(const MeshLambertMaterial& source) : Material(source) {
			shaderId = "lambert";
			color.copy(source.color);

			lightMap = source.lightMap;
			lightMapIntensity = source.lightMapIntensity;

			aoMap = source.aoMap;
			aoMapIntensity = source.aoMapIntensity;

			emissive.copy(source.emissive);
			emissiveMap = source.emissiveMap;
			emissiveIntensity = source.emissiveIntensity;

			specularMap = source.specularMap;

			alphaMap = source.alphaMap;

			envMap = source.envMap;
			combine = source.combine;
			reflectivity = source.reflectivity;
			refractionRatio = source.refractionRatio;

			wireframe = source.wireframe;
			wireframeLinewidth = source.wireframeLinewidth;
			wireframeLinecap = source.wireframeLinecap;
			wireframeLinejoin = source.wireframeLinejoin;

			skinning = source.skinning;
			morphTargets = source.morphTargets;
			morphNormals = source.morphNormals;
		}
		virtual ~MeshLambertMaterial() = default;

		static ptr create() {
			return std::make_shared<MeshLambertMaterial>();
		}

		static ptr create(Color color) {
			return std::make_shared<MeshLambertMaterial>(color);
		}

		MeshLambertMaterial* clone() {
			return new MeshLambertMaterial(*this);
		}

		MeshLambertMaterial& copy(const MeshLambertMaterial& source)
		{
			Material::copy(source);
			color.copy(source.color);
			

			lightMapIntensity = source.lightMapIntensity;

			aoMapIntensity = source.aoMapIntensity;

			emissive.copy(source.emissive);

			emissiveIntensity = source.emissiveIntensity;



			combine = source.combine;
			reflectivity = source.reflectivity;
			refractionRatio = source.refractionRatio;

			wireframe = source.wireframe;
			wireframeLinewidth = source.wireframeLinewidth;
			wireframeLinecap = source.wireframeLinecap;
			wireframeLinejoin = source.wireframeLinejoin;

			skinning = source.skinning;
			morphTargets = source.morphTargets;
			morphNormals = source.morphNormals;

			return *this;
		}
	};
}
#endif