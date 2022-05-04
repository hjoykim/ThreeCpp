#ifndef LIGHT_THREE_H
#define LIGHT_THREE_H
#include <three/math/Color.h>
#include <three/math/SphericalHarmonics3.h>
#include <three/core/Object3D.h>
#include <three/lights/LightShadow.h>
using namespace three::math;
namespace three {
	enum class LightType : unsigned {
		Light=0,
		AmbientLight=1,
		DirectionalLight=2,
		SpotLight=3,
		PointLight=4,
		HemisphereLight=5,
		RectAreaLight=6
	};
	

	class Light : public Object3D {
	public:	

		using ptr = std::shared_ptr<Light>;
		using LightShadowPtr= std::shared_ptr<LightShadow>;
		Color color = Color(ColorKeywords::white);
		
		Color groundColor= Color(ColorKeywords::blue);

		Color skyColor;

		float intensity=1.0f;

		float distance=0.0f;

		float angle=0.0f;

		float exponent=0.0f;

		float decay=0.0f;

		float penumbra=0.0f;
				
		LightShadow::ptr shadow = nullptr;

		Object3D::ptr target;

		SphericalHarmonics3 sh;

		float width=1;

		float height=1;

		LightType lightType;

		Vector3 direction;

		float coneCos = 0;
		

		Vector3 halfWidth;

		Vector3 halfHeight;
		

		Light();
		

		Light(const Color& color, float indensity = 1);
	
		Light(const Light& source);
		
		static ptr create() {
			return std::make_shared<Light>();
		}

		static ptr create(const Color& color, float indensity = 1) {
			return std::make_shared<Light>(color, indensity);
		}

		virtual Light& copy(const Light& source);
		
		virtual Light* clone() {
			return new Light(*this);
		}
		virtual float power() {
			return this->intensity;
		}
		virtual void setPower(float value) {
			this->intensity = value;
		}
	};

	class AmbientLight : public Light {
	public:
		using ptr = std::shared_ptr<AmbientLight>;

		AmbientLight();		

		AmbientLight(const Color& color, float intensity = 1);
	

		AmbientLight(const AmbientLight& source);
	

		static ptr create() {
			return std::make_shared<AmbientLight>();
		}
		static ptr create(const Color& color, float intensity = 1) {
			return std::make_shared<AmbientLight>(color, intensity);
		}
		virtual AmbientLight* clone() override {
			return new AmbientLight(*this);
		}
		virtual AmbientLight& copy(const AmbientLight& source);
		
	};
	
	class DirectionalLight : public Light {
	public:
		using ptr = std::shared_ptr<DirectionalLight>;
			

		DirectionalLight();
	

		DirectionalLight(const Color& color, float intensity = 1);
		

		DirectionalLight(const DirectionalLight& source);
	

		static ptr create(const Color color, float intensity = 1) {
			return std::make_shared<DirectionalLight>(color, intensity);
		}
		virtual DirectionalLight* clone() override {
			return new DirectionalLight(*this);
		}
		virtual DirectionalLight& copy(const DirectionalLight& source);
		
		
	};

	class HemisphereLight : public Light {
	public:
		using ptr = std::shared_ptr<HemisphereLight>;

		HemisphereLight();
		
		HemisphereLight(const Color& skyColor, const Color& groundColor, float intensity = 1);
		

		static ptr create(const Color& skyColor, const Color& groundColor, float intensity = 1) {
			return std::make_shared<HemisphereLight>(skyColor, groundColor, intensity);
		}

		virtual HemisphereLight* clone() override {
			return new HemisphereLight(*this);
		}

		virtual HemisphereLight& copy(const HemisphereLight& source);
	
	};
	
	class PointLight : public Light {
	public:
		using ptr = std::shared_ptr<PointLight>;		
		

		PointLight();
	
		PointLight(const Color& color, float intensity = 1, float distance = std::numeric_limits<float>::quiet_NaN(), float decay = std::numeric_limits<float>::quiet_NaN());
		

		static ptr create(const Color& color, float intensity = 1, float distance = std::numeric_limits<float>::quiet_NaN(), float decay = std::numeric_limits<float>::quiet_NaN()) {
			return std::make_shared<PointLight>(color,intensity,distance,decay);
		}

		virtual PointLight* clone() override {
			return new PointLight(*this);
		}
		virtual PointLight& copy(const PointLight& source);
		
		virtual float power() override {
			return (float)(this->intensity * 4 * M_PI);
		}
		virtual void setPower(float value) override {
			this->intensity = (float)(value / (4 * M_PI));
		}
	};

	class RectAreaLight : public Light {
	public:
		using ptr = std::shared_ptr<RectAreaLight>;

		RectAreaLight();
		
		RectAreaLight(const Color& color, float intensity = 1, float width = std::numeric_limits<float>::quiet_NaN(), float height = std::numeric_limits<float>::quiet_NaN());
	

		static ptr create(const Color& color, float intensity = 1, float width = std::numeric_limits<float>::quiet_NaN(), float height = std::numeric_limits<float>::quiet_NaN()) {
			return std::make_shared<RectAreaLight>(color, intensity, width, height);
		}
		virtual RectAreaLight* clone() override {
			return new RectAreaLight(*this);
		}
		virtual RectAreaLight& copy(const RectAreaLight& source);
		
	};

	class SpotLight : public Light {
	public:
		using ptr = std::shared_ptr<SpotLight>;

		

		SpotLight();
	
		SpotLight(const Color& color, float intensity = 1, float distance = std::numeric_limits<float>::quiet_NaN(),	float angle = std::numeric_limits<float>::quiet_NaN(),	float penumbra = std::numeric_limits<float>::quiet_NaN(),float decay = std::numeric_limits<float>::quiet_NaN()); 
		

		static ptr create(const Color& color, float intensity = 1, float distance = std::numeric_limits<float>::quiet_NaN(),	float angle = std::numeric_limits<float>::quiet_NaN(),	float penumbra = std::numeric_limits<float>::quiet_NaN(),float decay = std::numeric_limits<float>::quiet_NaN()) 
		{

			return std::make_shared<SpotLight>(color,intensity,distance,angle,penumbra,decay);
		}

		virtual SpotLight* clone() override {
			return new SpotLight(*this);
		}

		virtual SpotLight& copy(const SpotLight& source);
		
		virtual float power() override {
			return (float)(this->intensity *  M_PI);
		}
		virtual void setPower(float value) override {
			this->intensity = (float)(value /  M_PI);
		}
	};
}
#endif