#ifndef LIGHT_SHADOW_THREE_H
#define LIGHT_SHADOW_THREE_H

#include <three/math/Frustum.h>
#include <three/math/Vector2.h>
#include <three/math/Vector4.h>
#include <three/math/Matrix4.h>
#include <three/renderers/gl/GLRenderTarget.h>
#include <memory>
using namespace three::math;
namespace three {
	class Light;
	class Camera;


	using namespace gl;

	enum class LightShadowType : unsigned {
		LightShadow=0,
		DirectionalLightShadow=1,
		PointLightShadow=2,
		SpotLightShadow=3
	};
	namespace shadow {
		static size_t _shadowId = 0;
	}
	using namespace shadow;
	class LightShadow {
		
	public:
		using ptr = std::shared_ptr<LightShadow>;
		using CameraPtr = std::shared_ptr<Camera>;

		size_t id;

		bool autoUpdate = true;

		bool needsUpdate = false;

		LightShadowType lightShadowType;

		Frustum frustum;

		Vector2 frameExtents = Vector2(1, 1);

		unsigned viewportCount = 1;

		std::vector<Vector4> viewports = { {Vector4(0,0,1,1)} };		

		using ptr = std::shared_ptr<LightShadow>;
		
		CameraPtr camera;

		float shadowBias = 0.0f;

		float shadowNormalBias = 0;

		float shadowRadius = 1.0f;

		Vector2 shadowMapSize = Vector2(512,512);

		GLRenderTarget::ptr shadowMap = nullptr;

		GLRenderTarget::ptr shadowMapPass = nullptr;

		Matrix4 shadowMatrix;

		float shadowCameraNear;

		float shadowCameraFar;


		LightShadow(): id(_shadowId++),lightShadowType(LightShadowType::LightShadow){}

		LightShadow(const CameraPtr& camera) :id(_shadowId++), camera(camera), lightShadowType(LightShadowType::LightShadow) {}
		
		LightShadow(const LightShadow& source);

		static ptr create() {
			return std::make_shared<LightShadow>();
		}

		static ptr create(const CameraPtr& camera) {
			return std::make_shared<LightShadow>(camera);
		}

		virtual LightShadow& copy(const LightShadow& source);

		virtual LightShadow* clone();		

		virtual void updateMatrices(const Light& light, int viewportIndex=-1);

		Vector2& getFrameExtents() {
			return frameExtents;
		}

		unsigned getViewportCount() {
			return viewportCount;
		}

		Vector4& getViewport(int viewportIndex) {
			return viewports[viewportIndex];
		}

		Frustum& getFrustum() {
			return frustum;
		}
	};

	class DirectionalLightShadow : public LightShadow {
	public:
		using ptr = std::shared_ptr<DirectionalLightShadow>;

		DirectionalLightShadow();
		
		

		DirectionalLightShadow(const DirectionalLightShadow& source) : LightShadow(source) {
			lightShadowType = LightShadowType::DirectionalLightShadow;
		}

		static ptr create() {
			return std::make_shared<DirectionalLightShadow>();
		}
		
		virtual DirectionalLightShadow* clone() override {			
			return new DirectionalLightShadow(*this);
		}

		virtual DirectionalLightShadow& copy(const DirectionalLightShadow& source) {
			LightShadow::copy(source);
			lightShadowType = LightShadowType::DirectionalLightShadow;
			return *this;
		}
	};

	class PointLightShadow : public LightShadow {
	public :
		using ptr = std::shared_ptr<PointLightShadow>;

		PointLightShadow();

		static ptr create() {
			return std::make_shared<PointLightShadow>();
		}

		virtual void updateMatrices(const Light& light, int viewportIndex = -1) override;
		
		virtual PointLightShadow* clone() override {
			return new PointLightShadow(*this);
		}

		virtual PointLightShadow& copy(const PointLightShadow& source) {
			LightShadow::copy(source);
			lightShadowType = LightShadowType::PointLightShadow;
			return *this;
		}
		
	};

	class SpotLightShadow : public LightShadow {
	public:
		using ptr = std::shared_ptr<SpotLightShadow>;

		SpotLightShadow(); 
	

		static ptr create() {
			return std::make_shared<SpotLightShadow>();
		}

		virtual void updateMatrices(const Light& light, int viewportIndex = -1) override;

		virtual SpotLightShadow* clone() override {
			return new SpotLightShadow(*this);
		}

		virtual SpotLightShadow& copy(const SpotLightShadow& source) {
			LightShadow::copy(source);
			lightShadowType = LightShadowType::SpotLightShadow;
			return *this;
		}

	};
}
#endif