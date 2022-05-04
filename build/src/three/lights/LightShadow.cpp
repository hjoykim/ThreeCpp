#include "LightShadow.h"
#include <three/lights/Light.h>
#include <three/math/Vector3.h>
#include <three/cameras/Camera.h>
#include <three/cameras/OrthographicCamera.h>
#include <three/cameras/PerspectiveCamera.h>
namespace three {
	namespace lightshadow {
		Matrix4 _projScreenMatrix;
		Vector3 _lightPositionWorld;
		Vector3 _lookTarget;
	}
	using namespace lightshadow;
	LightShadow::LightShadow(const LightShadow& source) : LightShadow()
	{		
		frameExtents = source.frameExtents;
		frustum.copy(source.frustum);
		viewportCount = source.viewportCount;

		if (viewports.size() != source.viewports.size())
			viewports.resize(source.viewports.size());

		std::copy(source.viewports.begin(), source.viewports.end(), viewports.begin());

		this->camera.reset();

		camera = std::make_shared<Camera>(*source.camera);

		shadowBias = source.shadowBias;

		shadowRadius = source.shadowRadius;

		shadowNormalBias = source.shadowNormalBias;

		shadowMapSize.copy(source.shadowMapSize);

		/*if (source.shadowMap != nullptr)
			shadowMap = std::make_shared<GLRenderTarget>(*source.map);
		if(source.mapPass!=nullptr)
			mapPass = std::make_shared<GLRenderTarget>(*source.mapPass);*/

		shadowMatrix.copy(source.shadowMatrix);

		shadowCameraNear = source.shadowCameraNear;

		shadowCameraFar = source.shadowCameraFar;
	}
	LightShadow& LightShadow::copy(const LightShadow& source)
	{
		frameExtents = source.frameExtents;
		frustum.copy(source.frustum);
		viewportCount = source.viewportCount;

		if (viewports.size() != source.viewports.size())
			viewports.resize(source.viewports.size());

		std::copy(source.viewports.begin(), source.viewports.end(), viewports.begin());

		this->camera.reset();

		camera = std::make_shared<Camera>(*source.camera);

		camera = std::make_shared<Camera>(*source.camera);

		shadowBias = source.shadowBias;

		shadowRadius = source.shadowRadius;

		shadowNormalBias = source.shadowNormalBias;

		shadowMapSize.copy(source.shadowMapSize);

		/*if (source.shadowMap != nullptr)
			shadowMap = std::make_shared<GLRenderTarget>(*source.map);
		if(source.mapPass!=nullptr)
			mapPass = std::make_shared<GLRenderTarget>(*source.mapPass);*/

		shadowMatrix.copy(source.shadowMatrix);

		shadowCameraNear = source.shadowCameraNear;

		shadowCameraFar = source.shadowCameraFar;

		return *this;
	}
	LightShadow* LightShadow::clone()
	{
		return new LightShadow(*this);
	}

	void LightShadow::updateMatrices(const Light& light, int viewportIndex)
	{
		/*var shadowCamera = this.camera,
			shadowMatrix = this.matrix,
			projScreenMatrix = this._projScreenMatrix,
			lookTarget = this._lookTarget,
			lightPositionWorld = this._lightPositionWorld;*/

		_lightPositionWorld.setFromMatrixPosition(light.matrixWorld);
		camera->position.copy(_lightPositionWorld);

		_lookTarget.setFromMatrixPosition(light.target->matrixWorld);
		camera->lookAt(_lookTarget);
		camera->updateMatrixWorld();

		_projScreenMatrix.multiplyMatrices(camera->projectionMatrix, camera->matrixWorldInverse);
		frustum.setFromProjectionMatrix(_projScreenMatrix);

		shadowMatrix.set(
			0.5, 0.0, 0.0, 0.5,
			0.0, 0.5, 0.0, 0.5,
			0.0, 0.0, 0.5, 0.5,
			0.0, 0.0, 0.0, 1.0
		);

		shadowMatrix.multiply(camera->projectionMatrix);
		shadowMatrix.multiply(camera->matrixWorldInverse);
	}
	namespace pointlightshaow {
		std::vector<Vector3> _cubeDirections = {
			Vector3(1,0,0),
			Vector3(-1,0,0),
			Vector3(0,0,1),
			Vector3(0,0,-1),
			Vector3(0,1,0),
			Vector3(0,-1,0)
		};

		std::vector<Vector3> _cubeUps = {
			Vector3(0,1,0),
			Vector3(0,1,0),
			Vector3(0,1,0),
			Vector3(0,1,0),
			Vector3(0,0,1),
			Vector3(0,0,-1)
		};

		Matrix4 projScreenMatrix;

		Vector3 lightPositionWorld;

		Vector3 lookTarget;
	}
	using namespace pointlightshaow;

	PointLightShadow::PointLightShadow() : LightShadow(PerspectiveCamera::create(90.0f,1.0f,0.5f,500.0f))
	{
		lightShadowType = LightShadowType::PointLightShadow;
		frameExtents.copy(Vector2(4, 2));
		viewportCount = 6;
		this->viewports.resize(6);
		std::vector<Vector4> _viewports = {
			 Vector4(2,1,1,1),
			 Vector4(0,1,1,1),
			 Vector4(3,1,1,1),
			 Vector4(1,1,1,1),
			 Vector4(3,0,1,1),
			 Vector4(1,0,1,1)
		};

		for (unsigned i = 0; i < viewports.size(); i++) {
			this->viewports[i].copy(viewports[i]);
		}
	}

	void PointLightShadow::updateMatrices(const Light& light, int viewportIndex)
	{
		if (viewportIndex == -1) viewportIndex = 0;
		

		/*var camera = this.Camera;
		var shadowMatrix = this.Matrix;
		var lightPositionWorld = this._lightPositionWorld;
		var lookTarget = this._lookTarget;
		var projScreenMatrix = this._projScreenMatrix;*/

		lightPositionWorld.setFromMatrixPosition(light.matrixWorld);
		camera->position.copy(lightPositionWorld);

		lookTarget.copy(camera->position);
		lookTarget.add(_cubeDirections[viewportIndex]);
		camera->up.copy(_cubeUps[viewportIndex]);
		camera->lookAt(lookTarget);
		camera->updateMatrixWorld();

		shadowMatrix.makeTranslation(-lightPositionWorld.x, -lightPositionWorld.y, -lightPositionWorld.z);

		projScreenMatrix.multiplyMatrices(camera->projectionMatrix, camera->matrixWorldInverse);
		frustum.setFromProjectionMatrix(projScreenMatrix);
	}


	

	SpotLightShadow::SpotLightShadow() :LightShadow(PerspectiveCamera::create(50.0f, 1.0f, 0.5f, 500.0f)) {
		lightShadowType = LightShadowType::SpotLightShadow;
	}

	void SpotLightShadow::updateMatrices(const Light& light, int viewportIndex)
	{
		float fov = (float)(180 / M_PI * 2 * light.angle);
		float aspect = shadowMapSize.x / shadowMapSize.y;
		float far = !std::isnan(light.distance)&& light.distance != 0 ? light.distance : camera->_far;

		if (fov != camera->fov || aspect != camera->aspect || far != camera->_far)
		{
			camera->fov = fov;
			camera->aspect = aspect;
			camera->_far = far;
			camera->updateProjectionMatrix();
		}
		LightShadow::updateMatrices(light);
	}

	DirectionalLightShadow::DirectionalLightShadow() : LightShadow(OrthographicCamera::create(-5, 5, 5, -5, 0.5f, 500.0f)) {
		lightShadowType = LightShadowType::DirectionalLightShadow;
	}

}
