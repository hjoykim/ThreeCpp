#ifndef MESH_PHYSICAL_MATERIAL_THREE_H
#define MESH_PHYSICAL_MATERIAL_THREE_H
#include <string>
#include <three/textures/Texture.h>
#include <three/materials/MeshStandardMaterial.h>
#include <three/Constants.h>
#include <three/math/Vector2.h>
#include <three/math/Color.h>
namespace three {
	class MeshPhysicalMaterial : public MeshStandardMaterial {
	public:
		MeshPhysicalMaterial() :MeshStandardMaterial()
		{
			definesString.insert({"STANDARD", std::string() });
			definesString.insert({"PHYSICAL", std::string() });
			
			type = "MeshPhysicalMaterial";
			name = "MeshPhysicalMaterial";

			shaderId = "physical";

			clearcoat = 0.0f;
			//clearcoatMap = null;
			clearcoatRoughness = 0.0f;
			//clearcoatRoughnessMap = null;
			clearcoatNormalScale.set(1.0f, 1.0f);// = new Vector2(1, 1);
			//clearcoatNormalMap = null;

			reflectivity = 0.5f; // maps to F0 = 0.04

			sheen.set(0x000000);// = null; // null will disable sheen bsdf

			transparency = 0.0f;
		}

		MeshPhysicalMaterial(const MeshPhysicalMaterial& source) : MeshStandardMaterial(source) {
			definesString.insert({ "STANDARD", std::string() });
			definesString.insert({ "PHYSICAL", std::string() });

			clearcoat = source.clearcoat;
			clearcoatMap = source.clearcoatMap;
			clearcoatRoughness = source.clearcoatRoughness;
			clearcoatRoughnessMap = source.clearcoatRoughnessMap;
			clearcoatNormalMap = source.clearcoatNormalMap;
			clearcoatNormalScale.copy(source.clearcoatNormalScale);

			reflectivity = source.reflectivity;


			sheen.copy(source.sheen);


			transparency = source.transparency;
		}
		virtual ~MeshPhysicalMaterial() = default;

		virtual MeshPhysicalMaterial* clone()
		{
			return new MeshPhysicalMaterial(*this);
		}
		virtual MeshPhysicalMaterial& copy(const MeshPhysicalMaterial& source)
		{
			MeshStandardMaterial::copy(source);

			definesString.insert({ "STANDARD", std::string() });
			definesString.insert({ "PHYSICAL", std::string() });

			clearcoat = source.clearcoat;
			clearcoatMap = source.clearcoatMap;
			clearcoatRoughness = source.clearcoatRoughness;
			clearcoatRoughnessMap = source.clearcoatRoughnessMap;
			clearcoatNormalMap = source.clearcoatNormalMap;
			clearcoatNormalScale.copy(source.clearcoatNormalScale);

			reflectivity = source.reflectivity;


			sheen.copy(source.sheen);


			transparency = source.transparency;

			return *this;
		}
	};
}
#endif