#ifndef GLLIGHTS_THREE_H
#define GLLIGHTS_THREE_H
#include <memory>
#include <vector>
#include <three/lights/Light.h>
#include <three/cameras/Camera.h>
#include <three/lights/LightShadow.h>
#include <three/textures/Texture.h>
namespace three {

	struct LightsHash {
		unsigned directionalLength = 0;
		unsigned pointLength = 0;
		unsigned spotLength = 0;
		unsigned rectAreaLength = 0;
		unsigned hemiLength = 0;
		unsigned shadowsLength = 0;
		unsigned numDirectionalShadows = 0;
		unsigned numPointShadows = 0;
		unsigned numSpotShadows = 0;

		LightsHash() {}

		LightsHash(unsigned directionalLength, unsigned pointLength,unsigned spotLength, unsigned rectAreaLength, unsigned hemiLength, unsigned shadowsLength,
			unsigned numDirectionalShadow,unsigned numPointShadows,unsigned numSpotShadows)
			: directionalLength(directionalLength), pointLength(pointLength), spotLength(spotLength),rectAreaLength(rectAreaLength), hemiLength(hemiLength), shadowsLength(shadowsLength),
			numDirectionalShadows(numDirectionalShadows),numPointShadows(numPointShadows),numSpotShadows(numSpotShadows){}

		bool operator ==(const LightsHash& other)
		{
			return directionalLength == other.directionalLength &&
				pointLength == other.pointLength &&
				spotLength == other.spotLength &&
				rectAreaLength == other.rectAreaLength &&
				hemiLength == other.hemiLength &&
				shadowsLength == other.shadowsLength &&
				numDirectionalShadows == other.numDirectionalShadows &&
				numPointShadows == other.numPointShadows &&
				numSpotShadows == other.numSpotShadows;
		}
		bool operator !=(const LightsHash& other)
		{
			return !this->operator==(other);
		}
	};

	class UniformsCache  {
	public:
		std::unordered_map<unsigned, Light::ptr> lights;

		UniformsCache() {}

		virtual ~UniformsCache() = default;

		Light::ptr& get(Light::ptr light) {
			DirectionalLight::ptr directionalLight;
			SpotLight::ptr spotLight;
			PointLight::ptr pointLight;
			HemisphereLight::ptr hemisphereLight;
			RectAreaLight::ptr rectAreaLight;

			if (lights.find(light->id) != lights.end()) {
				return lights[light->id];
			}
			else {
				switch (light->lightType) {
					case LightType::DirectionalLight :
						directionalLight = std::make_shared<DirectionalLight>();						
						lights[light->id] = directionalLight;
						break;
					case LightType::SpotLight:
						spotLight = std::make_shared<SpotLight>();
						spotLight->distance = 0;
						spotLight->coneCos = 0;
						spotLight->penumbra = 0;
						spotLight->decay = 0;
						lights[light->id] = spotLight;
						break;
					case LightType::PointLight :
						pointLight = std::make_shared<PointLight>();
						pointLight->distance = 0;
						pointLight->decay = 0;
						lights[light->id] = pointLight;
						break;
					case LightType::HemisphereLight :
						hemisphereLight = std::make_shared<HemisphereLight>();
						lights[light->id] = hemisphereLight;
						break;
					case LightType::RectAreaLight :
						rectAreaLight = std::make_shared<RectAreaLight>();
						lights[light->id] = rectAreaLight;
						break;
				}
				return lights[light->id];
			}
			
		}
	};
	class ShadowUniformsCache  {
	public:
		using ptr = std::shared_ptr<ShadowUniformsCache>;
		std::unordered_map<unsigned, LightShadow::ptr> shadows;

		ShadowUniformsCache(){}

		virtual ~ShadowUniformsCache() = default;

		LightShadow::ptr& get(Light::ptr light) {
			DirectionalLightShadow::ptr directionalLightShadow;
			SpotLightShadow::ptr spotLightShadow;
			PointLightShadow::ptr pointLightShadow;

			if (shadows.find(light->id) != shadows.end()) {
				return shadows[light->id];
			}
			else {
				switch (light->lightType) {
				case LightType::DirectionalLight:
					directionalLightShadow = std::make_shared<DirectionalLightShadow>();
					directionalLightShadow->shadowBias = 0;
					directionalLightShadow->shadowNormalBias = 0;
					directionalLightShadow->shadowRadius = 1;
					shadows[light->id] = directionalLightShadow;
					break;
				case LightType::SpotLight:
					spotLightShadow = std::make_shared<SpotLightShadow>();
					spotLightShadow->shadowBias = 0;
					spotLightShadow->shadowNormalBias = 0;
					spotLightShadow->shadowRadius = 1;
					shadows[light->id] = spotLightShadow;
					break;
				case LightType::PointLight:
					pointLightShadow = std::make_shared<PointLightShadow>();
					pointLightShadow->shadowBias = 0;
					pointLightShadow->shadowNormalBias = 0;
					pointLightShadow->shadowCameraNear = 1;
					pointLightShadow->shadowCameraFar = 1000;
					shadows[light->id] = pointLightShadow;
					break;
			
				}
				return shadows[light->id];
			}
		}
	};
	struct GLLightsState {
		int version;
		LightsHash hash;
		Color ambient;
		std::vector<Vector3> probe;

		std::vector<DirectionalLight::ptr> directional;
		std::vector<DirectionalLightShadow::ptr> directionalShadow;
		std::vector<Texture::ptr> directionalShadowMap;
		std::vector<Matrix4> directionalShadowMatrix;

		std::vector<SpotLight::ptr> spot;
		std::vector<SpotLightShadow::ptr> spotShadow;
		std::vector<Texture::ptr> spotShadowMap;
		std::vector<Matrix4> spotShadowMatrix;

		std::vector<RectAreaLight::ptr> rectArea;

		std::vector<PointLight::ptr> point;
		std::vector<PointLightShadow::ptr> pointShadow;
		std::vector<Texture::ptr> pointShadowMap;
		std::vector<Matrix4> pointShadowMatrix;

		std::vector<HemisphereLight::ptr> hemi;
	};

	class GLLights {
	private:
		unsigned nextVersion = 0;
		Vector3 vector3;

	public:
		
		UniformsCache cache;		

		ShadowUniformsCache shadowCache;

		GLLightsState state;

		GLLights();

		virtual ~GLLights() = default;

		void setup(std::vector<Light::ptr>& lights, const Camera::ptr& camera);		

	};
	inline bool shadowCastingLightsFirst(const Light::ptr& lightA, const Light::ptr& lightB) {
		int result = (lightB->castShadow ? 1 : 0) - (lightA->castShadow ? 1 : 0);
		return result > 0 ? true : false;
	}
}

#endif