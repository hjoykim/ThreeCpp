#ifndef GL_BACKGROUND_THREE_H
#define GL_BACKGROUND_THREE_H
#include <three/math/Color.h>
#include <three/objects/Mesh.h>
#include <three/renderers/gl/GLState.h>
#include <three/renderers/gl/GLObjects.h>
using namespace three::math;
namespace three {
	class Scene;
	class Camera;
	class GLRenderer;

	namespace gl {
		class GLRenderList;
		class GLRenderTarget;		

		class GLBackground 
		{				
		private:
			void setClear(const Color& color, const float alpha);
		public:
			using ptr = std::shared_ptr<GLBackground>;

			GLState::ptr state;

			GLObjects::ptr objects;

			Color clearColor = Color().set(ColorKeywords::black);
			
			float clearAlpha = 0;

			Mesh::ptr planeMesh = nullptr;

			Mesh::ptr boxMesh = nullptr;

			void* currentBackground = nullptr;

			int currentBackgroundVersion = 0;

			ToneMapping currentTonemapping = ToneMapping::NoToneMapping;

			bool premultipliedAlpha=false;				

			GLBackground(GLState::ptr& state, GLObjects::ptr& objects, bool premultipliedAlpha) : state(state), objects(objects), premultipliedAlpha(premultipliedAlpha) {}
			virtual ~GLBackground() = default;

			Color& getClearColor();

			void setClearColor(const Color& color, const float alpha = 1);

			float getClearAlpha();

			void setClearAlpha(const float alpha);

			void render(three::GLRenderer& renderer, GLRenderList& renderList, Scene& scene, Camera& camera, bool forceClear = false);

			void beforeRender(GLRenderer& renderer, const std::shared_ptr<Scene>& scene, const std::shared_ptr<Camera>& camera, const std::shared_ptr<Object3D>& object,const std::shared_ptr<BufferGeometry>& geometry, const std::shared_ptr<Material>& material, const std::shared_ptr<GLRenderTarget>& renderTarget, DrawRange* group = nullptr);
		};
	}
}
#endif