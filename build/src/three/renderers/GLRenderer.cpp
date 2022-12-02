#include "GLRenderer.h"
#include <three/objects/SkinnedMesh.h>
#include <three/renderers/shaders/ShaderLib.h>
#include <three/renderers/shaders/UniformsLib.h>
#include <three/objects/Sprite.h>
#include <three/core/InstancedBufferGeometry.h>
#include <three/core/InterleavedBufferAttribute.h>
#include <three/core/InstancedInterleavedBuffer.h>
#include <three/core/InstancedBufferAttribute.h>
#include <three/objects/Line.h>
#include <three/objects/Points.h>
#include <three/objects/Sprite.h>
#include <three/renderers/gl/GLBufferRendererInterface.h>
#include <three/renderers/GLCubeRenderTarget.h>
#include <three/renderers/GLMultisampleRenderTarget.h>
#include <three/objects/Group.h>
#include <three/objects/LOD.h>
#include <three/cameras/PerspectiveCamera.h>
#include <three/extras/objects/ImmediateRenderObject.h>
#include <three/Textures/CompressedTexture.h>
namespace three {
	using namespace gl;	
	GLRenderer::GLRenderer(int width, int height) 	
	{
		initGLContext(width, height);
		
	}
	void GLRenderer::initGLContext(int width, int height)
	{		
		extensions = std::make_shared<GLExtensions>();
		capabilitiesParameter.logarithmicDepthBuffer = false;
		capabilitiesParameter.precision = "highp";
		capabilities = std::make_shared<GLCapabilities>(extensions, capabilitiesParameter);
		
		

		state = std::make_shared<GLState>();

		info = std::make_shared<GLInfo>();

		properties = std::make_shared<GLProperties>();

		_viewport.set(0, 0, width, height);
		//_scissor.set(0, 0, width, height);

		if (capabilities->isGL2 == false) {

		}

		
		_currentScissor = (_scissor * _pixelRatio).floor();
		_currentViewport = (_viewport * _pixelRatio).floor();

		state->scissor(_currentScissor);
		state->viewport(_currentViewport);

		
		cubeMaps = GLCubeMap::create(*this);
		

		textures = std::make_shared<GLTextures>(extensions, state, properties, capabilities, info);

		attributes = std::make_shared<GLAttributes>(capabilities->isGL2);

		geometries = std::make_shared<GLGeometries>(attributes, info);

		objects = std::make_shared<GLObjects>(geometries, attributes, info);

		morphtargets = std::make_shared<GLMorphtargets>();

		bindingStates = std::make_shared<GLBindingStates>(extensions, attributes, capabilities);

		programCache = std::make_shared<GLPrograms>(*cubeMaps,extensions, capabilities,bindingStates,_clipping);
		

		renderLists = std::make_shared<GLRenderLists>();

		renderStates = std::make_shared<GLRenderStates>();

		background = std::make_shared<GLBackground>(state, objects, premultipliedAlpha);

		bufferRenderer =GLBufferRenderer(extensions, info, capabilities);

		indexedBufferRenderer = GLIndexedBufferRenderer(extensions, info, capabilities);

		info->programs = programCache->programs;

		tempScene = std::make_shared<Scene>();

		shadowMap = std::make_shared<GLShadowMap>(objects, capabilities->maxTextureSize);

		materials = GLMaterials(properties);
		
		
	}

	int GLRenderer::getTargetPixelRatio()
	{		
		return _pixelRatio != 0 ? _pixelRatio : 1;
	}

	void GLRenderer::initMaterial(const Material::ptr& material, const Scene::ptr& scene, const Object3D::ptr& object)
	{
		auto fog = scene->fog;

		auto& materialProperties = properties->getMaterialProperties(material->uuid);
		MaterialProperties m;
		GLLights lights = currentRenderState->state.lights;

		std::vector<Light::ptr> shadowsArray = currentRenderState->state.shadowsArray;

		auto lightsStateVersion = lights.state.version;

		ProgramParameters::ptr parameters= programCache->getParameters(*this, material, lights, shadowsArray, scene.get(),*object);
		std::string programCacheKey = programCache->getProgramCacheKey(*material, *parameters);

		bool programChange = true;

		if (materialProperties.program==nullptr) {
			material->onDispose.connect(*this, &GLRenderer::onMaterialDispose);
		}
		else if (materialProperties.program->cacheKey != programCacheKey) {
			releaseMaterialProgramReference(*material);
		}
		else if (materialProperties.lightsStateVersion!=lightsStateVersion) {
			materialProperties.lightsStateVersion =lightsStateVersion;
			programChange = false;
		}		
		else if (!parameters->shaderID.empty()) {
			auto environment = instanceOf<MeshStandardMaterial>(material.get()) ? scene->environment : nullptr;
			materialProperties.envMap = cubeMaps->get(material->envMap != nullptr ? material->envMap : environment);
			return;
		}
		else {
			programChange = false;
		}

		if (programChange) {

			parameters->uniforms = programCache->getUniforms(material);			
			
			GLProgram::ptr program = programCache->acquireProgram(*this,*parameters, programCacheKey);

			materialProperties.program = program;
			materialProperties.uniforms = parameters->uniforms;			
			materialProperties.outputEncoding = parameters->outputEncoding;
			
		}

		auto& uniforms = materialProperties.uniforms;

		

		if (!material->isShaderMaterial && !material->isRawShaderMaterial || material->clipping) {
			materialProperties.numClippingPlanes = _clipping.numPlanes;
			materialProperties.numIntersection = _clipping.numIntersection;
			uniforms.set("clippingPlanes", _clipping.uniform);
		}
		materialProperties.environment = instanceOf<MeshStandardMaterial>(material.get()) ? scene->environment : nullptr;
		materialProperties.fog = scene->fog;
		materialProperties.envMap = cubeMaps->get(material->envMap != nullptr ? material->envMap : materialProperties.environment);
		materialProperties.needsLights = materialNeedsLights(*material);
		materialProperties.lightsStateVersion =lightsStateVersion;

		if (materialProperties.needsLights) {
			uniforms.set("ambientLightColor",lights.state.ambient);
			uniforms.set("lightProbe",lights.state.probe);
			uniforms.set("directionalLights",lights.state.directional);
			uniforms.set("directionalLightShadows",lights.state.directionalShadow);
			uniforms.set("spotLights",lights.state.spot);
			uniforms.set("spotLightShadows",lights.state.spotShadow);
			uniforms.set("rectAreaLights",lights.state.rectArea);
			uniforms.set("pointLights",lights.state.point);
			uniforms.set("pointLightShadows",lights.state.pointShadow);
			uniforms.set("hemisphereLights",lights.state.hemi);

			uniforms.set("directionalShadowMap",lights.state.directionalShadowMap);
			uniforms.set("directionalShadowMatrix",lights.state.directionalShadowMatrix);
			uniforms.set("spotShadowMap",lights.state.spotShadowMap);
			uniforms.set("spotShadowMatrix",lights.state.spotShadowMatrix);
			uniforms.set("pointShadowMap",lights.state.pointShadowMap);
			uniforms.set("pointShadowMatrix",lights.state.pointShadowMatrix);
		}

		//auto progUniforms = materialProperties->get<GLProgram::ptr>("program")->getUniforms();
		GLProgram::ptr program = materialProperties.program;
		GLUniforms::ptr progUniforms = program->getUniforms();

		std::vector<GLUniform::ptr> uniformList = progUniforms->seqWithValue(uniforms);

		materialProperties.uniformsList = uniformList;
	}

	bool GLRenderer::materialNeedsLights(Material& material)
	{		
		return (material.type=="MeshLambertMaterial" || material.type=="MeshToonMaterial" || material.type=="MeshPhongMaterial" || 
			material.type=="MeshStandardMaterial" || material.type=="ShadowMaterial" || material.type=="ShaderMaterial") && material.lights == true;
	}

	GLProgram::ptr GLRenderer::setProgram(const Camera::ptr& camera, const Scene::ptr& scene, const Material::ptr& material, const Object3D::ptr& object)
	{
		Timer sw;
		textures->resetTextureUnits();

		Fog::ptr fog = scene->fog;

		auto environment = material->type=="MeshStandardMaterial" ? scene->environment : nullptr;
		auto encoding = (_currentRenderTarget == nullptr) ? outputEncoding : _currentRenderTarget->texture->encoding;
		auto envMap = cubeMaps->get(material->envMap != nullptr ? material->envMap : environment);
		auto& materialProperties = properties->getMaterialProperties(material->uuid);

		GLLights lights = currentRenderState->state.lights;

		if (_clippingEnabled) {
			if (_localClippingEnabled || camera != _currentCamera) {
				auto useCache = camera == _currentCamera && material->id == _currentMaterialId;

				_clipping.setState(material->clippingPlanes, material->clipIntersection, material->clipShadows, camera, materialProperties, useCache);
			}
		}
		

		int version = materialProperties.version > -1 ? materialProperties.version : -1;

		if (material->version == version) {
			if (material->fog && materialProperties.fog != fog) {
				initMaterial(material, scene, object);
			}
			else if (materialProperties.environment != nullptr && materialProperties.environment != environment) {
				initMaterial(material, scene, object);
			}			
			else if (materialProperties.needsLights) {
				if (lights.state.version != materialProperties.lightsStateVersion) {
					initMaterial(material, scene, object);
				}
			}
			else if (materialProperties.numClippingPlanes != _clipping.numPlanes ||
				materialProperties.numIntersection != _clipping.numIntersection)
			{
				initMaterial(material, scene, object);
			}
			else if (materialProperties.outputEncoding != outputEncoding) {
				initMaterial(material, scene, object);
			}
			else if (materialProperties.envMap != nullptr && materialProperties.envMap != envMap) {
				initMaterial(material, scene, object);
			}
		}
		else {
			initMaterial(material, scene, object);
			materialProperties.version = material->version;			
			//std::cout << "material version:" << material->version << std::endl;
		}
		
		GLProgram::ptr program = materialProperties.program;
		GLUniforms::ptr p_uniforms = program->getUniforms();
		UniformValues& m_uniforms = materialProperties.uniforms;

		bool refreshProgram = false;
		bool refreshMaterial = false;
		bool refreshLights = false;

	
		if (materialProperties.needsLights) {
			m_uniforms.set("ambientLightColor", lights.state.ambient);
			m_uniforms.set("lightProbe", lights.state.probe);
			m_uniforms.set("directionalLights", lights.state.directional);
			m_uniforms.set("directionalLightShadows", lights.state.directionalShadow);
			m_uniforms.set("spotLights", lights.state.spot);
			m_uniforms.set("spotLightShadows", lights.state.spotShadow);
			m_uniforms.set("rectAreaLights", lights.state.rectArea);
			m_uniforms.set("pointLights", lights.state.point);
			m_uniforms.set("pointLightShadows", lights.state.pointShadow);
			m_uniforms.set("hemisphereLights", lights.state.hemi);

			m_uniforms.set("directionalShadowMap", lights.state.directionalShadowMap);
			m_uniforms.set("directionalShadowMatrix", lights.state.directionalShadowMatrix);
			m_uniforms.set("spotShadowMap", lights.state.spotShadowMap);
			m_uniforms.set("spotShadowMatrix", lights.state.spotShadowMatrix);
			m_uniforms.set("pointShadowMap", lights.state.pointShadowMap);
			m_uniforms.set("pointShadowMatrix", lights.state.pointShadowMatrix);
		}

		if (state->useProgram(program->program)) {
			refreshProgram = true;
			refreshMaterial = true;
			refreshLights = true;
		}
		if (material->id != _currentMaterialId) {
			_currentMaterialId = material->id;

			refreshMaterial = true;
		}

		// When resizeing, it always need to apply camera ProjectionMatrix
		p_uniforms->setProjectionMatrix(camera->projectionMatrix);

		if (refreshProgram || _currentCamera != camera) {
			//p_uniforms->setValue("projectionMatrix", camera->projectionMatrix);

			if (capabilities->logarithmicDepthBuffer) {
				float logDepthBufFC = (GLfloat)(2.0f / (log(camera->_far + 1.0f) / LN2));
				p_uniforms->setUniformValue("logDepthBufFC",logDepthBufFC);
			}

			if (_currentCamera != camera) {
				_currentCamera = camera;
				// lighting uniforms depend on the camera so enforce an update
				// now, in case this material supports lights - or later, when
				// the next material that does gets activated:

				refreshMaterial = true;		// set to true on material change
				refreshLights = true;		// remains set until update done
			}
			// load material specific uniforms
			// (shader material also gets them for the sake of genericity)

			if(instanceOf<ShaderMaterial>(material.get()) ||
			   instanceOf<MeshPhongMaterial>(material.get()) ||
			   instanceOf<MeshToonMaterial>(material.get()) ||
			   instanceOf<MeshStandardMaterial>(material.get()) ||
			   material->envMap!=nullptr) {		
				if (p_uniforms->contains("cameraPosition")) {
					GLUniform::ptr uCamPos = p_uniforms->get("cameraPosition");
					uCamPos->setValue(Vector3().setFromMatrixPosition(camera->matrixWorld));
				}
			}

			if (instanceOf<MeshPhongMaterial>(material.get()) ||
				instanceOf<MeshToonMaterial>(material.get()) ||
				instanceOf<MeshLambertMaterial>(material.get()) ||
				instanceOf<MeshBasicMaterial>(material.get()) ||
				instanceOf<MeshStandardMaterial>(material.get()) ||
				instanceOf<ShaderMaterial>(material.get())) {
				bool isOrthographic = camera->isOrthographic;
				p_uniforms->setUniformValue("isOrthographic", isOrthographic);
			}

			if (instanceOf<MeshPhongMaterial>(material.get()) ||
				instanceOf<MeshToonMaterial>(material.get()) ||
				instanceOf<MeshLambertMaterial>(material.get()) ||
				instanceOf<MeshBasicMaterial>(material.get()) ||
				instanceOf<MeshStandardMaterial>(material.get()) ||
				instanceOf<ShaderMaterial>(material.get()) ||
				material->skinning) {		

				p_uniforms->setUniformValue("viewMatrix",camera->matrixWorldInverse);
			}
		}

		// skinning uniforms must be set even if material didn't change
		// auto-setting of texture unit for bone texture must go before other textures
		// otherwise textures used for skinning can take over texture units reserved for other material textures

		if (material->skinning) {
			//p_uniforms->setOptional<Matrix4>(*dynamic_cast<ThreeDataTable *>(object.get()), "bindMatrix");
			//p_uniforms->setOptional<Matrix4>(*dynamic_cast<ThreeDataTable*>(object.get()), "bindMatrixInverse");
			

			SkinnedMesh* skinnedMesh = dynamic_cast<SkinnedMesh*>(object.get());
			p_uniforms->setUniformValue("bindMatrix", skinnedMesh->bindMatrix);
			p_uniforms->setUniformValue("bindMatrixInverse", skinnedMesh->bindMatrixInverse);


			Skeleton::ptr skeleton = skinnedMesh->skeleton;
			if (skeleton != nullptr) {
				auto bones = skeleton->bones;
				
				if (capabilities->floatVertexTextures) {

					if (skeleton->boneTexture != nullptr) {
						// layout (1 matrix = 4 pixels)
						//      RGBA RGBA RGBA RGBA (=> column1, column2, column3, column4)
						//  with  8x8  pixel texture max   16 bones * 4 pixels =  (8 * 8)
						//       16x16 pixel texture max   64 bones * 4 pixels = (16 * 16)
						//       32x32 pixel texture max  256 bones * 4 pixels = (32 * 32)
						//       64x64 pixel texture max 1024 bones * 4 pixels = (64 * 64)

						float size = (float)sqrt(bones.size() * 4);
						size = ceilPowerOfTwo(size);
						size = (int)std::max(size, 4.0f);

						std::vector<unsigned char> boneMatrices((int)(size * size * 4));

						std::copy(skeleton->boneMatrices.begin(), skeleton->boneMatrices.end(), boneMatrices.begin());
						
						DataTexture::ptr boneTexture = std::make_shared<DataTexture>(boneMatrices, size, size, 
							TextureMapping::UVMapping,Wrapping::ClampToEdgeWrapping,Wrapping::ClampToEdgeWrapping,
							TextureFilter::NearestFilter,TextureFilter::NearestFilter,
							PixelFormat::RGBAFormat, TextureDataType::FloatType,
							1,TextureEncoding::LinearEncoding);
						skeleton->boneMatrices = boneMatrices;
						skeleton->boneTexture = boneTexture;
						skeleton->boneTextureSize = (int)size;
					}

					p_uniforms->setUniformValue("boneTexture", skeleton->boneTexture);
					p_uniforms->setUniformValue("boneTextureSize", skeleton->boneTextureSize);
				}
				else {
					p_uniforms->setUniformValue("boneMatrices", skeleton->boneMatrices);
					//p_uniforms->setOptional<std::vector<float>>(*dynamic_cast<ThreeDataTable*>(skeleton.get()), "boneMatrices");
				}
			}
		}

		if (refreshMaterial || (materialProperties.receiveShadow != object->receiveShadow)) {
			materialProperties.receiveShadow = object->receiveShadow;
			p_uniforms->setUniformValue("receiveShadow", object->receiveShadow);
		}

		if (refreshMaterial) {
			p_uniforms->setUniformValue("toneMappingExposure", toneMappingExposure);
			//p_uniforms->setUniformValue("toneMappingWhitePoint, toneMappingWhitePoint);

			if (materialProperties.needsLights)
			{
				markUniformsLightsNeedsUpdate(m_uniforms, refreshLights);
			}

			if (scene->fog != nullptr && material->fog) {
				materials.refreshUniformsFog(m_uniforms, *scene->fog);
			}

			materials.refreshMaterialUniforms(m_uniforms, material, _pixelRatio, _height);
			
			std::vector<GLUniform::ptr> uniformsList = materialProperties.uniformsList;
			
			GLUniforms::upload(uniformsList, m_uniforms);

		}

		if (instanceOf<ShaderMaterial>(material.get()) && material->uniformsNeedUpdate == true) {
			std::vector<GLUniform::ptr> uniformsList = materialProperties.uniformsList;
			GLUniforms::upload(uniformsList, m_uniforms);

			//WebGLUniforms.upload(_gl, materialProperties.uniformsList, m_uniforms, textures);
			material->uniformsNeedUpdate = false;

		}

		if (instanceOf<SpriteMaterial>(material.get())) {
			Sprite::ptr sprite = std::dynamic_pointer_cast<Sprite>(object);
			p_uniforms->setUniformValue("center", sprite->center);

		}

		// common matrices

		p_uniforms->setUniformValue("modelViewMatrix", object->modelViewMatrix);
		p_uniforms->setUniformValue("normalMatrix", object->normalMatrix);
		p_uniforms->setUniformValue("modelMatrix", object->matrixWorld);

		//std::cout << "setProgram :" << sw.get_elapsed_time() << std::endl;
		return program;

	}
	void GLRenderer::markUniformsLightsNeedsUpdate(UniformValues& uniforms, bool value)
	{
		
		uniforms.needsUpdate("ambientLightColor",value);
		uniforms.needsUpdate("lightProbe", value);
		uniforms.needsUpdate("directionalLights", value);
		uniforms.needsUpdate("directionalLightShadows", value);
		uniforms.needsUpdate("pointLights", value);
		uniforms.needsUpdate("pointLightShadows", value);
		uniforms.needsUpdate("spotLights", value);
		uniforms.needsUpdate("spotLightShadows", value);
		uniforms.needsUpdate("rectAreaLights", value);
		uniforms.needsUpdate("hemisphereLights", value);
		
	}

	
	void GLRenderer::deallocateMaterial(Material& material)
	{
		releaseMaterialProgramReference(material);
		properties->remove(material.uuid);
	}

	void GLRenderer::releaseMaterialProgramReference(Material& material)
	{
		auto& materialProperties = properties->getMaterialProperties(material.uuid);
		if (materialProperties.program!=nullptr) {
			auto programInfo = materialProperties.program;
			programCache->releaseProgram(programInfo);
		}	
	}

	void GLRenderer::onMaterialDispose(Material& material)
	{
		material.onDispose.disconnectAll();
		deallocateMaterial(material);
	}

	void GLRenderer::projectObject(const std::shared_ptr<Object3D>& object, Camera& camera, int groupOrder, bool sortObjects)
	{
		if (object->visible == false) return;

		//std::cout << object->name << std::endl;

		bool visible = object->layers.test(camera.layers);

		if (visible) {
			if (instanceOf<Group>(object.get())) {
				groupOrder = object->renderOrder;
			}
			else if(instanceOf<LOD>(object.get())){
				LOD::ptr lodObject = std::dynamic_pointer_cast<LOD>(object);
				if (lodObject->autoUpdate == true) lodObject->update(camera);
			}
			else if (instanceOf<Light>(object.get())) {
				Light::ptr lightObject = std::dynamic_pointer_cast<Light>(object);
				currentRenderState->pushLight(lightObject);
				if (lightObject->castShadow) {
					currentRenderState->pushShadow(lightObject);
				}
			}
			else if (instanceOf<Sprite>(object.get())) {
				Sprite::ptr spriteObject = std::dynamic_pointer_cast<Sprite>(object);
				if (!spriteObject->frustumCulled || _frustum.intersectsSprite(*spriteObject)) {
					if (sortObjects) {
						_vector3.setFromMatrixPosition(spriteObject->matrixWorld).applyMatrix4(_projScreenMatrix);
					}
					auto geometry = objects->update(*spriteObject);
					auto material = spriteObject->material;
					if (material->visible) {
						currentRenderList->push(spriteObject, geometry, material, groupOrder, _vector3.z, NULL);
					}
				}
			}
			/*else if (is<ImmediateRenderObject>(object)) {

			}*/
			else if (instanceOf<Mesh>(object.get()) || instanceOf<Line>(object.get()) || instanceOf<Points>(object.get())) {
				if (instanceOf<SkinnedMesh>(object.get())) {
					SkinnedMesh::ptr skinObject = std::dynamic_pointer_cast<SkinnedMesh>(object);
					if (skinObject->skeleton->frame != info->render.frame) {
						skinObject->skeleton->update();
						skinObject->skeleton->frame = info->render.frame;
					}
				}

				if (!object->frustumCulled || _frustum.intersectsObject(*object)) {
					if (sortObjects) {

						_vector3.setFromMatrixPosition(object->matrixWorld).applyMatrix4(_projScreenMatrix);
					}
					
					auto geometry = objects->update(*object);
					auto material = object->material;
					if(object->materials.size()>1) {
						auto& groups = geometry->groups;

						for (auto i = 0;i < groups.size();i++) {
							auto& group = groups[i];
							auto& groupMaterial = object->materials[group.materialIndex];
							if (groupMaterial != nullptr && groupMaterial->visible) {
								currentRenderList->push(object, geometry, groupMaterial, groupOrder, _vector3.z, &group);
							}
						}
					}
					else if (material->visible) {
						currentRenderList->push(object, geometry, material, groupOrder, _vector3.z, NULL);
					}
				}
			}
		}

		for (unsigned i = 0;i < object->children.size();i++) {
			projectObject(object->children[i], camera, groupOrder, sortObjects);
		}
	}
	void GLRenderer::renderObjects(const std::vector<RenderItem::ptr>& renderList, Scene::ptr& scene, const Camera::ptr& camera)
	{
		auto overrideMaterial = scene != nullptr ? scene->overrideMaterial : nullptr;

		for (unsigned i = 0;i < renderList.size();i++) {
			auto renderItem = renderList[i];

			auto object = renderItem->object;
			auto geometry = renderItem->geometry;
			auto material = overrideMaterial == nullptr ? renderItem->material : overrideMaterial;
			
			DrawRange* group = renderItem->group;

			if (camera->isArrayCamera) {
				_currentArrayCamera = std::dynamic_pointer_cast<ArrayCamera>(camera);
				auto cameras = _currentArrayCamera->cameras;

				for (unsigned j = 0;j < cameras.size();j++) {
					auto camera2 = cameras[j];
					if (object->layers.test(camera2->layers)) {
						state->viewport(_currentViewport.copy(camera2->viewport));
						
						currentRenderState->setupLights(camera2);

						renderObject(object, scene, camera2, geometry, material, group);
					}
				}
					
			}
			else {
				if (_currentArrayCamera != nullptr) {
					_currentArrayCamera.reset();
					_currentArrayCamera = nullptr;
				}
				renderObject(object, scene, camera, geometry, material, group);
			}
		}
	}
	void GLRenderer::renderObject(const Object3D::ptr& object, Scene::ptr& scene, const Camera::ptr& camera, const BufferGeometry::ptr& geometry, const Material::ptr& material, DrawRange* group)
	{

		object->onBeforeRender.emitSignal(*this, scene, camera, object, geometry,material,nullptr, NULL);
		currentRenderState = renderStates->get(scene, _currentArrayCamera != nullptr ? _currentArrayCamera : camera);

		object->modelViewMatrix.multiplyMatrices(camera->matrixWorldInverse, object->matrixWorld);
		object->normalMatrix.getNormalMatrix(object->modelViewMatrix);

		if (instanceOf<extras::ImmediateRenderObject>(object.get())) {
			auto program = setProgram(camera, scene, material, object);
			
			state->setMaterial(*material);

			_currentGeometryProgram.geometry = -1;
			_currentGeometryProgram.program = -1;
			_currentGeometryProgram.wireframe = false;

			renderObjectImmediate(object, program);
		}
		else {
			renderBufferDirect(camera, scene, geometry, material, object, group);
		}
		object->onAfterRender.emitSignal(*this, scene, camera, object,geometry,material,nullptr, group);
		currentRenderState = renderStates->get(scene, _currentArrayCamera != nullptr ? _currentArrayCamera : camera);
	}

	void GLRenderer::renderObjectImmediate(const Object3D::ptr& object, const GLProgram::ptr& program)
	{
		renderBufferImmediate(object, program);
	}
	
	

	GLRenderer::~GLRenderer()
	{
		if (programCache != nullptr && programCache->programs.size() > 0) {
			for (int i = 0;i < programCache->programs.size();i++) {
				auto p = programCache->programs[i];
				if(p!=nullptr && p->program)
					glDeleteProgram(p->program);
			}
		}
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_SCISSOR_TEST);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);

		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ZERO);
		glBlendFuncSeparate(GL_ONE, GL_ZERO, GL_ONE, GL_ZERO);

		glColorMask(true, true, true, true);
		glClearColor(0, 0, 0, 0);

		glDepthMask(true);
		glDepthFunc(GL_LESS);
		glClearDepth(1);

		glStencilMask(0xffffffff);
		glStencilFunc(GL_ALWAYS, 0, 0xffffffff);
		glStencilOp(GL_KEEP, GL_KEEP,GL_KEEP);
		glClearStencil(0);

		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		glPolygonOffset(0, 0);

		glActiveTexture(GL_TEXTURE0);

		glBindFramebuffer(GL_FRAMEBUFFER, NULL);

		if (capabilities->isGL2 == true)
		{

			glBindFramebuffer(GL_DRAW_BUFFER, NULL);
			glBindFramebuffer(GL_READ_FRAMEBUFFER,NULL);

		}

		glUseProgram(0);

		glLineWidth(1);

		glScissor(0, 0, _width, _height);
		glViewport(0, 0, _width, _height);
	}

	int GLRenderer::getPixelRatio()
	{
		return _pixelRatio;
	}

	void GLRenderer::setPixelRatio(int value)
	{
		_pixelRatio = value;
	}

	Vector2& GLRenderer::getDrawingBufferSize(Vector2& target)
	{
		target.set(_width * _pixelRatio, _height * _pixelRatio).floor();
		return target;
	}

	void GLRenderer::setDrawingBufferSize(float width, float height, int pixelRatio)
	{
		_width = width;
		_height = height;
		_pixelRatio = pixelRatio;
	}
	Vector2& GLRenderer::getSize(Vector2& target)
	{
		target.set(_width, _height);
		return target;
	}

	void GLRenderer::setSize(float width, float height, bool updateStyle)
	{
		_width = width;
		_height = height;

		setViewport(0, 0, width, height);

	}

	Vector4& GLRenderer::getCurrentViewport(Vector4& target)
	{
		target.copy(_currentViewport);
		return target;
	}

	Vector4& GLRenderer::getViewport(Vector4& target)
	{
		target.copy(_viewport);
		return target;
	}

	void GLRenderer::setViewport(Vector4& v)
	{
		_viewport.copy(v);
		_currentViewport.copy(_viewport);
		_currentViewport.multiplyScalar(_pixelRatio).floor();
		state->viewport(_currentViewport);
	}

	void GLRenderer::setViewport(float x, float y, float width, float height)
	{
		_viewport.set(x,y,width,height);

		//_scissor.set(0, 0, width, height);

		_currentViewport.copy(_viewport);

		//_currentScissor.copy(_scissor);

		_currentViewport.multiplyScalar(_pixelRatio).floor();

		//_currentScissor.multiplyScalar(_pixelRatio).floor();

		state->viewport(_currentViewport);
	}

	Vector4& GLRenderer::getScissor(Vector4& target)
	{
		target.copy(_scissor);
		return target;
	}

	void GLRenderer::setScissor(float x, float y, float width, float height)
	{
		_scissor.set(x, y, width, height);
		_currentScissor.copy(_scissor).multiplyScalar(_pixelRatio).floor();
		state->scissor(_currentScissor);
	}

	void GLRenderer::setScissor(Vector4& v)
	{
		_scissor.copy(v);
		_currentScissor.copy(_scissor).multiplyScalar(_pixelRatio).floor();
		state->scissor(_currentScissor);
	}

	bool GLRenderer::getScissorTest()
	{
		return _scissorTest;
	}

	void GLRenderer::setScissorTest(bool enable)
	{
		_scissorTest = enable;
		state->setScissorTest(_scissorTest);
	}

	Color& GLRenderer::getClearColor()
	{
		return background->getClearColor();
	}

	void GLRenderer::setClearColor(const Color& color, float alpha)
	{
		if (std::isnan(alpha))
			background->setClearColor(color);
		else
			background->setClearColor(color, alpha);
	}

	void GLRenderer::setClearColor(int color, float alpha)
	{
		if (std::isnan(alpha))
			background->setClearColor(Color().setHex(color));
		else
			background->setClearColor(Color().setHex(color),alpha);
	}

	int GLRenderer::getClearAlpha()
	{
		return background->getClearAlpha();
	}

	void GLRenderer::setClearAlpha(float alpha)
	{
		background->setClearAlpha(alpha);
		
	}

	void GLRenderer::clear(bool color, bool depth, bool stencil)
	{
		unsigned bits = 0;
		if (color) bits |= GL_COLOR_BUFFER_BIT;
		if (depth) bits |= GL_DEPTH_BUFFER_BIT;
		if (stencil) bits |= GL_STENCIL_BUFFER_BIT;

		glClear(bits);
	}

	void GLRenderer::clearColor()
	{
		clear(true, false, false);
	}

	void GLRenderer::clearDepth()
	{
		clear(false, true, false);
	}

	void GLRenderer::clearStencil()
	{
		clear(false, false, true);
	}
	void GLRenderer::renderBufferImmediate(const Object3D::ptr& object, const GLProgram::ptr& program)
	{
		state->initAttributes();
	}
	void GLRenderer::renderBufferDirect(const Camera::ptr& camera, Scene::ptr& scene, const BufferGeometry::ptr& geometry, const Material::ptr& material, const Object3D::ptr& object, DrawRange* geometryGroup)
	{
		
		if (scene == nullptr) scene = tempScene;

		bool frontFaceCW = (object->isMesh) && (object->matrixWorld.determinant() < 0);

		auto program = setProgram(camera, scene, material,object);

		state->setMaterial(*material, frontFaceCW);

		/*auto updateBuffers = false;

		if (_currentGeometryProgram.geometry != geometry->id ||
			_currentGeometryProgram.program != program->id ||
			_currentGeometryProgram.wireframe != (material->wireframe ==  true)) {

			_currentGeometryProgram.geometry = geometry->id;
			_currentGeometryProgram.program = program->id;
			_currentGeometryProgram.wireframe = material->wireframe == true;
			updateBuffers = true;
		}

		
		
		if (material->morphTargets || material->morphNormals) {
			
			morphtargets->update(*object, *bufferGeometry, *material, *program);
			updateBuffers = true;
		}*/


		std::shared_ptr<BufferAttribute<unsigned>> index = geometry->index;
		std::shared_ptr<BufferAttribute<float>> position = geometry->getAttribute(AttributeName::position);
				
		if (index == nullptr) {
			if (position == nullptr || position->count == 0) return;
		}
		else if (index->count == 0) {
			return;
		}

		//

		auto rangeFactor = 1;

		if (material->wireframe ==  true) {

			index = geometries->getWireframeAttribute(geometry);
			rangeFactor = 2;

		}

		if (material->morphTargets || material->morphNormals)
		{
			morphtargets->update(*object, *geometry, *material, *program);
		}

		bindingStates->setUp(object, material, program, geometry, index);

		Attribute attribute;

		GLBufferRenderer* renderer = &bufferRenderer;
		bool isBufferRenderMode = true;
		if (index != nullptr) {

			attribute = attributes->get<unsigned>(*index);

			renderer =reinterpret_cast<GLBufferRenderer*>(&indexedBufferRenderer);
			isBufferRenderMode = false;
			//GLIndexedBufferRenderer indexRenderer = std::any_cast<GLIndexedBufferRenderer>(renderer);
			renderer->setIndex(attribute);
		}

		/*if (updateBuffers) {

			setupVertexAttributes(object, geometry, material, program);

			if (index !=  nullptr) {

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, attribute.buffer);

			}

		}*/

		

		auto dataCount = (index !=  nullptr) ? index->count : position->count;

		auto rangeStart = geometry->drawRange.start * rangeFactor;
		auto rangeCount = geometry->drawRange.count * rangeFactor;

		auto groupStart = geometryGroup != NULL ? geometryGroup->start * rangeFactor : 0;
		unsigned groupCount = geometryGroup != NULL ? geometryGroup->count * rangeFactor : (unsigned)std::numeric_limits<unsigned>::infinity;

		auto drawStart = std::max(rangeStart, groupStart);
		auto drawEnd =   std::min(std::min(dataCount, rangeStart + rangeCount), groupStart + groupCount) - 1;

		auto drawCount = std::max((unsigned)0, drawEnd - drawStart + 1);

		if (drawCount == 0) return;

		//

		if (instanceOf<Mesh>(object.get())) {

			if (material->wireframe == true) {

				state->setLineWidth(material->wireframeLinewidth * getTargetPixelRatio());
				renderer->setMode(GL_LINES);				
			}
			else {
				renderer->setMode(GL_TRIANGLES);				
				//renderer.setMode(_gl.TRIANGLES);
			}

		}
		else if (instanceOf<Line>(object.get())) {

			auto lineWidth = material->linewidth;

			if (lineWidth == 0) lineWidth = 1; // Not using Line*Material

			state->setLineWidth(lineWidth * getTargetPixelRatio());

			if (instanceOf<LineSegments>(object.get())) {

				renderer->setMode(GL_LINES);

			}
			else if (instanceOf<LineLoop>(object.get())) {

				renderer->setMode(GL_LINE_LOOP);

			}
			else {

				renderer->setMode(GL_LINE_STRIP);

			}

		}
		else if (instanceOf<Points>(object.get())) {

			renderer->setMode(GL_POINTS);

		}
		else if (instanceOf<Sprite>(object.get())) {

			renderer->setMode(GL_TRIANGLES);

		}

		if (instanceOf<InstancedMesh>(object.get())) {
			InstancedMesh::ptr instanceMesh = std::dynamic_pointer_cast<InstancedMesh>(object);
			renderer->renderInstances(*geometry, drawStart, drawCount, instanceMesh->count);

		}
		else if (instanceOf<InstancedBufferGeometry>(geometry.get())) {
			InstancedBufferGeometry::ptr instanceBufferGeometry = std::dynamic_pointer_cast<InstancedBufferGeometry>(object);
			renderer->renderInstances(*geometry, drawStart, drawCount, instanceBufferGeometry->maxInstancedCount);

		}
		else {

			renderer->render(drawStart, drawCount);

		}
		
	}
	void GLRenderer::compile(Scene& scene, Camera& camera)
	{
	}
	void GLRenderer::render(Scene::ptr& scene, const Camera::ptr& camera)
	{
		if (scene == nullptr || camera==nullptr) return;

		bindingStates->resetDefaultState();

		GLRenderTarget::ptr renderTarget = nullptr;

		bool forceClear = false;

		// reset caching for this frame

		_currentGeometryProgram.geometry = -1;
		_currentGeometryProgram.program = -1;
		_currentGeometryProgram.wireframe = false;
		_currentMaterialId = -1;
		_currentCamera = nullptr;

		// update scene graph
		if (scene->autoUpdate == true) scene->updateMatrixWorld();

		// update camera matrices and frustum
		if (camera->parent == nullptr) camera->updateMatrixWorld();
		
		//std::any _renderTarget = static_cast<std::any>(renderTarget.get());

		//scene->onBeforeRender.emitSignal(*this, *scene, *camera, *renderTarget,NULL);

		currentRenderState = renderStates->get(scene, camera);
		currentRenderState->init();

		_projScreenMatrix.multiplyMatrices(camera->projectionMatrix, camera->matrixWorldInverse);
		_frustum.setFromProjectionMatrix(_projScreenMatrix);

		_localClippingEnabled = localClippingEnabled;
		_clippingEnabled = _clipping.init(clippingPlanes, _localClippingEnabled, camera);

		currentRenderList = renderLists->get(scene, camera);
		currentRenderList->init();

		projectObject(scene, *camera, 0, sortObjects);

		currentRenderList->finish();

		if (sortObjects == true) {
			currentRenderList->sort(customOpaqueSort,customTransparentSort);
		}

		if (_clippingEnabled) _clipping.beginShadows();

		auto shadowsArray = currentRenderState->state.shadowsArray;

		shadowMap->render(*this, shadowsArray,scene, camera);

		currentRenderState->setupLights(camera);

		if (_clippingEnabled) _clipping.endShadows();

		if (info->autoReset) info->reset();

		if (renderTarget != nullptr) {
			setRenderTarget(renderTarget);
		}

		//autoClear = scene->clearBeforeRender;

		background->render(*this,*currentRenderList, *scene, *camera, forceClear);

		auto opaqueObjects = currentRenderList->opaque;
		auto transparentObjects = currentRenderList->transparent;

		if (opaqueObjects.size()) 
			renderObjects(opaqueObjects, scene, camera);
		
		if (transparentObjects.size()) 
			renderObjects(transparentObjects, scene, camera);			
		
		//scene->onAfterRender.emitSignal(*this, *scene, *camera,nullObject,NULL);

		if (_currentRenderTarget != nullptr) {
			// Generate mipmap if we're using any kind of mipmap filtering

			textures->updateRenderTargetMipmap(*_currentRenderTarget);

			// resolve multisample renderbuffers to a single-sample texture if necessary

			textures->updateMultisampleRenderTarget(*_currentRenderTarget);
		}
		// Ensure depth buffer writing is enabled so it can be cleared on next render

		state->depthBuffer.setTest(true);
		state->depthBuffer.setMask(true);
		state->colorBuffer.setMask(true);

		state->setPolygonOffset(false);
		
		currentRenderList.reset();
	}
	int GLRenderer::getActiveCubeFace()
	{
		return _currentActiveCubeFace;
	}
	int GLRenderer::getActiveMipmapLevel()
	{
		return _currentActiveMipmapLevel;
	}
	void GLRenderer::setFramebuffer(int value)
	{
	}
	GLRenderTarget::ptr GLRenderer::getRenderTarget()
	{
		return _currentRenderTarget;
	}

	void GLRenderer::setRenderTarget(const GLRenderTarget::ptr& renderTarget, int activeCubeFace, int activeMipmapLevel)
	{
		_currentRenderTarget = renderTarget;
		_currentActiveCubeFace = activeCubeFace;
		_currentActiveMipmapLevel = activeMipmapLevel;

		bool containFrameBuffer = false;
		if (renderTarget!=nullptr) {
			auto& ttable = properties->getProperties(renderTarget->uuid);
			if (ttable.framebuffer)
				containFrameBuffer = true;
		}
		if (renderTarget != nullptr && !containFrameBuffer) {
			textures->setupRenderTarget(*renderTarget);
		}

		auto framebuffer = _framebuffer;
		bool isCube = false;

		if (renderTarget != nullptr) {
			
			if (renderTarget->isGLCubeRenderTarget) {
				std::vector<GLint> __glFramebuffer = properties->getProperties(renderTarget->uuid).framebuffers;
				framebuffer = __glFramebuffer[activeCubeFace || 0];
				isCube = true;
			}
			else if (renderTarget->isGLMultisampleRenderTarget) {
				framebuffer = properties->getProperties(renderTarget->uuid).multisampledFramebuffer;
			}
			else {
				GLuint __glFramebuffer = properties->getProperties(renderTarget->uuid).framebuffer;
				framebuffer = __glFramebuffer;
			}
			_currentViewport.copy(renderTarget->viewport);
			_currentScissor.copy(renderTarget->scissor);
			_currentScissorTest = renderTarget->scissorTest;
		}
		else {
			_currentViewport.copy(_viewport).multiplyScalar(_pixelRatio).floor();
			_currentScissor.copy(_scissor).multiplyScalar(_pixelRatio).floor();
			_currentScissorTest = _scissorTest;
		}

		if (_currentFramebuffer != framebuffer) {
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer == 0? 0 : framebuffer);			
			_currentFramebuffer = framebuffer;
		}

		state->viewport(_currentViewport);
		state->scissor(_currentScissor);
		state->setScissorTest(_currentScissorTest);

		if (isCube) {
			auto& textureProperties = properties->getProperties(renderTarget->texture->uuid);
			GLuint glTexture = textureProperties.texture;
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + (activeCubeFace || 0), glTexture, activeMipmapLevel || 0);
		}

	}
	void GLRenderer::readRenderTargetPixels(const GLRenderTarget::ptr& renderTarget, float x, float y, float width, float height, int activeCubeFaceIndex)
	{
		if (!(renderTarget != nullptr && renderTarget->type=="GLRenderTarget"))
			return;

		unsigned framebuffer = properties->getProperties(renderTarget->uuid).framebuffer;

		if (renderTarget->isGLCubeRenderTarget && activeCubeFaceIndex > -1) {
			std::vector<GLint> __glFramebuffer = properties->getProperties(renderTarget->uuid).framebuffers;
			framebuffer = __glFramebuffer[activeCubeFaceIndex];
		}
		if (framebuffer > 0) {
			bool restore = false;

			if (framebuffer != _currentFramebuffer) {
				glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
				restore = true;
			}
			
			auto texture = renderTarget->texture;
			auto textureFormat = texture->format;
			auto textureType = texture->type;

			//if(textureFormat !=PixelFormat::RGBAFormat && )
		}
	}
	void GLRenderer::copyFramebufferToTexture(const Vector2& position, const Texture::ptr& texture, int level)
	{
		if (level == std::numeric_limits<int>::quiet_NaN()) level = 0;

		auto levelScale = (float)std::pow(2, -level);
		auto width = (float)std::floor(texture->imageWidth * levelScale);
		auto height = (float)std::floor(texture->imageHeight * levelScale);
		auto glFormat = texture->format;

		textures->setTexture2D(*texture, 0);

		glCopyTexImage2D(GL_TEXTURE_2D, level, (GLenum)glFormat, (int)position.x, (int)position.y, (int)width, (int)height, 0);
		state->unbindTexture();
	}
	void GLRenderer::copyTextureToTexture(const Vector2& position, const Texture::ptr& srcTexture, const Texture::ptr& dstTexture, int level)
	{
		if (level == std::numeric_limits<int>::quiet_NaN()) level = 0;

		auto width = srcTexture->imageWidth;
		auto height = srcTexture->imageHeight;
		auto glFormat = dstTexture->format;
		auto glType = dstTexture->type;

		textures->setTexture2D(*dstTexture, 0);

		// As another texture upload may have changed pixelStorei
		// parameters, make sure they are correct for the dstTexture
		//glPixelStorei(GL_UNPACK_FLIP_Y_WEBGL, dstTexture->flipY);
		//glPixelStorei(GL_UNPACK_PREMULTIPLY_ALPHA_WEBGL, dstTexture->premultiplyAlpha);
		glPixelStorei(GL_UNPACK_ALIGNMENT, dstTexture->unpackAlignment);

		if (instanceOf<DataTexture>(srcTexture.get())) {

			glTexSubImage2D(GL_TEXTURE_2D, level, position.x, position.y, width, height, (GLenum)glFormat, (GLenum)glType, &srcTexture->image[0]);

		}
		else {

			if (instanceOf<CompressedTexture>(srcTexture.get())) {

				glCompressedTexSubImage2D(GL_TEXTURE_2D, level, position.x, position.y, srcTexture->mipmaps[0].width, srcTexture->mipmaps[0].height, (GLenum)glFormat, srcTexture->mipmaps[0].data.size(),&srcTexture->mipmaps[0].data[0]);

			}
			else {

				glTexSubImage2D(GL_TEXTURE_2D, level, position.x, position.y, srcTexture->imageWidth,srcTexture->imageHeight,(GLenum)glFormat, (GLenum)glType, &srcTexture->image[0]);

			}

		}

		// Generate mipmaps only when copying level 0
		if (level == 0 && dstTexture->generateMipmaps) glGenerateMipmap(GL_TEXTURE_2D);

		state->unbindTexture();
	}
	void GLRenderer::initTexture(const Texture::ptr& texture)
	{
		textures->setTexture2D(*texture, 0);
		state->unbindTexture();
	}
	void GLRenderer::initGLParameter(void)
	{
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_SCISSOR_TEST);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);

		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ZERO);
		glBlendFuncSeparate(GL_ONE, GL_ZERO, GL_ONE, GL_ZERO);

		glColorMask(true, true, true, true);
		glClearColor(0, 0, 0, 0);

		glDepthMask(true);
		glDepthFunc(GL_LESS);
		glClearDepth(1);

		glStencilMask(0xffffffff);
		glStencilFunc(GL_ALWAYS, 0, 0xffffffff);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glClearStencil(0);

		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		glPolygonOffset(0, 0);

		glActiveTexture(GL_TEXTURE0);

		glBindFramebuffer(GL_FRAMEBUFFER, NULL);

		if (capabilities->isGL2 == true)
		{

			glBindFramebuffer(GL_DRAW_BUFFER, NULL);
			glBindFramebuffer(GL_READ_FRAMEBUFFER, NULL);

		}

		glUseProgram(0);

		glLineWidth(1);

		glScissor(0, 0, _width, _height);
		glViewport(0, 0, _width, _height);
	}
}
