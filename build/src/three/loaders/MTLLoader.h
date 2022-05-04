#ifndef MTL_LOADER_H
#define MTL_LOADER_H
#include <limits>
#include <vector>
#include <string>
#include <unordered_map>

#include <filesystem>
#include <three/math/Color.h>
#include <three/math/Vector2.h>
#include <three/materials/Material.h>
#include <three/renderers/gl/GLProgram.h>
#include <three/textures/Texture.h>
using namespace std;
namespace three {
	using namespace math;

	
	class MTLLoader {
	public:
		enum class MTLName : unsigned {
			kd,
			ka,
			ks,
			ke,
			map_kd,
			map_ks,
			map_ke,
			norm,
			map_bump,
			bump,
			map_d,
			ns,
			d,
			tr
		};
		enum class MTLMapType : unsigned {
			map,
			specularMap,
			emissiveMap,
			normalMap,
			bumpMap,
			alphaMap
		};
		struct MTLParameters {
			string name;
			Side side;
			Color color;
			Color specular;
			Color emissive;			
			Texture::ptr map;
			Texture::ptr specularMap;
			Texture::ptr emissiveMap;
			Texture::ptr normalMap;
			Texture::ptr bumpMap;
			Texture::ptr alphaMap;
			float bumpScale=1.0f;
			float shininess=30.0f;
			float opacity=1.0f;
			bool transparent=false;
		};
		struct MaterialCreatorOptions {
			Side side = Side::None;
			Wrapping wrap = Wrapping::None;
			bool normalizedRGB = false;
			bool ignoreZeroRGBs = false;
			bool invertTrpproperty = false;

			bool isEmpty() {
				return (side==Side::None)|| (wrap== Wrapping::None);
			}

		};
		
		struct MaterialInfo {
			vector<float> Ks;
			vector<float> Kd;
			vector<float> Ke;
			float value[3];
			string stringValue;
			string map_kd;
			string map_ks;
			string map_ke;
			string norm;
			string map_bump;
			string bump;
			string map_d;
			int Ns = numeric_limits<int>::quiet_NaN();
			int D = numeric_limits<int>::quiet_NaN();
			int Tr = numeric_limits<int>::quiet_NaN();
		};

		struct TexParams {
			Vector2 scale;
			Vector2 offset;
			string url;
		};

		using MaterialsInfoMap = unordered_map<MTLName, MaterialInfo>;

		class MaterialCreator {
		public:
			using ptr = shared_ptr<MaterialCreator>;

			std::filesystem::path filePath;

			MaterialCreatorOptions options;

			unordered_map<string, MaterialsInfoMap> materialsInfo;

			unordered_map<string, Material::ptr> materials;

			vector<Material::ptr> materialsArray;

			unordered_map<string, string> nameLookUp;

			Side side;

			Wrapping wrap;

			string crossOrigin = "anonymous";

			MaterialCreator() {}

			~MaterialCreator() = default;

			MaterialCreator(string path, MaterialCreatorOptions options = MaterialCreatorOptions());			

			MaterialCreator& setCrossOrigin(string value);

			void setMaterials(unordered_map<string, MaterialsInfoMap>& materialsInfo); 

			unordered_map<string, MaterialsInfoMap> convert(unordered_map<string, MaterialsInfoMap>& materialsInfo); 
			
			void setMapForType(MTLParameters& parameter, MTLMapType mapType, const string& value);
			
			TexParams getTextureParams(const string& value, MTLParameters& matParams);

			Material::ptr create(const string& materialName);
			
			Material::ptr createMaterial(const string& materialName);

			Texture::ptr loadTexture(string filePath, TextureMapping mapping = TextureMapping::Unknown);
		};

		MaterialCreatorOptions materialOptions;

		string crossOrigin;

		MaterialCreator multiMaterialCreator;

		MTLLoader() {}

		~MTLLoader() = default;

		MaterialCreator::ptr parse(const string& filepath);

		MaterialCreator::ptr load(const string& filepath);
	};
}
#endif