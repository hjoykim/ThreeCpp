#include "GLMaterials.h"
#include <three/Materials.h>
namespace three {
	namespace gl {
		void GLMaterials::refreshMaterialUniforms(UniformValues& m_uniforms, const Material::ptr& material, float pixelRatio, float height)
		{

			if (material->type == "MeshBasicMaterial") {
				refreshUniformsCommon(m_uniforms, *material);
			}
			else if (material->type == "MeshLambertMaterial") {
				refreshUniformsCommon(m_uniforms, *material);
				refreshUniformsLambert(m_uniforms, *material);
			}
			else if (material->type == "MeshToonMaterial") {
				refreshUniformsCommon(m_uniforms, *material);
				refreshUniformsToon(m_uniforms, *material);
			}
			else if (material->type == "MeshPhongMaterial") {
				refreshUniformsCommon(m_uniforms, *material);
				refreshUniformsPhong(m_uniforms, *material);
			}
			else if (instanceOf<MeshStandardMaterial>(material.get())) {
				refreshUniformsCommon(m_uniforms, *material);
				if (material->type == "MeshPhysicalMaterial") {
					refreshUniformsPhysical(m_uniforms, *material);
				}
				else {
					refreshUniformsStandard(m_uniforms, *material);
				}
			}
			else if (material->type == "MeshMatcapMaterial") {
				refreshUniformsCommon(m_uniforms, *material);
				refreshUniformsMatcap(m_uniforms, *material);
			}
			else if (material->type == "MeshDepthMaterial") {
				refreshUniformsCommon(m_uniforms, *material);
				refreshUniformsDepth(m_uniforms, *material);
			}
			else if (material->type == "MeshDistanceMaterial") {

				refreshUniformsCommon(m_uniforms, *material);
				refreshUniformsDistance(m_uniforms, *material);

			}
			else if (material->type == "MeshNormalMaterial") {

				refreshUniformsCommon(m_uniforms, *material);
				refreshUniformsNormal(m_uniforms, *material);

			}
			else if (instanceOf<LineBasicMaterial>(material.get())) {

				refreshUniformsLine(m_uniforms, *material);

				if (material->type == "LineDashedMaterial") {
					refreshUniformsDash(m_uniforms, *material);
				}
			}
			else if (material->type == "PointsMaterial") {

				refreshUniformsPoints(m_uniforms, *material,pixelRatio,height);

			}
			else if (material->type == "SpriteMaterial") {

				refreshUniformsSprites(m_uniforms, *material);

			}
			else if (material->type == "ShadowMaterial") {

				m_uniforms.set("color", material->color);
				m_uniforms.set("opacity", material->opacity);

			}
			else if (instanceOf<ShaderMaterial>(material.get())) {
				material->uniformsNeedUpdate = false;
			}
		}
		void GLMaterials::refreshUniformsFog(UniformValues& uniforms, Fog& fog)
		{
			uniforms.set("fogColor", fog.color);
			if (fog.type == "Fog") {
				uniforms.set("fogNear", fog._near);
				uniforms.set("fogFar", fog._far);
			}
			else if (fog.type == "FogExp2") {
				FogExp2* fogExp2 = dynamic_cast<FogExp2*>(&fog);
				uniforms.set("fogDensity", fogExp2->density);
			}
		}
		void GLMaterials::refreshUniformsCommon(UniformValues& uniforms, Material& material)
		{
			uniforms.set("opacity", material.opacity);
			if (!material.color.isNull()) {
				uniforms.set("diffuse", material.color);
			}

			if (!material.emissive.isNull()) {
				Color emissiveColor = material.emissive;
				uniforms.set("emissive", emissiveColor.multiplyScalar(material.emissiveIntensity));
			}

			if (material.map != nullptr) {
				uniforms.set("map", material.map);
			}
			if (material.alphaMap != nullptr) {
				uniforms.set("alphaMap", material.alphaMap);
			}
			if (material.specularMap != nullptr) {
				uniforms.set("specularMap", material.specularMap);
			}

			if (material.envMap != nullptr) {
				uniforms.set("envMap", material.envMap);
				uniforms.set("flipEnvMap", material.envMap->isCubeTexture ? -1 : 1);
				uniforms.set("reflectivity", material.reflectivity);
				uniforms.set("refractionRatio", material.refractionRatio);
				unsigned maxMipLevel = properties->getProperties(material.envMap->uuid).maxMipLevel;
				uniforms.set("maxMipLevel", maxMipLevel);
			}
			if (material.lightMap != nullptr) {
				uniforms.set("lightMap", material.lightMap);
				uniforms.set("lightMapIntensity", material.lightMapIntensity);
			}
			if (material.aoMap != nullptr) {
				uniforms.set("aoMap", material.aoMap);
				uniforms.set("aoMapIntensity", material.aoMapIntensity);
			}

			Texture::ptr uvScaleMap = nullptr;

			if (material.map != nullptr) {
				uvScaleMap = material.map;
			}
			else if (material.specularMap != nullptr) {
				uvScaleMap = material.specularMap;
			}
			else if (material.displacementMap != nullptr) {
				uvScaleMap = material.displacementMap;
			}
			else if (material.normalMap != nullptr) {
				uvScaleMap = material.normalMap;
			}
			else if (material.bumpMap != nullptr) {
				uvScaleMap = material.bumpMap;
			}
			else if (material.roughnessMap != nullptr) {
				uvScaleMap = material.roughnessMap;
			}
			else if (material.metalnessMap != nullptr) {
				uvScaleMap = material.metalnessMap;
			}
			else if (material.alphaMap != nullptr) {
				uvScaleMap = material.alphaMap;
			}
			else if (material.emissiveMap != nullptr) {
				uvScaleMap = material.emissiveMap;
			}

			if (uvScaleMap != nullptr) {
				if (instanceOf<GLRenderTarget>(uvScaleMap.get())) {
					GLRenderTarget::ptr temp = std::dynamic_pointer_cast<GLRenderTarget>(uvScaleMap);
					uvScaleMap = temp->texture;
				}
				if (uvScaleMap->matrixAutoUpdate) {
					uvScaleMap->updateMatrix();
				}
				uniforms.set("uvTransform", uvScaleMap->matrix);
			}

			// uv repeat and offset setting priorities for uv2
			// 1. ao map
			// 2. light map
			Texture::ptr uv2ScaleMap = nullptr;
			if (material.aoMap != nullptr) {
				uv2ScaleMap = material.aoMap;
			}
			else if (material.lightMap != nullptr) {
				uv2ScaleMap = material.lightMap;
			}

			if (uv2ScaleMap != nullptr) {
				if (instanceOf<GLRenderTarget>(uv2ScaleMap.get())) {
					GLRenderTarget::ptr temp = std::dynamic_pointer_cast<GLRenderTarget>(uv2ScaleMap);
					uv2ScaleMap = temp->texture;
				}
				if (uv2ScaleMap->matrixAutoUpdate) {
					uv2ScaleMap->updateMatrix();
				}
				uniforms.set("uv2Transform", uv2ScaleMap->matrix);
			}

		}
		void GLMaterials::refreshUniformsLambert(UniformValues& uniforms, Material& material)
		{
			if (material.emissiveMap != nullptr) {
				uniforms.set("emissiveMap", material.emissiveMap);
			}
		}
		void GLMaterials::refreshUniformsToon(UniformValues& uniforms, Material& material)
		{
			uniforms.set("specular", material.specular);
			uniforms.set("shininess", std::max(material.shininess, (float)1e-4)); // to prevent pow( 0.0, 0.0 )

			if (material.gradientMap != nullptr) {

				uniforms.set("gradientMap", material.gradientMap);

			}

			if (material.emissiveMap != nullptr) {

				uniforms.set("emissiveMap", material.emissiveMap);

			}

			if (material.bumpMap != nullptr) {

				uniforms.set("bumpMap", material.bumpMap);
				uniforms.set("bumpScale", material.bumpScale);
				if (material.side == Side::BackSide) {
					float scale = uniforms["bumpScale"].get<float>();
					//float scale = uniforms.set("bumpScale")->get<float>("value");
					scale *= -1;
					uniforms.set("bumpScale", scale);
				}
			}

			if (material.normalMap != nullptr) {

				uniforms.set("normalMap", material.normalMap);
				uniforms.set("normalScale", material.normalScale);
				if (material.side == Side::BackSide) {
					Vector2 normalScale = uniforms["normalScale"].get<Vector2>();
					normalScale.negate();
					uniforms.set("normalScale", normalScale);
				}

			}

			if (material.displacementMap != nullptr) {

				uniforms.set("displacementMap", material.displacementMap);
				uniforms.set("displacementScale", material.displacementScale);
				uniforms.set("displacementBias", material.displacementBias);

			}
		}
		void GLMaterials::refreshUniformsPhong(UniformValues& uniforms, Material& material)
		{
			uniforms.set("specular", material.specular);
			uniforms.set("shininess", std::max(material.shininess, (float)1e-4)); // to prevent pow( 0.0, 0.0 )

			if (material.emissiveMap != nullptr) {

				uniforms.set("emissiveMap", material.emissiveMap);

			}

			if (material.bumpMap != nullptr) {

				uniforms.set("bumpMap", material.bumpMap);
				uniforms.set("bumpScale", material.bumpScale);
				if (material.side == Side::BackSide) {
					float bumpScale = uniforms["bumpScale"].get<float>();
					bumpScale *= -1;
					uniforms.set("bumpScale", bumpScale);
				}

			}

			if (material.normalMap != nullptr) {

				uniforms.set("normalMap", material.normalMap);
				uniforms.set("normalScale", material.normalScale);
				if (material.side == Side::BackSide) {
					Vector2 normalScale = uniforms["normalScale"].get<Vector2>();
					normalScale.negate();
					uniforms.set("normalScale", normalScale);
				}
			}

			if (material.displacementMap) {

				uniforms.set("displacementMap", material.displacementMap);
				uniforms.set("displacementScale", material.displacementScale);
				uniforms.set("displacementBias", material.displacementBias);

			}
		}
		void GLMaterials::refreshUniformsPhysical(UniformValues& uniforms, Material& material)
		{
			refreshUniformsStandard(uniforms, material);

			uniforms.set("reflectivity", material.reflectivity); // also part of uniforms common

			uniforms.set("clearcoat", material.clearcoat);
			uniforms.set("clearcoatRoughness", material.clearcoatRoughness);
			if (material.sheen.r != -1) uniforms.set("sheen", material.sheen);

			if (material.clearcoatMap != nullptr) {

				uniforms.set("clearcoatMap", material.clearcoatMap);

			}

			if (material.clearcoatRoughnessMap != nullptr) {

				uniforms.set("clearcoatRoughnessMap", material.clearcoatRoughnessMap);

			}

			if (material.clearcoatNormalMap != nullptr) {

				uniforms.set("clearcoatNormalScale", material.clearcoatNormalScale);
				uniforms.set("clearcoatNormalMap", material.clearcoatNormalMap);

				if (material.side == Side::BackSide) {
					Vector2 clearcoatNormalScale = uniforms["clearcoatNormalScale"].get<Vector2>();
					clearcoatNormalScale.negate();
					uniforms.set("clearcoatNormalScale", clearcoatNormalScale);

				}

			}

			uniforms.set("transparency", material.transparency);
		}

		void GLMaterials::refreshUniformsStandard(UniformValues& uniforms, Material& material)
		{
			uniforms.set("roughness", material.roughness);
			uniforms.set("metalness", material.metalness);

			if (material.roughnessMap != nullptr) {

				uniforms.set("roughnessMap", material.roughnessMap);

			}

			if (material.metalnessMap != nullptr) {

				uniforms.set("metalnessMap", material.metalnessMap);

			}

			if (material.emissiveMap != nullptr) {

				uniforms.set("emissiveMap", material.emissiveMap);

			}

			if (material.bumpMap != nullptr) {

				uniforms.set("bumpMap", material.bumpMap);
				uniforms.set("bumpScale", material.bumpScale);
				if (material.side == Side::BackSide) {
					float bumpScale = uniforms["bumpScale"].get<float>();
					bumpScale *= -1;
					uniforms.set("bumpScale", bumpScale);
				}

			}

			if (material.normalMap) {

				uniforms.set("normalMap", material.normalMap);
				uniforms.set("normalScale", material.normalScale);
				if (material.side == Side::BackSide) {
					Vector2 normalScale = uniforms["normalScale"].get<Vector2>();
					normalScale.negate();
					uniforms.set("normalScale", normalScale);
				}

			}

			if (material.displacementMap) {

				uniforms.set("displacementMap", material.displacementMap);
				uniforms.set("displacementScale", material.displacementScale);
				uniforms.set("displacementBias", material.displacementBias);

			}

			if (material.envMap != nullptr) {

				//uniforms.envMap.value = material.envMap; // part of uniforms common
				uniforms.set("envMapIntensity", material.envMapIntensity);

			}
		}
		void GLMaterials::refreshUniformsMatcap(UniformValues& uniforms, Material& material)
		{
			if (material.matcap != nullptr) {

				uniforms.set("matcap", material.matcap);

			}

			if (material.bumpMap != nullptr) {

				uniforms.set("bumpMap", material.bumpMap);
				uniforms.set("bumpScale", material.bumpScale);
				if (material.side == Side::BackSide) {
					float bumpScale = uniforms["bumpScale"].get<float>();// *= -1;
					bumpScale *= -1;
					uniforms.set("bumpScale", bumpScale);
				}

			}

			if (material.normalMap != nullptr) {

				uniforms.set("normalMap", material.normalMap);
				uniforms.set("normalScale", material.normalScale);
				if (material.side == Side::BackSide) {
					Vector2 normalScale = uniforms["normalScale"].get<Vector2>();
					normalScale.negate();
					uniforms.set("normalScale", normalScale);
				}

			}

			if (material.displacementMap != nullptr) {

				uniforms.set("displacementMap", material.displacementMap);
				uniforms.set("displacementScale", material.displacementScale);
				uniforms.set("displacementBias", material.displacementBias);

			}
		}
		void GLMaterials::refreshUniformsDepth(UniformValues& uniforms, Material& material)
		{
			if (material.displacementMap != nullptr) {

				uniforms.set("displacementMap", material.displacementMap);
				uniforms.set("displacementScale", material.displacementScale);
				uniforms.set("displacementBias", material.displacementBias);

			}
		}
		void GLMaterials::refreshUniformsDistance(UniformValues& uniforms, Material& material)
		{
			if (material.displacementMap != nullptr) {

				uniforms.set("displacementMap", material.displacementMap);
				uniforms.set("displacementScale", material.displacementScale);
				uniforms.set("displacementBias", material.displacementBias);

			}

			uniforms.set("referencePosition", material.referencePosition);
			uniforms.set("nearDistance", material.nearDistance);
			uniforms.set("farDistance", material.farDistance);
		}
		void GLMaterials::refreshUniformsNormal(UniformValues& uniforms, Material& material)
		{
			if (material.bumpMap != nullptr) {

				uniforms.set("bumpMap", material.bumpMap);
				uniforms.set("bumpScale", material.bumpScale);
				if (material.side == Side::BackSide) {
					float bumpScale = uniforms["bumpScale"].get<float>();
					bumpScale *= -1;
					uniforms.set("bumpScale", bumpScale);
				}
			}

			if (material.normalMap != nullptr) {

				uniforms.set("normalMap", material.normalMap);
				uniforms.set("normalScale", material.normalScale);
				if (material.side == Side::BackSide) {
					Vector2 normalScale = uniforms["normalScale"].get<Vector2>();
					normalScale.negate();
					uniforms.set("normalScale", normalScale);
				}

			}

			if (material.displacementMap != nullptr) {

				uniforms.set("displacementMap", material.displacementMap);
				uniforms.set("displacementScale", material.displacementScale);
				uniforms.set("displacementBias", material.displacementBias);

			}
		}
		void GLMaterials::refreshUniformsLine(UniformValues& uniforms, Material& material)
		{
			uniforms.set("diffuse", material.color);
			uniforms.set("opacity", material.opacity);
		}
		void GLMaterials::refreshUniformsDash(UniformValues& uniforms, Material& material)
		{
			uniforms.set("dashSize", material.dashSize);
			uniforms.set("totalSize", material.dashSize + material.gapSize);
			uniforms.set("scale", material.scale);
		}
		void GLMaterials::refreshUniformsPoints(UniformValues& uniforms, Material& material, float _pixelRatio, float _height)
		{
			uniforms.set("diffuse", material.color);
			uniforms.set("opacity", material.opacity);
			uniforms.set("size", material.size * _pixelRatio);
			uniforms.set("scale", _height * 0.5f);

			if (material.map) {

				uniforms.set("map", material.map);

			}

			if (material.alphaMap) {

				uniforms.set("alphaMap", material.alphaMap);

			}

			// uv repeat and offset setting priorities
			// 1. color map
			// 2. alpha map

			Texture::ptr uvScaleMap;

			if (material.map != nullptr) {

				uvScaleMap = material.map;

			}
			else if (material.alphaMap != nullptr) {

				uvScaleMap = material.alphaMap;

			}

			if (uvScaleMap != nullptr) {

				if (uvScaleMap->matrixAutoUpdate == true) {

					uvScaleMap->updateMatrix();

				}

				uniforms.set("uvTransform", uvScaleMap->matrix);

			}
		}
		void GLMaterials::refreshUniformsSprites(UniformValues& uniforms, Material& material)
		{
			uniforms.set("diffuse", material.color);
			uniforms.set("opacity", material.opacity);
			uniforms.set("rotation", material.rotation);

			if (material.map != nullptr) {

				uniforms.set("map", material.map);

			}

			if (material.alphaMap != nullptr) {

				uniforms.set("alphaMap", material.alphaMap);

			}

			// uv repeat and offset setting priorities
			// 1. color map
			// 2. alpha map

			Texture::ptr uvScaleMap;

			if (material.map != nullptr) {

				uvScaleMap = material.map;

			}
			else if (material.alphaMap != nullptr) {

				uvScaleMap = material.alphaMap;

			}

			if (uvScaleMap != nullptr) {

				if (uvScaleMap->matrixAutoUpdate == true) {

					uvScaleMap->updateMatrix();

				}

				uniforms.set("uvTransform", uvScaleMap->matrix);

			}
		}
	}
}
