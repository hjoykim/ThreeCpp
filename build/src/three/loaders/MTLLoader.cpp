#include "MTLLoader.h"
#include <stdlib.h>
#include <pcrecpp.h>
#include <three/loaders/TextureLoader.h>
#include <three/materials/MeshPhongMaterial.h>
namespace three {
	MTLLoader::MaterialCreator::MaterialCreator(string path, MaterialCreatorOptions options)
		:options(options) {
		filePath = std::filesystem::path(path).parent_path();
		this->side = options.isEmpty() ? Side::FrontSide : options.side;
		this->wrap = options.isEmpty() ? Wrapping::RepeatWrapping : options.wrap;
	}
	MTLLoader::MaterialCreator& MTLLoader::MaterialCreator::setCrossOrigin(string value)
	{
		this->crossOrigin = value;
		return *this;
	}
	void MTLLoader::MaterialCreator::setMaterials(unordered_map<string, MaterialsInfoMap>& materialsInfo)
	{
		this->materialsInfo = materialsInfo;
	}
	unordered_map<string, MTLLoader::MaterialsInfoMap> MTLLoader::MaterialCreator::convert(unordered_map<string, MaterialsInfoMap>& materialsInfo)
	{
		if (this->options.isEmpty()) return materialsInfo;

		unordered_map<string, MaterialsInfoMap> converted;

		for (auto key = materialsInfo.begin();key != materialsInfo.end();) {
			MaterialsInfoMap mat = key->second;

			converted.insert({ key->first, MaterialsInfoMap() });

			MaterialsInfoMap& covmat = converted[key->first];

			MaterialInfo convertedMaterialInfo;

			for (auto key1 = mat.begin();key1 != mat.end();) {
				bool save = true;
				auto& value = mat[key1->first].value;
				MTLName lprop = key1->first;
			
				if (lprop == MTLName::kd || lprop == MTLName::ka || lprop == MTLName::ks) {
					if (!this->options.isEmpty() && this->options.normalizedRGB) {
						value[0] = value[0] / 255.0f;
						value[1] = value[1] / 255.0f;
						value[2] = value[2] / 255.0f;
					}
					if (!this->options.isEmpty() && this->options.ignoreZeroRGBs) {
						if (value[0] == 0 && value[1] == 0 && value[2] == 0) {
							save = false;
						}
					}
				}
				if (save) {
					std::memcpy(convertedMaterialInfo.value, value, 3);
					
					covmat.insert({ lprop,convertedMaterialInfo });
				}
				key1++;
			}
			key++;
		}
		return converted;
	}
	void MTLLoader::MaterialCreator::setMapForType(MTLParameters& parameter, MTLMapType mapType, const string& value)
	{
		auto texParams = getTextureParams(value, parameter);
		auto map = loadTexture(filePath.string() + "/" + texParams.url,TextureMapping::Unknown);

		map->repeat.copy(texParams.scale);
		map->offset.copy(texParams.offset);

		map->wrapS = this->wrap;
		map->wrapT = this->wrap;

		switch (mapType) {
		case MTLMapType::map: parameter.map = map;break;
		case MTLMapType::specularMap: parameter.specularMap = map;break;
		case MTLMapType::emissiveMap: parameter.emissiveMap = map;break;
		case MTLMapType::normalMap: parameter.normalMap = map;break;
		case MTLMapType::bumpMap: parameter.bumpMap = map;break;
		case MTLMapType::alphaMap: parameter.alphaMap = map;break;
		}
	}
	MTLLoader::TexParams MTLLoader::MaterialCreator::getTextureParams(const string& value, MTLParameters& matParams)
	{
		TexParams texParams;
		texParams.scale.set(1, 1);
		texParams.offset.set(0, 0);

		string pattern = R"(\s+)";
		pcrecpp::RE re(pattern);
		pcrecpp::StringPiece input(value);
		std::string match;
		std::string localSource = value;
		int pos = -1;
		vector<string> items;
		while (re.FindAndConsume(&input, &match)) {
			items.push_back(match);
		}
		for (int i = 0;i < items.size();i++) {
			if (items[i].find("-bm", 0) != string::npos) {
				pos = i;
				break;
			}
		}

		if (pos >= 0) {
			char* pEnd;
			matParams.bumpScale = strtof(items[pos + 1].c_str(), &pEnd);
			vector<string>::iterator iter = items.begin();
			int start = pos;
			int end = pos + 2;
			items.erase(iter + start - 1, iter + end - 1);
		}
		for (int i = 0;i < items.size();i++) {
			if (items[i].find("-s", 0) != string::npos) {
				pos = i;
				break;
			}
			pos = -1;
		}

		if (pos >= 0) {
			char* pEnd;
			float x = strtof(items[pos + 1].c_str(), &pEnd);
			float y = strtof(items[pos + 2].c_str(), &pEnd);
			texParams.scale.set(x, y);
			vector<string>::iterator iter = items.begin();
			int start = pos;
			int end = pos + 4;
			items.erase(iter + start - 1, iter + end - 1);
		}

		for (int i = 0;i < items.size();i++) {
			if (items[i].find("-o", 0) != string::npos) {
				pos = i;
				break;
			}
			pos = -1;
		}

		if (pos >= 0) {
			char* pEnd;
			float x = strtof(items[pos + 1].c_str(), &pEnd);
			float y = strtof(items[pos + 2].c_str(), &pEnd);
			texParams.offset.set(x, y);
			vector<string>::iterator iter = items.begin();
			int start = pos;
			int end = pos + 4;
			items.erase(iter + start - 1, iter + end - 1);
		}
		std::string s;
		for (const auto& piece : items) s += piece;
		
		texParams.url = s;

		return texParams;
	}
	Material::ptr MTLLoader::MaterialCreator::create(const string& materialName)
	{
		if (this->materials.count(materialName) == 0) {
			createMaterial(materialName);
		}

		return materials[materialName];
	}
	Material::ptr MTLLoader::MaterialCreator::createMaterial(const string& materialName)
	{
		auto mat = materialsInfo[materialName];

		MTLParameters parameter;
		parameter.name = materialName;
		parameter.side = this->side;
		float n;
		for (auto key = mat.begin();key != mat.end();) {
			auto materialInfo = key->second;
			//if (materialInfo.isEmpty()) continue;

			switch (key->first) {
			case MTLName::kd:
				parameter.color.fromArray(materialInfo.value, 3);
				break;
			case MTLName::ks:
				parameter.specular.fromArray(materialInfo.value, 3);
				break;
			case MTLName::ke:
				parameter.emissive.fromArray(materialInfo.value, 3);
				break;
			case MTLName::map_kd:
				setMapForType(parameter, MTLMapType::map, materialInfo.stringValue);
				break;
			case MTLName::map_ks:
				setMapForType(parameter, MTLMapType::specularMap, materialInfo.stringValue);
				break;
			case MTLName::map_ke:
				setMapForType(parameter, MTLMapType::emissiveMap, materialInfo.stringValue);
				break;
			case MTLName::norm:
				setMapForType(parameter, MTLMapType::normalMap, materialInfo.stringValue);
				break;
			case MTLName::map_bump:
				setMapForType(parameter, MTLMapType::bumpMap, materialInfo.stringValue);
				break;
			case MTLName::map_d:
				setMapForType(parameter, MTLMapType::alphaMap, materialInfo.stringValue);
				break;
			case MTLName::ns:
				parameter.shininess = atof(materialInfo.stringValue.c_str());
				break;
			case MTLName::d:
				n = atof(materialInfo.stringValue.c_str());
				
				if (n < 1) {
					parameter.opacity = n;
					parameter.transparent = true;
				}
				break;
			case MTLName::tr:
				n = atof(materialInfo.stringValue.c_str());
				if (this->options.invertTrpproperty) n = 1 - n;
				if (n > 0) {
					parameter.opacity = 1 - n;
					parameter.transparent = true;
				}
				break;
			default:
				break;
			}
			key++;
		}
		MeshPhongMaterial::ptr material = MeshPhongMaterial::create();
		material->name = parameter.name;
		material->side = parameter.side;
		material->color = parameter.color;
		material->specular = parameter.specular;
		material->emissive = parameter.emissive;
		material->map = parameter.map;
		material->specularMap = parameter.specularMap;
		material->emissiveMap = parameter.emissiveMap;
		material->normalMap = parameter.normalMap;
		material->bumpMap = parameter.bumpMap;
		material->alphaMap = parameter.alphaMap;
		material->bumpScale = parameter.bumpScale;
		material->shininess = parameter.shininess;
		material->opacity = parameter.opacity;
		material->transparent = parameter.transparent;

		this->materials[materialName] = material;

		return material;
	}
	Texture::ptr MTLLoader::MaterialCreator::loadTexture(string filePath, TextureMapping mapping)
	{
		Texture::ptr texture = TextureLoader::load((char*)filePath.c_str());
		if (mapping != TextureMapping::Unknown) texture->mapping = mapping;

		return texture;
	}
	MTLLoader::MaterialCreator::ptr MTLLoader::parse(const string& filepath) {
		return nullptr;
	}

	MTLLoader::MaterialCreator::ptr MTLLoader::load(const string& filepath) {
		return nullptr;
	}
}
