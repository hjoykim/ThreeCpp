#ifndef LIGHTPROBE_THREE_H
#define LIGHTPROBE_THREE_H
#include <three/lights/Light.h>
namespace three {
	class LightProbe : public Light {
	public:
		using ptr = std::shared_ptr<LightProbe>;

		LightProbe() : Light() {
			type = "LightProbe";
			isLightProbe = true;
		}
		LightProbe(const Color& color, float intensity = 1) : Light(color, intensity) {
			type = "LightProbe";
			isLightProbe = true;
		}

		LightProbe(const LightProbe& source) : Light(source) {
			type = "LightProbe";
			isLightProbe = true;
			sh.copy(source.sh);
		}
		LightProbe(const SphericalHarmonics3& sh, float intensity = 1.0f) : Light(Color(ColorKeywords::white), intensity) {
			type = "LightProbe";
			isLightProbe = true;
			this->sh = sh;
		}

		virtual LightProbe& copy(const LightProbe& source) {
			Light::copy(source);
			sh.copy(source.sh);
			isLightProbe = true;
			return *this;
		}

		static ptr create() {
			return std::make_shared<LightProbe>();
		}

		static ptr create(const Color& color, float intensity = 1) {
			return std::make_shared<LightProbe>(color, intensity);
		}

		static ptr create(const SphericalHarmonics3& sh, float intensity = 1) {
			return std::make_shared<LightProbe>(sh, intensity);
		}


	};

	class AmbientLightProbe : public LightProbe {
	public:
		using ptr = std::shared_ptr<AmbientLightProbe>;

		AmbientLightProbe() : LightProbe() {
			type = "AmbientLightProbe";
		}
		AmbientLightProbe(const Color& color, float intensity = 1) : LightProbe(color, intensity) {
			type = "AmbientLightProbe";
			sh.coefficients[0].set(color.r, color.g, color.b).multiplyScalar(2 * (float)sqrt(M_PI));
		}
		AmbientLightProbe(const SphericalHarmonics3& sh, float intensity = 1) : LightProbe(sh, intensity) {
			type = "AmbientLightProbe";
		}

		virtual AmbientLightProbe& copy(const AmbientLightProbe& source) {
			LightProbe::copy(source);
			return *this;
		}

		static ptr create() {
			return std::make_shared<AmbientLightProbe>();
		}

		static ptr create(const Color& color, float intensity = 1) {
			return std::make_shared<AmbientLightProbe>(color, intensity);
		}

		static ptr create(const SphericalHarmonics3& sh, float intensity = 1) {
			return std::make_shared<AmbientLightProbe>(sh, intensity);
		}
	};

	class HemisphereLightProbe : public LightProbe {
	public :
		using ptr = std::shared_ptr<HemisphereLightProbe>;

		HemisphereLightProbe() : LightProbe() {
			type = "HemisphereLightProbe";
		}
		HemisphereLightProbe(const Color& skyColor,const Color& groundColor,float intensity=1.0f) : LightProbe(Color(0xffffff),intensity) {
			type = "HemisphereLightProbe";

			auto color1 = skyColor;
			auto color2 = groundColor;

			auto sky = Vector3(color1.r, color1.g, color1.b);
			auto ground = Vector3(color2.r, color2.g, color2.b);

			float c0 = (float)sqrt(M_PI);
			float c1 = c0 * sqrt(0.75f);


			sh.coefficients[0].copy(sky).add(ground).multiplyScalar(c0);
			sh.coefficients[1].copy(sky).add(ground).multiplyScalar(c1);
		}		

		virtual HemisphereLightProbe& copy(const HemisphereLightProbe& source) {
			LightProbe::copy(source);
			return *this;
		}

		static ptr create() {
			return std::make_shared<HemisphereLightProbe>();
		}

		static ptr create(Color skyColor, Color groundColor,float intensity = 1.0f) {
			return std::make_shared<HemisphereLightProbe>(skyColor, groundColor,intensity);
		}

		
	};
}
#endif