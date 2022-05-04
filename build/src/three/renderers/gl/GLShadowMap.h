#ifndef GLSHADOWMAP_THREE_H
#define GLSHADOWMAP_THREE_H
#include <memory>
#include <three/scenes/Scene.h>
#include <three/cameras/Camera.h>
#include <three/Constants.h>
#include <three/math/Frustum.h>
#include <three/math/Vector2.h>
#include <three/math/Vector4.h>
#include <three/materials/ShaderMaterial.h>
#include <three/renderers/gl/GLObjects.h>
#include <three/objects/Mesh.h>
#include <three/lights/LightShadow.h>
#include <three/lights/Light.h>
using namespace three::math;
namespace three{	
	class GLRenderer;
	namespace gl {		
		class UniformValues;	

		class GLShadowMap {			
		private:
			Frustum _frustum;
			Vector2 _shadowMapSize;
			Vector2 _viewportSize;
			Vector4 _viewport;
			std::unordered_map<int, Material::ptr> _depthMaterials;
			std::unordered_map<int, Material::ptr> _distanceMaterials;
			
			std::unordered_map<std::string,std::unordered_map<std::string,Material::ptr>> _materialCache;

			std::vector<Side> shadowSide = { Side::BackSide,Side::FrontSide,Side::DoubleSide };
			
			ShaderMaterial::ptr shadowMaterialVertical = nullptr;

			ShaderMaterial::ptr shadowMaterialHorizontal = nullptr;

			int maxTextureSize;

			GLObjects::ptr objects;

			Mesh::ptr fullScreenMesh = nullptr;

			//std::function<Material::ptr(int useMorphing, int useSkinning, int useInstancing)> getMaterialVariant;

			//Material::ptr getDepthMaterialVariant(int useMorphing, int useSkinning, int useInstancing);
			
			//Material::ptr getDistanceMaterialVariant(int useMorphing, int useSkinning, int useInstancing);



			Material::ptr getDepthMaterial(GLRenderer& renderer,Object3D& object3D,const Geometry::ptr& geometry, const Material::ptr& material, Light& light, float shadowCameraNear,float shadowCameraFar, ShadowMapType type);

			void renderObject(GLRenderer& renderer, const Object3D::ptr& object, const Camera::ptr& camera, const Camera::ptr& shadowCamera, Light& light, ShadowMapType type);
			
			void VSMPass(GLRenderer& renderer,LightShadow& shadow, const Camera::ptr& camera);


			template<typename T>
			void addUniformsValue(UniformValues& uniforms, const std::string& key,const T& val) 
			{
				//GLUniform::ptr uniform = GLUniform::create("value", val);				
				uniforms.set(key, val);				
			}

		public:

			using ptr = std::shared_ptr<GLShadowMap>;

			bool enabled = false;

			bool autoUpdate = true;

			bool needsUpdate = false;

			ShadowMapType type;

			
			GLShadowMap(GLObjects::ptr& objects, int maxTextureSize);

			virtual ~GLShadowMap() = default;

			void render(GLRenderer& renderer,const std::vector<Light::ptr>& lights,Scene::ptr& scene, const Camera::ptr& camera);
			
		};
	}
}
#endif