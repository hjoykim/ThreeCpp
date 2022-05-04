#ifndef SCENE_THREE_H
#define SCENE_THREE_H
#include <three/core/Object3D.h>
#include <three/scenes/Fog.h>
#include <three/textures/Texture.h>
#include <three/textures/CubeTexture.h>
#include <three/renderers/GLCubeRenderTarget.h>
namespace three {	
	using namespace gl;
	class Scene : public Object3D {
	private:
		
		GLCubeRenderTarget::ptr cubeRenderTarget;
		Texture::ptr texture;
		CubeTexture::ptr cubeTexture;
		Color color;
		
	public:
		bool isColor = false;
		bool isCubeTexture = false;
		bool isGLCubeRenderTarget = false;
		bool isTexture = false;
		
		using ptr = std::shared_ptr<Scene>;
		
		Fog::ptr fog = nullptr;

		Material::ptr overrideMaterial = nullptr;

		bool autoUpdate = true;

		Texture::ptr environment;

		//const void* background =nullptr;
		//const void* environment = nullptr;

		Scene() :Object3D() {
			type = "Scene";
		}

		virtual ~Scene() {}
		//{
		//	this->onDisposed.emitSignal(*this);
		//}
		Scene(const Scene& source) : Object3D(source) {		
			type = "Scene";
			fog = source.fog;
			overrideMaterial = source.overrideMaterial;
			fog = source.fog;
			autoUpdate = source.autoUpdate;
		}
		virtual Scene* clone(bool recursive=true) override {
			return new Scene(*this);
		}

		virtual Scene& copy(const Scene& source, bool recursive = true) {
			Object3D::copy(source, recursive);
			
			fog = source.fog;
			overrideMaterial = source.overrideMaterial;
			fog = source.fog;
			autoUpdate = source.autoUpdate;

			return *this;
		}
		static ptr create() {
			return std::make_shared<Scene>();
		}	

		Texture::ptr& getBackgroundTexture() {
			return texture;
		}
		CubeTexture::ptr& getBackgroundCubeTexture() {
			return cubeTexture;
		}
		GLCubeRenderTarget::ptr& getBackgroundCubeRenderTarget() {
			return cubeRenderTarget;
		}

		Color& getBackgroundColor() {
			return color;
		}
		
		void setBackgroundColor(Color& value) {
			color = value;
			isColor = true;
			isTexture = false;
			isCubeTexture = false;
			isGLCubeRenderTarget = false;
		}
		void setBackgroundTexture(Texture::ptr& value) {
			texture = value;
			isColor = false;
			isTexture = true;
			isCubeTexture = false;
			isGLCubeRenderTarget = false;
		}
		void setBackgroundCubeTexture(CubeTexture::ptr& value) {
			cubeTexture = value;
			isColor = false;
			isTexture = false;
			isCubeTexture = true;
			isGLCubeRenderTarget = false;
		}
		
		void setBackgroundCubeRenderTarget(GLCubeRenderTarget::ptr& value) {

			cubeRenderTarget = value;
			isColor = false;
			isTexture = false;
			isCubeTexture = false;
			isGLCubeRenderTarget = true;

		}

		bool hasBackground() const {
			return isColor || isTexture || isCubeTexture || isGLCubeRenderTarget;
		}

		Signal<void(const Scene& scene)> onDisposed;
				
	};
}
#endif