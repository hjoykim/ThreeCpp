#include "GLLights.h"
#include <three/lights/LightProbe.h>
#include <three/renderers/gl/GLUniforms.h>
namespace three {
	using namespace gl;
	

	GLLights::GLLights()
	{
		state.version=0;			
		state.ambient = { 0,0,0 };
		state.probe = { Vector3(), Vector3(), Vector3(), Vector3(), Vector3(), Vector3(), Vector3(), Vector3(), Vector3() };
	}

	using namespace std;
	void GLLights::setup(std::vector<Light::ptr>& lights, const Camera::ptr& camera)
	{
		Vector3 vector3;
		Matrix4 matrix4;
		Matrix4 matrix42;
			

		for (int i = 0; i < 9; i++)
			state.probe[i].set(0, 0, 0);

		int directionalLength = 0;

		int pointLength = 0;
		int spotLength = 0;
		int rectAreaLength = 0;
		int hemiLength = 0;

		int numDirectionalShadows = 0;
		int numPointShadows = 0;
		int numSpotShadows = 0;

		Matrix4 viewMatrix = camera->matrixWorldInverse;
		std::sort(lights.begin(), lights.end(), shadowCastingLightsFirst);

		//resetStates();

		state.directional.clear();
		state.directionalShadow.clear();
		state.directionalShadowMap.clear();
		state.directionalShadowMatrix.clear();
		state.spot.clear();
		state.spotShadow.clear();
		state.spotShadowMap.clear();
		state.spotShadowMatrix.clear();
		state.rectArea.clear();
		state.point.clear();
		state.pointShadow.clear();
		state.pointShadowMap.clear();
		state.pointShadowMatrix.clear();
		state.hemi.clear();

		state.directional.shrink_to_fit();
		state.directionalShadow.shrink_to_fit();
		state.directionalShadowMap.shrink_to_fit();
		state.directionalShadowMatrix.shrink_to_fit();
		state.spot.shrink_to_fit();
		state.spotShadow.shrink_to_fit();
		state.spotShadowMap.shrink_to_fit();
		state.spotShadowMatrix.shrink_to_fit();
		state.rectArea.shrink_to_fit();
		state.point.shrink_to_fit();
		state.pointShadow.shrink_to_fit();
		state.pointShadowMap.shrink_to_fit();
		state.pointShadowMatrix.shrink_to_fit();
		state.hemi.shrink_to_fit();

		state.ambient.set(0x000000);

		for (unsigned i = 0; i < lights.size(); i++) {

			Light::ptr light = lights[i];

			Color color = light->color;
			float intensity = light->intensity;
			float distance = light->distance;

			auto shadowMap = (light->shadow && light->shadow->shadowMap) ? light->shadow->shadowMap->texture : nullptr;

			if (light->type=="AmbientLight") {

				color.multiplyScalar(intensity);
				state.ambient = color;

			}
			else if (light->isLightProbe) {

				for (int j = 0; j < 9; j++) {					
					state.probe[j].addScaledVector(light->sh.coefficients[j], intensity);
				}

			}
			else if (light->type=="DirectionalLight") {				

				auto uniforms = cache.get(light);
				uniforms->color.copy(light->color);
				uniforms->color.multiplyScalar(light->intensity);
				uniforms->direction.setFromMatrixPosition(light->matrixWorld);

				vector3.setFromMatrixPosition(light->target->matrixWorld);
				uniforms->direction.sub(vector3);
				uniforms->direction.transformDirection(viewMatrix);
				

				if (light->castShadow) {

					auto shadow = light->shadow;

					auto shadowUniforms = shadowCache.get(light);
					shadowUniforms->shadowBias = shadow->shadowBias;
					
					shadowUniforms->shadowBias = shadow->shadowBias;
					shadowUniforms->shadowRadius = shadow->shadowRadius;
					shadowUniforms->shadowMapSize = shadow->shadowMapSize;

					state.directionalShadow.push_back(dynamic_pointer_cast<DirectionalLightShadow>(shadowUniforms));					
					state.directionalShadowMap.push_back(shadowMap);
					state.directionalShadowMatrix.push_back(light->shadow->shadowMatrix);

					numDirectionalShadows++;

				}

				state.directional.push_back(dynamic_pointer_cast<DirectionalLight>(uniforms));

				directionalLength++;

			}
			else if (light->type=="SpotLight") {

				auto uniforms = cache.get(light);
				uniforms->position.setFromMatrixPosition(light->matrixWorld);
				uniforms->position.applyMatrix4(viewMatrix);
				uniforms->color.copy(color);
				uniforms->color.multiplyScalar(intensity);
				uniforms->distance=distance;

				uniforms->direction.setFromMatrixPosition(light->matrixWorld);
				vector3.setFromMatrixPosition(light->target->matrixWorld);
				uniforms->direction.sub(vector3);
				uniforms->direction.transformDirection(viewMatrix);

				uniforms->coneCos= (float)cos(light->angle);
				uniforms->penumbra =(float)(cos(light->angle * (1 - light->penumbra)));
				uniforms->decay =light->decay;

				if (light->castShadow) {

					auto shadow = light->shadow;

					auto shadowUniforms = shadowCache.get(light);

					shadowUniforms->shadowBias=shadow->shadowBias;
					shadowUniforms->shadowRadius=shadow->shadowRadius;
					shadowUniforms->shadowMapSize.copy(shadow->shadowMapSize);

					state.spotShadow.push_back(dynamic_pointer_cast<SpotLightShadow>(shadowUniforms));
					state.spotShadowMap.push_back(shadowMap);
					state.spotShadowMatrix.push_back(light->shadow->shadowMatrix);					

					numSpotShadows++;

				}

				//state.spot[spotLength] = uniforms;
				state.spot.push_back(dynamic_pointer_cast<SpotLight>(uniforms));
				spotLength++;

			}
			else if (light->type=="RectAreaLight") {

				auto uniforms = cache.get(light);

				// (a) intensity is the total visible light emitted
				//uniforms.color.copy( color ).multiplyScalar( intensity / ( light->width * light->height * Math.PI ) );

				// (b) intensity is the brightness of the light
				uniforms->color.copy(color);
				uniforms->color.multiplyScalar(intensity);


				uniforms->position.setFromMatrixPosition(light->matrixWorld);
				uniforms->position.applyMatrix4(viewMatrix);


				// extract local rotation of light to derive width/height half vectors
				matrix42.identity();
				matrix4.copy(light->matrixWorld);
				matrix4.premultiply(viewMatrix);
				matrix42.extractRotation(matrix4);

				uniforms->halfWidth.set(light->width * 0.5f, 0.0, 0.0);
				uniforms->halfWidth.applyMatrix4(matrix42);

				uniforms->halfHeight.set(0.0, light->height * 0.5f, 0.0);
				uniforms->halfHeight.applyMatrix4(matrix42);

				// TODO (abelnation): RectAreaLight distance?
				// uniforms.distance = distance;

				//state.rectArea[rectAreaLength] = uniforms;
				state.rectArea.push_back(dynamic_pointer_cast<RectAreaLight>(uniforms));
				rectAreaLength++;

			}
			else if (light->type=="PointLight") {
				
				auto uniforms = cache.get(light); //  is equal to auto uniforms = cache->get(light->type);
				uniforms->position.setFromMatrixPosition(light->matrixWorld);
				uniforms->position.applyMatrix4(viewMatrix);

				uniforms->color.copy(light->color); 
				uniforms->color.multiplyScalar(light->intensity);

				uniforms->distance =light->distance;
				uniforms->decay =light->decay; // is equal to *uniforms["decay"] = light->decay;

				if (light->castShadow) {

					auto shadow = light->shadow;

					auto shadowuniforms = shadowCache.get(light);

					shadowuniforms->shadowBias=shadow->shadowBias;
					shadowuniforms->shadowRadius=shadow->shadowRadius;
					shadowuniforms->shadowMapSize.copy(shadow->shadowMapSize);
					shadowuniforms->shadowCameraNear=shadow->camera->_near;
					shadowuniforms->shadowCameraFar=shadow->camera->_far;

					state.pointShadow.push_back(dynamic_pointer_cast<PointLightShadow>(shadowuniforms));
					state.pointShadowMap.push_back(shadowMap);
					state.pointShadowMatrix.push_back(light->shadow->shadowMatrix);

					/*state.pointShadow[pointLength] = shadowUniforms;
					state.pointShadowMap[pointLength] = shadowMap;
					state.pointShadowMatrix[pointLength] = light->shadow.matrix;*/

					numPointShadows++;

				}


				state.point.push_back(dynamic_pointer_cast<PointLight>(uniforms));
				//state.point[pointLength] = uniforms;

				pointLength++;

			}
			else if (light->type=="HemisphereLight") {

				auto uniforms = cache.get(light);

				uniforms->direction.setFromMatrixPosition(light->matrixWorld);
				uniforms->direction.transformDirection(viewMatrix);
				uniforms->direction.normalize();
				uniforms->skyColor.copy(light->color);
				uniforms->skyColor.multiplyScalar(intensity);				
				uniforms->groundColor.copy(light->groundColor);
				uniforms->groundColor.multiplyScalar(intensity);

				state.hemi.push_back(dynamic_pointer_cast<HemisphereLight>(uniforms));
				//state.hemi[hemiLength] = uniforms;

				hemiLength++;

			}

		}

		
		
		/*state.ambient[1] = g;
		state.ambient[2] = b;*/

		//var hash = state.hash;

		if (state.hash.directionalLength != directionalLength ||
			state.hash.pointLength != pointLength ||
			state.hash.spotLength != spotLength ||
			state.hash.rectAreaLength != rectAreaLength ||
			state.hash.hemiLength != hemiLength ||
			state.hash.numDirectionalShadows != numDirectionalShadows ||
			state.hash.numPointShadows != numPointShadows ||
			state.hash.numSpotShadows != numSpotShadows) {

			

			state.hash.directionalLength= directionalLength;
			state.hash.pointLength= pointLength;
			state.hash.spotLength= spotLength;
			state.hash.rectAreaLength= rectAreaLength;
			state.hash.hemiLength= hemiLength;

			state.hash.numDirectionalShadows= numDirectionalShadows;
			state.hash.numPointShadows= numPointShadows;
			state.hash.numSpotShadows= numSpotShadows;

			state.version = nextVersion++;
		}
	}

}
