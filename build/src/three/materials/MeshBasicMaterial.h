#ifndef MESH_BASIC_MATERIAL_THREE_H
#define MESH_BASIC_MATERIAL_THREE_H
#include <three/materials/Material.h>
namespace three {
	class MeshBasicMaterial : public Material {
	public:
		MeshBasicMaterial() : Material()
		{
			type = "MeshBasicMaterial";

			name = "MeshBasicMaterial";

			shaderId = "basic";

			color.set(0xffffff);

			//map = null;

			//lightMap = null;

			lightMapIntensity = 1.0;

			//aoMap = null;

			aoMapIntensity = 1.0f;

			//specularMap = null;

			//alphaMap = null;

			//envMap = null;

			combine = Combine::MultiplyOperation;

			reflectivity = 1;

			refractionRatio = 0.98f;

			wireframe = false;

			wireframeLinewidth = 1;

			wireframeLinecap = "round";

			wireframeLinejoin = "round";

			skinning = false;

			morphTargets = false;
		}
		using ptr = std::shared_ptr<MeshBasicMaterial>;

		MeshBasicMaterial(const MeshBasicMaterial& source) :Material(source) {
			type = source.type;

			color = source.color;

			shaderId = "basic";

			//map = null;
			
			//lightMap = null;

			lightMapIntensity = source.lightMapIntensity;

			//aoMap = null;

			aoMapIntensity = source.aoMapIntensity;

			//specularMap = null;

			//alphaMap = null;

			//envMap = null;

			combine = source.combine;

			reflectivity = source.reflectivity;

			refractionRatio = source.refractionRatio;

			wireframe = source.wireframe;

			wireframeLinewidth = source.wireframeLinewidth;

			wireframeLinecap = source.wireframeLinecap;

			wireframeLinejoin = source.wireframeLinejoin;

			skinning = source.skinning;

			morphTargets = source.morphTargets;
		}
		MeshBasicMaterial(const Color& color) : MeshBasicMaterial() {
			this->color.copy(color);
		}

		static ptr create() {
			return std::make_shared<MeshBasicMaterial>();
		}

		static ptr create(const Color& color) {
			return std::make_shared<MeshBasicMaterial>(color);
		}

		virtual MeshBasicMaterial* clone()
		{
			return new MeshBasicMaterial(*this);
		}
		virtual MeshBasicMaterial& copy(const MeshBasicMaterial& source)
		{		
			Material::copy(source);
			
			type = source.type;

			color = source.color;

			//map = null;


			//lightMap = null;

			lightMapIntensity = source.lightMapIntensity;

			//aoMap = null;

			aoMapIntensity = source.aoMapIntensity;

			//specularMap = null;

			//alphaMap = null;

			//envMap = null;

			combine = source.combine;

			reflectivity = source.reflectivity;

			refractionRatio = source.refractionRatio;

			wireframe = source.wireframe;

			wireframeLinewidth = source.wireframeLinewidth;

			wireframeLinecap = source.wireframeLinecap;

			wireframeLinejoin = source.wireframeLinejoin;

			skinning = source.skinning;

			morphTargets = source.morphTargets;

			return *this;
		}


		virtual ~MeshBasicMaterial() = default;
	};
}
#endif