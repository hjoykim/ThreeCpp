#include "GLShadowMap.h"
#include <three/utils/osdecl.h>
#include <three/objects/Line.h>
#include <three/objects/Points.h>
#include <three/objects/SkinnedMesh.h>
#include <three/renderers/shaders/ShaderChunk.h>
#include <three/renderers/GLRenderer.h>
#include <three/textures/Texture.h>
#include <three/materials/MeshDepthMaterial.h>
#include <three/materials/MeshDistanceMaterial.h>
#include <three/renderers/shaders/ShaderLib.h>
#include <iostream>
//#include <three/renderers/shaders/ShaderLib/vsm_vert.h>
//#include <three/renderers/shaders/ShaderLib/vsm_frag.h>
namespace three {
	namespace gl {
		Material::ptr getDepthMaterialVariant(int useMorphing, int useSkinning, int useInstancing,std::unordered_map<int,Material::ptr> *materials)
		{
			auto index = useMorphing << 0 | useSkinning << 1 | useInstancing << 2;

			Material::ptr material = nullptr;
			

			if (materials->count(index) == 0) {

				MeshDepthMaterial::ptr material1 = std::make_shared<MeshDepthMaterial>();
				material1->depthPacking = DepthPackingStrategies::RGBADepthPacking;
				material1->morphTargets = useMorphing;
				material1->skinning = useSkinning;
				material = std::dynamic_pointer_cast<Material>(material1);
				materials->insert({ index,material });
			}
			else
				material = materials->at(index);

			return material;
		}

		Material::ptr getDistanceMaterialVariant(int useMorphing, int useSkinning, int useInstancing, std::unordered_map<int, Material::ptr>* materials)
		{
			auto index = useMorphing << 0 | useSkinning << 1 | useInstancing << 2;

			Material::ptr material = nullptr;

			if (materials->count(index) == 0)
			{
				MeshDistanceMaterial::ptr material1 =  std::make_shared<MeshDistanceMaterial>();
				material1->morphTargets = useMorphing;
				material1->skinning = useSkinning;
				material = std::dynamic_pointer_cast<Material>(material1);
				materials->insert({ index,material });
			}
			else
				material = materials->at(index);

			return material;
		}

		Material::ptr GLShadowMap::getDepthMaterial(GLRenderer& renderer,Object3D& object3D, const Geometry::ptr& geometry,const Material::ptr& material, Light& light, float shadowCameraNear, float shadowCameraFar, ShadowMapType type)
		{
			Material::ptr result = nullptr;
			
			std::function<Material::ptr(int, int,int,std::unordered_map<int,Material::ptr>*)> getMaterialVariant = &getDepthMaterialVariant;
			std::unordered_map<int, Material::ptr>* materials = &_depthMaterials;
			auto customMaterial = object3D.customDepthMaterial;

			if (light.lightType==LightType::PointLight) {
				getMaterialVariant = &getDistanceMaterialVariant;
				customMaterial = object3D.customDistanceMaterial;
				materials = &_distanceMaterials;
			}

			if (customMaterial == nullptr) {
				bool useMorphing = false;

				if (material->morphTargets == true) {
					if (instanceOf<BufferGeometry>(geometry.get())) {
						auto bufferGeometry = std::dynamic_pointer_cast<BufferGeometry>(geometry);
						std::unordered_map<std::string, std::vector<BufferAttribute<float>::ptr>> attributes = bufferGeometry->morphAttributes;
						useMorphing = attributes.size() > 0 && attributes.count("position")>0 && attributes["position"].size() > 0;
					}
				}

				bool useSkinning = false;

				if (instanceOf<SkinnedMesh>(&object3D)) {
					if (material->skinning == true) {
						useSkinning = true;
					}
				}

				bool useInstancing = instanceOf<InstancedMesh>(&object3D);

				result = getMaterialVariant((int)useMorphing, (int)useSkinning, (int)useInstancing, materials);
			}
			else {
				result = customMaterial;
			}

			if (renderer.localClippingEnabled && material->clipShadows == true && material->clippingPlanes.size() != 0) {
				sole::uuid keyA = result->uuid;
				sole::uuid keyB = material->uuid;

				std::unordered_map<std::string, Material::ptr>* materialsForVariant = nullptr;

				if (_materialCache.count(keyA.str()) == 0) {					
					_materialCache.insert({ keyA.str(),std::unordered_map<std::string,Material::ptr>() });
				}
				else {
					materialsForVariant = &_materialCache[keyA.str()];
				}

				Material::ptr cachedMaterial = nullptr;

				if (materialsForVariant->count(keyB.str()) == 0) {
					if (light.lightType == LightType::PointLight) {
						MeshDistanceMaterial* material1 = reinterpret_cast<MeshDistanceMaterial *>(material.get());
						MeshDistanceMaterial::ptr cachedMaterial1 = std::make_shared<MeshDistanceMaterial>(*material1);
						cachedMaterial = std::reinterpret_pointer_cast<Material>(cachedMaterial1);
					}
					else {
						MeshDepthMaterial* material1 = reinterpret_cast<MeshDepthMaterial*>(material.get());
						MeshDepthMaterial::ptr cachedMaterial1 = std::make_shared<MeshDepthMaterial>(*material1);
						cachedMaterial = std::reinterpret_pointer_cast<Material>(cachedMaterial1);
					}

					materialsForVariant->insert({ keyB.str(), cachedMaterial });
				}
				else {
					cachedMaterial = materialsForVariant->at(keyB.str());
				}

				result = cachedMaterial;
			}

			result->visible = material->visible;
			result->wireframe = material->wireframe;

			if (type == ShadowMapType::VSMShadowMap) {
				result->side = (material->shadowSide != Side::None) ? material->shadowSide : material->side;
			}			
			else {
				int side;
				switch (material->side) {
					case Side::BackSide: side = 1;break;
					case Side::FrontSide:side = 0;break;
					case Side::DoubleSide: side = 2;break;
					default: side = 0;
				}
				result->side = (material->shadowSide != Side::None) ? material->shadowSide : shadowSide[side];
			}

			result->clipShadows = material->clipShadows;
			result->clippingPlanes = material->clippingPlanes;
			result->clipIntersection = material->clipIntersection;

			result->wireframeLinewidth = material->wireframeLinewidth;
			result->linewidth = material->linewidth;

			if (light.lightType==LightType::PointLight && instanceOf<MeshDistanceMaterial>(result.get())) {
				result->referencePosition.setFromMatrixPosition(light.matrixWorld);
				result->nearDistance = shadowCameraNear;
				result->farDistance = shadowCameraFar;
			}

			return result;
		}
		void GLShadowMap::renderObject(GLRenderer& renderer, const Object3D::ptr& object, const Camera::ptr& camera, const Camera::ptr& shadowCamera, Light& light, ShadowMapType type)
		{
			if (object->visible == false) return;

			bool visible = object->layers.test(camera->layers);
			Scene::ptr sceneTemp = nullptr;
			if (visible && (instanceOf<Mesh>(object.get()) || instanceOf<Line>(object.get()) || instanceOf<Points>(object.get()))) {

				if ((object->castShadow || (object->receiveShadow && type == ShadowMapType::VSMShadowMap)) && (!object->frustumCulled || _frustum.intersectsObject(*object))) {

					object->modelViewMatrix.multiplyMatrices(shadowCamera->matrixWorldInverse, object->matrixWorld);

					auto geometry = objects->update(*object);
					auto material = object->material;

					if (object->materials.size()>1) {

						auto groups = geometry->groups;

						for (int k = 0;k < groups.size(); k++) {

							auto group = groups[k];
							auto groupMaterial = object->materials[group.materialIndex];

							if (groupMaterial!=nullptr && groupMaterial->visible) {

								auto depthMaterial = getDepthMaterial(renderer,*object, geometry, groupMaterial, light, shadowCamera->_near, shadowCamera->_far, type);
								
								renderer.renderBufferDirect(shadowCamera, sceneTemp, geometry, depthMaterial, object, &group);

							}

						}

					}
					else if (material->visible) {

						auto depthMaterial = getDepthMaterial(renderer,*object, geometry, material, light, shadowCamera->_near, shadowCamera->_far, type);

						renderer.renderBufferDirect(shadowCamera, sceneTemp, geometry, depthMaterial, object, nullptr);

					}

				}

			}

			auto children = object->children;

			for (auto i = 0;i < children.size(); i++) {

				renderObject(renderer,children[i], camera, shadowCamera, light, type);

			}
		}
		void GLShadowMap::VSMPass(GLRenderer& renderer,LightShadow& shadow, const Camera::ptr& camera)
		{
			auto geometry = objects->update(*fullScreenMesh);
			Scene::ptr sceneTemp = nullptr;
			// vertical pass
			addUniformsValue(*shadowMaterialVertical->uniforms, "shadow_pass",shadow.shadowMap->texture);
			addUniformsValue(*shadowMaterialVertical->uniforms, "resolution", shadow.shadowMapSize);
			addUniformsValue(*shadowMaterialVertical->uniforms, "radius", shadow.shadowRadius);

			
			renderer.setRenderTarget(shadow.shadowMapPass);
			renderer.clear();
			renderer.renderBufferDirect(camera, sceneTemp, geometry, shadowMaterialVertical, fullScreenMesh);

			// horizonal pass
			addUniformsValue(*shadowMaterialHorizontal->uniforms, "shadow_pass", shadow.shadowMap->texture);
			addUniformsValue(*shadowMaterialHorizontal->uniforms, "resolution", shadow.shadowMapSize);
			addUniformsValue(*shadowMaterialHorizontal->uniforms, "radius", shadow.shadowRadius);

			renderer.setRenderTarget(shadow.shadowMapPass);
			renderer.clear();
			renderer.renderBufferDirect(camera, sceneTemp, geometry, shadowMaterialHorizontal, fullScreenMesh);
		}
		
		GLShadowMap::GLShadowMap(GLObjects::ptr& objects, int maxTextureSize) : objects(objects),maxTextureSize(maxTextureSize)
		{

			shadowMaterialVertical = ShaderMaterial::create();
			shadowMaterialHorizontal = ShaderMaterial::create();

			shadowMaterialVertical->definesFloat = {
				{"SAMPLE_RATE",2.0f / 8.0f},
				{"HALF_SAMPLE_RATE", 1.0f / 8.0f}
			};
			shadowMaterialVertical->uniforms->set<Texture::ptr>("shadow_pass", nullptr);			
			shadowMaterialVertical->uniforms->set("resolution", Vector2());
			shadowMaterialVertical->uniforms->set("shadowRadius", 4.0f);
			shadowMaterialVertical->uniforms->set("radius", 4.0f);
						
			shadowMaterialVertical->vertexShader.assign(getShaderChunk(ShaderLibID::vsm_vert));
			shadowMaterialVertical->fragmentShader.assign(getShaderChunk(ShaderLibID::vsm_frag));

			shadowMaterialHorizontal->copy(*shadowMaterialVertical);
			shadowMaterialHorizontal->definesFloat.insert({ "HORIZONAL_PASS", 1 });
			type = ShadowMapType::PCFShadowMap;

			BufferGeometry::ptr fullScreenTri = BufferGeometry::create();
			fullScreenTri->setAttribute(AttributeName::position,BufferAttribute<float>::create({ -1, -1, 0.5f, 3, -1, 0.5f, -1, 3, 0.5f },3));

			fullScreenMesh = Mesh::create(std::dynamic_pointer_cast<Geometry>(fullScreenTri), std::dynamic_pointer_cast<Material>(shadowMaterialVertical));
		}
		void GLShadowMap::render(GLRenderer& renderer, const std::vector<Light::ptr>& lights, Scene::ptr& scene, const Camera::ptr& camera)
		{
			if (enabled == false) return;
			if (autoUpdate == false && needsUpdate == false) return;

			if (lights.size() == 0) return;

			auto currentRenderTarget = renderer.getRenderTarget();
			auto activeCubeFace = renderer.getActiveCubeFace();
			auto activeMipmapLevel = renderer.getActiveMipmapLevel();

			auto _state = renderer.state;

			// Set GL state for depth map.
			_state->setBlending(Blending::NoBlending);
			_state->colorBuffer.setClear(1, 1, 1, 1);
			_state->depthBuffer.setTest(true);
			_state->setScissorTest(false);

			// render depth map

			for (auto i = 0;i<lights.size(); i++) {

				auto light = lights[i];
				auto shadow = light->shadow;

				if (shadow == nullptr) {
					continue;
				}

				_shadowMapSize.copy(shadow->shadowMapSize);

				auto shadowFrameExtents = shadow->getFrameExtents();

				_shadowMapSize.multiply(shadowFrameExtents);

				_viewportSize.copy(shadow->shadowMapSize);

				if (_shadowMapSize.x > maxTextureSize || _shadowMapSize.y > maxTextureSize) {

					//console.warn('THREE.WebGLShadowMap:', light, 'has shadow exceeding max texture size, reducing');

					if (_shadowMapSize.x > maxTextureSize) {

						_viewportSize.x = std::floor(maxTextureSize / shadowFrameExtents.x);
						_shadowMapSize.x = _viewportSize.x * shadowFrameExtents.x;
						shadow->shadowMapSize.x = _viewportSize.x;

					}

					if (_shadowMapSize.y > maxTextureSize) {

						_viewportSize.y = std::floor(maxTextureSize / shadowFrameExtents.y);
						_shadowMapSize.y = _viewportSize.y * shadowFrameExtents.y;
						shadow->shadowMapSize.y = _viewportSize.y;

					}

				}

				if (shadow->shadowMap ==  nullptr && instanceOf<PointLightShadow>(shadow.get()) && this->type == ShadowMapType::VSMShadowMap) 
				{
					GLRenderTargetParameter pars;
					pars.minFilter = TextureFilter::LinearFilter;
					pars.magFilter = TextureFilter::LinearFilter;
					pars.format = PixelFormat::RGBAFormat;

					shadow->shadowMap = std::make_shared<GLRenderTarget>(_shadowMapSize.x, _shadowMapSize.y,&pars);

					
					shadow->shadowMap->texture->name = light->name + ".shadowMap";

					shadow->shadowMapPass = std::make_shared<GLRenderTarget>(_shadowMapSize.x, _shadowMapSize.y, &pars);

					shadow->camera->updateProjectionMatrix();

				}

				if (shadow->shadowMap == nullptr) {

					GLRenderTargetParameter pars;
					pars.minFilter = TextureFilter::NearestFilter;
					pars.magFilter = TextureFilter::NearestFilter;
					pars.format = PixelFormat::RGBAFormat;

					shadow->shadowMap = std::make_shared<GLRenderTarget>(_shadowMapSize.x, _shadowMapSize.y, &pars);

					shadow->shadowMap->texture->name = light->name + ".shadowMap";

					shadow->camera->updateProjectionMatrix();

				}

				renderer.setRenderTarget(shadow->shadowMap);
				renderer.clear();

				auto viewportCount = shadow->getViewportCount();

				for (auto vp = 0; vp < viewportCount; vp++) {

					auto viewport = shadow->getViewport(vp);

					_viewport.set(
						_viewportSize.x * viewport.x,
						_viewportSize.y * viewport.y,
						_viewportSize.x * viewport.z,
						_viewportSize.y * viewport.w
					);

					_state->viewport(_viewport);

					shadow->updateMatrices(*light, vp);

					_frustum = shadow->getFrustum();

					renderObject(renderer,scene, camera, shadow->camera, *light, this->type);

				}

				// do blur pass for VSM

				if (!instanceOf<PointLightShadow>(shadow.get()) && this->type == ShadowMapType::VSMShadowMap) {

					VSMPass(renderer,*shadow, camera);

				}

			}

			this->needsUpdate = false;

			renderer.setRenderTarget(currentRenderTarget, activeCubeFace, activeMipmapLevel);

		}
	}
}
