#include "Light.h"
namespace three {
	Light::Light() : Object3D() {
		isLight = true;
		receiveShadow = false;
		type = "Light";
		lightType = LightType::Light;
	}
	Light::Light(const Color& color,float intensity) :Object3D(), color(color), intensity(intensity) {
		isLight = true;
		receiveShadow = false;
		type = "Light";
		lightType = LightType::Light;
	}

	Light::Light(const Light& source) : Light() {
		type = source.type;
		receiveShadow = source.receiveShadow;

		color.copy(source.color);

		groundColor.copy(source.groundColor);

		intensity = source.intensity;


		angle = source.angle;

		exponent = source.exponent;

		decay = source.decay;

		penumbra = source.penumbra;

		//LightShadow::ptr shadow = nullptr;

		//Object3D::ptr target;

		sh.copy(source.sh);

		width = source.width;

		height = source.height;

		direction.copy(source.direction);

		distance = source.distance;

		coneCos = source.coneCos;


		halfWidth.copy(source.halfWidth);

		halfHeight;

	}


	Light& Light::copy(const Light& source) {
		Object3D::copy(source);

		type = source.type;
		receiveShadow = source.receiveShadow;

		lightType = source.lightType;

		color.copy(source.color);

		groundColor.copy(source.groundColor);
		
		intensity = source.intensity;


		angle = source.angle;

		exponent = source.exponent;

		decay = source.decay;

		penumbra = source.penumbra;

		//LightShadow::ptr shadow = nullptr;

		//Object3D::ptr target;

		sh.copy(source.sh);

		width = source.width;

		height = source.height;

		direction.copy(source.direction);

		distance = source.distance;

		coneCos = source.coneCos;

		halfWidth.copy(source.halfWidth);

		halfHeight;

		return *this;
	}


	AmbientLight::AmbientLight() : Light() {

		type = "AmbientLight";
		lightType = LightType::AmbientLight;

	}

	AmbientLight::AmbientLight(const Color& color, float intensity) :Light(color, intensity) {
		type = "AmbientLight";
		lightType = LightType::AmbientLight;
	}

	AmbientLight::AmbientLight(const AmbientLight& source) : Light(source) {
		lightType = LightType::AmbientLight;
	}

	AmbientLight& AmbientLight::copy(const AmbientLight& source)
	{
		Light::copy(source);
		return *this;
	}

	DirectionalLight::DirectionalLight() : Light() {

		type = "DirectionalLight";
		position.copy(object3d::DefaultUp);
		target = Object3D::create();
		lightType = LightType::DirectionalLight;
		this->shadow = DirectionalLightShadow::create();
	}

	DirectionalLight::DirectionalLight(const Color& color, float intensity) :Light(color, intensity) {
		type = "DirectionalLight";
		position.copy(object3d::DefaultUp);
		target = Object3D::create();
		lightType = LightType::DirectionalLight;
		this->shadow = DirectionalLightShadow::create();
	}

	DirectionalLight::DirectionalLight(const DirectionalLight& source) : Light(source) {
		type = source.type;
		position.copy(source.position);
		target = Object3D::create();

		target->copy(*source.target);

		lightType = LightType::DirectionalLight;
		this->shadow = DirectionalLightShadow::create();
	}

	DirectionalLight& DirectionalLight::copy(const DirectionalLight& source)
	{
		Light::copy(source);

		return *this;
	}

	HemisphereLight::HemisphereLight() : Light() {
		type = "HemisphereLight";
		lightType = LightType::HemisphereLight;
		position.copy(object3d::DefaultUp);
		groundColor.set(ColorKeywords::white);
	}

	HemisphereLight::HemisphereLight(const Color& skyColor, const Color& groundColor, float intensity) : Light(skyColor,intensity) {

		type = "HemisphereLight";
		lightType = LightType::HemisphereLight;
		position.copy(object3d::DefaultUp);
		this->groundColor = groundColor;
	}
	HemisphereLight& HemisphereLight::copy(const HemisphereLight& source)
	{
		Light::copy(source);
		type = source.type;
		position.copy(source.position);
		groundColor.copy(source.groundColor);
		return *this;
	}

	PointLight::PointLight() : Light() {
		type = "PointLight";
		lightType = LightType::PointLight;
		distance = 0;
		decay = 1;
		this->shadow = PointLightShadow::create();
	}

	PointLight::PointLight(const Color& color, float intensity, float distance, float decay) : Light(color, intensity) {
		type = "PointLight";
		lightType = LightType::PointLight;
		this->distance = !std::isnan(distance) ? distance : 0;
		this->decay = !std::isnan(decay) ? decay : 1;
		this->shadow = PointLightShadow::create();
	}

	PointLight& PointLight::copy(const PointLight& source)
	{
		Light::copy(source);
		this->distance = source.distance;
		this->decay = source.decay;



		if (source.shadow != nullptr) {
			shadow.reset();
			shadow->copy(*source.shadow);
		}

		return *this;
	}

	RectAreaLight::RectAreaLight() : Light() {
		type = "RectAreaLight";
		lightType = LightType::RectAreaLight;
		width = 10;
		height = 10;
	}

	RectAreaLight::RectAreaLight(const Color& color, float intensity, float width, float height) : Light(color, intensity) {
		type = "RectAreaLight";
		lightType = LightType::RectAreaLight;
		this->width = !std::isnan(width) ? width : 10;
		this->height = !std::isnan(height) ? height : 10;
	}

	RectAreaLight& RectAreaLight::copy(const RectAreaLight& source)
	{
		Light::copy(source);
		this->width = source.width;
		this->height = source.height;

		return *this;
	}

	SpotLight::SpotLight() : Light() {
		type = "SpotLight";
		lightType = LightType::SpotLight;
		this->position.copy(object3d::DefaultUp);
		this->updateMatrix();
		this->target = std::make_shared<Object3D>();
		this->shadow = SpotLightShadow::create();

	}

	SpotLight::SpotLight(const Color& color, float intensity, float distance, float angle, float penumbra, float decay) : Light(color,intensity) {
		type = "SpotLight";
		lightType = LightType::SpotLight;
		this->position.copy(object3d::DefaultUp);
		this->updateMatrix();
		this->target = std::make_shared<Object3D>();
		this->shadow = SpotLightShadow::create();
		this->distance = !std::isnan(distance) ? distance : 0;
		this->angle = !std::isnan(angle) ? angle : (float)(M_PI / 3);
		this->penumbra = !std::isnan(penumbra) ? penumbra : 0;
		this->decay = !std::isnan(decay) ? decay : 1;

	}

	SpotLight& SpotLight::copy(const SpotLight& source) {
		Light::copy(source);


		this->shadow.reset();
		if (source.shadow != nullptr) {
			this->shadow = SpotLightShadow::create();
			this->shadow->copy(*source.shadow);
		}

		this->target.reset();
		if (source.target != nullptr)
			this->target = Object3D::ptr(source.target->clone());

		return *this;
	}

}
