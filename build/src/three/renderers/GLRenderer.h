#ifndef GLRENDERER_THREE_H
#define GLRENDERER_THREE_H
#include <three/core/Object3D.h>
#include <three/scenes/Scene.h>
#include <three/cameras/Camera.h>
#include <three/cameras/PerspectiveCamera.h>
#include <three/materials/MeshBasicMaterial.h>
#include <three/materials/MeshStandardMaterial.h>
#include <three/materials/MeshLambertMaterial.h>
#include <three/materials/MeshPhongMaterial.h>
#include <three/materials/MeshToonMaterial.h>
#include <three/materials/MeshMatcapMaterial.h>
#include <three/materials/MeshDepthMaterial.h>
#include <three/materials/MeshDistanceMaterial.h>
#include <three/materials/MeshNormalMaterial.h>
#include <three/materials/LineBasicMaterial.h>
#include <three/materials/LineDashedMaterial.h>
#include <three/materials/PointsMaterial.h>
#include <three/materials/SpriteMaterial.h>
#include <three/materials/ShadowMaterial.h>
#include <three/materials/ShaderMaterial.h>
#include <three/materials/RawShaderMaterial.h>
#include <three/materials/MeshPhysicalMaterial.h>
#include <three/renderers/gl/GLRenderStates.h>
#include <three/renderers/gl/GLRenderLists.h>
#include <three/math/Plane.h>
#include <three/Constants.h>
#include <three/renderers/gl/GLRenderTarget.h>
#include <three/renderers/gl/GLShadowMap.h>
#include <three/renderers/gl/GLExtensions.h>
#include <three/renderers/gl/GLCapabilities.h>
#include <three/renderers/gl/GLState.h>
#include <three/renderers/gl/GLInfo.h>
#include <three/renderers/gl/GLProperties.h>
#include <three/renderers/gl/GLTextures.h>
#include <three/renderers/gl/GLAttributes.h>
#include <three/renderers/gl/GLGeometries.h>
#include <three/renderers/gl/GLObjects.h>
#include <three/renderers/gl/GLPrograms.h>
#include <three/renderers/gl/GLMorphtargets.h>
#include <three/renderers/gl/GLProperties.h>
#include <three/renderers/gl/GLBackground.h>
#include <three/renderers/gl/GLBufferRenderer.h>
#include <three/renderers/gl/GLIndexedBufferRenderer.h>
#include <three/renderers/gl/GLClipping.h>
#include <three/renderers/shaders/ShaderLib.h>
#include <three/renderers/gl/GLBindingStates.h>
#include <three/renderers/gl/GLCubeMap.h>
#include <three/renderers/gl/GLMaterials.h>
#include <functional>
namespace three {
	using namespace gl;

	class GLRenderer {
		struct GeometryProgram {
			int geometry;
			int program;
			bool wireframe;
			GeometryProgram() : geometry(0), program(0), wireframe(false) {}
		};

	private:
		GLRenderState::ptr currentRenderState;
		GLRenderList::ptr currentRenderList;
			
		GLCapabilities::GLCapabilitiesParameters capabilitiesParameter;

		Vector4 _viewport;
		Vector4 _scissor;

		Vector4 _currentViewport;
		Vector4 _currentScissor;
		bool _currentScissorTest;

		unsigned _frameBuffer;

		int _currentActiveCubeFace = 0;

		int _currentActiveMipmapLevel = 0;

		int _framebuffer = 0;

		

		int _currentFramebuffer;

		int _currentMaterialId = -1;

		GeometryProgram _currentGeometryProgram;

		Camera::ptr _currentCamera;

		ArrayCamera::ptr _currentArrayCamera;

		int _pixelRatio = 1;
		bool premultipliedAlpha = true;
		float _width;
		float _height;

		bool _scissorTest = false;

		Frustum _frustum;

		GLClipping _clipping;

		bool _clippingEnabled = false;
		bool _localClippingEnabled = false;

		Matrix4 _projScreenMatrix;

		Vector3 _vector3;


		Scene::ptr tempScene;

		GLBindingStates::ptr bindingStates;

		GLCubeMap::ptr cubeMaps;

		void initGLContext(int width,int height);

		int getTargetPixelRatio();

		void initMaterial(const Material::ptr& material, const Scene::ptr& scene, const Object3D::ptr& object);

		bool materialNeedsLights(Material& material);
		
		

		void markUniformsLightsNeedsUpdate(UniformValues& uniforms, bool value);
		
		void deallocateMaterial(Material& material);

		void releaseMaterialProgramReference(Material& material);

		void onMaterialDispose(Material& material);

		void projectObject(const std::shared_ptr<Object3D>& object, Camera& camera, int groupOrder, bool sortObjects = true);

		void renderObjects(const std::vector<RenderItem::ptr>& renderList, Scene::ptr& scene, const Camera::ptr& camera);

		void renderObject(const Object3D::ptr& object, Scene::ptr& scene, const Camera::ptr& camera, const BufferGeometry::ptr& geometry, const Material::ptr& material, DrawRange* group=NULL);

		void renderObjectImmediate(const Object3D::ptr& object, const GLProgram::ptr& program);
	public:		
		using ptr = std::shared_ptr<GLRenderer>;
		// clearing
		bool autoClear = true;
		bool autoClearColor = true;
		bool autoClearDepth = true;
		bool autoClearStencil = true;

		// scene graph
		bool sortObjects = true;

		// user-defined clipping

		std::vector<Plane> clippingPlanes;
		bool localClippingEnabled = false;

		// physically based shading

		float gammaFactor = 2.0f;	// for backwards compatibility
		TextureEncoding outputEncoding = TextureEncoding::LinearEncoding;

		// physical lights

		bool physicallyCorrectLights = false;

		// tone mapping

		ToneMapping toneMapping = ToneMapping::NoToneMapping;
		float toneMappingExposure = 1.0f;
		float toneMappingWhitePoint = 1.0f;

		// morphs

		int maxMorphTargets = 8;
		int maxMorphNormals = 4;
		
		GLRenderTarget::ptr _currentRenderTarget = nullptr;

		GLExtensions::ptr extensions;

		GLCapabilities::ptr capabilities;

		GLState::ptr state;

		GLInfo::ptr info;

		GLProperties::ptr properties;

		GLTextures::ptr textures;

		GLAttributes::ptr attributes;

		GLGeometries::ptr geometries;

		GLObjects::ptr objects;

		GLMorphtargets::ptr morphtargets;

		GLPrograms::ptr programCache;

		GLRenderLists::ptr renderLists;

		GLRenderStates::ptr renderStates;

		GLBackground::ptr background;

		GLBufferRenderer bufferRenderer;

		GLIndexedBufferRenderer indexedBufferRenderer;

		GLShadowMap::ptr shadowMap;

		GLMaterials materials;
		using ptr = std::shared_ptr<GLRenderer>;

		std::function<bool(RenderItem::ptr, RenderItem::ptr)> customOpaqueSort = nullptr;

		std::function<bool(RenderItem::ptr, RenderItem::ptr)> customTransparentSort = nullptr;

		GLRenderer(int width, int height);
		/*{
			initGLContext(width,height);
		};*/
		virtual ~GLRenderer();

		int getPixelRatio();
		void setPixelRatio(int value);

		Vector2& getDrawingBufferSize(Vector2& target);
		void setDrawingBufferSize(float width, float height, int pixelRatio);

		Vector2& getSize(Vector2& target);

		/**
		* Resizes the output canvas to (width, height), and also sets the viewport to fit that size, starting in (0, 0).
		*/
		void setSize(float width, float height, bool updateStyle = false);

		Vector4& getCurrentViewport(Vector4& target);


		/**
		 * Copies the viewport into target.
		 */
		Vector4& getViewport(Vector4& target);

		void setViewport(Vector4& v);

		/**
		* Sets the viewport to render from (x, y) to (x + width, y + height).
		* (x, y) is the lower-left corner of the region.
		*/
		void setViewport(float x, float y, float width, float height);
		
		
		/**
		* Copies the scissor area into target.
		*/
		Vector4& getScissor(Vector4& target);

		void setScissor(float x, float y, float width, float height);
		/**
		 * Sets the scissor area from (x, y) to (x + width, y + height).
		 */
		
		void setScissor(Vector4& v);

		/**
		 * Returns true if scissor test is enabled; returns false otherwise.
		 */
		bool getScissorTest();

		/**
		 * Enable the scissor test. When this is enabled, only the pixels within the defined scissor area will be affected by further renderer actions.
		 */
		void setScissorTest(bool enable);

		/**
		 * Sets the custom opaque sort function for the WebGLRenderLists. Pass null to use the default painterSortStable function.
		 */
		//void setOpaqueSort(method: Function);

		/**
		 * Sets the custom transparent sort function for the WebGLRenderLists. Pass null to use the default reversePainterSortStable function.
		 */
		//setTransparentSort(method: Function) : void;

		/**
		 * Returns a THREE.Color instance with the current clear color.
		 */
		Color& getClearColor();

		/**
		 * Sets the clear color, using color for the color and alpha for the opacity.
		 */
		void setClearColor(const Color& color, float alpha=std::numeric_limits<float>::quiet_NaN());

		void setClearColor(int color, float alpha=std::numeric_limits<float>::quiet_NaN() );


		/**
		 * Returns a float with the current clear alpha. Ranges from 0 to 1.
		 */
		int getClearAlpha();

		void setClearAlpha(float alpha);

		/**
		 * Tells the renderer to clear its color, depth or stencil drawing buffer(s).
		 * Arguments default to true
		 */
		void clear(bool color=true, bool depth=true, bool stencil=true);

		void clearColor();
		void clearDepth();
		void clearStencil();

		GLProgram::ptr setProgram(const Camera::ptr& camera, const Scene::ptr& scene, const Material::ptr& material, const Object3D::ptr& object);

		//void dispose();
		Signal<void(const GLRenderer&)> onDispose;

		void renderBufferImmediate(const Object3D::ptr& object,const GLProgram::ptr& program);

		void renderBufferDirect(const Camera::ptr& camera,Scene::ptr& scene,const BufferGeometry::ptr& geometry,const Material::ptr& material,const Object3D::ptr& object, DrawRange* geometryGroup=nullptr);

		/**
		 * A build in function that can be used instead of requestAnimationFrame. For WebXR projects this function must be used.
		 * @param callback The function will be called every available frame. If `null` is passed it will stop any already ongoing animation.
		 */
		//setAnimationLoop(callback: Function | null) : void;

		/**
		 * @deprecated Use {@link WebGLRenderer#setAnimationLoop .setAnimationLoop()} instead.
		 */
		//animate(callback: Function) : void;

		/**
		 * Compiles all materials in the scene with the camera. This is useful to precompile shaders before the first rendering.
		 */
		void compile(Scene& scene, Camera& camera);
		

		/**
		 * Render a scene using a camera.
		 * The render is done to a previously specified {@link WebGLRenderTarget#renderTarget .renderTarget} set by calling
		 * {@link WebGLRenderer#setRenderTarget .setRenderTarget} or to the canvas as usual.
		 *
		 * By default render buffers are cleared before rendering but you can prevent this by setting the property
		 * {@link WebGLRenderer#autoClear autoClear} to false. If you want to prevent only certain buffers being cleared
		 * you can set either the {@link WebGLRenderer#autoClearColor autoClearColor},
		 * {@link WebGLRenderer#autoClearStencil autoClearStencil} or {@link WebGLRenderer#autoClearDepth autoClearDepth}
		 * properties to false. To forcibly clear one ore more buffers call {@link WebGLRenderer#clear .clear}.
		 */
		void render(Scene::ptr& scene, const Camera::ptr& camera);
			

		/**
		 * Returns the current active cube face.
		 */
		int getActiveCubeFace();

		/**
		 * Returns the current active mipmap level.
		 */
		int getActiveMipmapLevel();

		/**
		 * Sets the given WebGLFramebuffer. This method can only be used if no render target is set via
		 * {@link WebGLRenderer#setRenderTarget .setRenderTarget}.
		 *
		 * @param value The WebGLFramebuffer.
		 */
		void setFramebuffer(int  value);

		/**
		 * Returns the current render target. If no render target is set, null is returned.
		 */
		GLRenderTarget::ptr getRenderTarget();
			

		/**
		 * Sets the active render target.
		 *
		 * @param renderTarget The {@link WebGLRenderTarget renderTarget} that needs to be activated. When `null` is given, the canvas is set as the active render target instead.
		 * @param activeCubeFace Specifies the active cube side (PX 0, NX 1, PY 2, NY 3, PZ 4, NZ 5) of {@link WebGLCubeRenderTarget}.
		 * @param activeMipmapLevel Specifies the active mipmap level.
		 */
		void setRenderTarget(const GLRenderTarget::ptr& renderTarget, int activeCubeFace=std::numeric_limits<int>::quiet_NaN(), int activeMipmapLevel = std::numeric_limits<int>::quiet_NaN());

		void readRenderTargetPixels(const GLRenderTarget::ptr& renderTarget, float x, float y, float width, float height, int activeCubeFaceIndex = std::numeric_limits<int>::quiet_NaN());
			

		/**
		 * Copies a region of the currently bound framebuffer into the selected mipmap level of the selected texture.
		 * This region is defined by the size of the destination texture's mip level, offset by the input position.
		 *
		 * @param position Specifies the pixel offset from which to copy out of the framebuffer.
		 * @param texture Specifies the destination texture.
		 * @param level Specifies the destination mipmap level of the texture.
		 */
		void copyFramebufferToTexture(const Vector2& position,const Texture::ptr& texture,int level=std::numeric_limits<int>::quiet_NaN());

		/**
		 * Copies srcTexture to the specified level of dstTexture, offset by the input position.
		 *
		 * @param position Specifies the pixel offset into the dstTexture where the copy will occur.
		 * @param srcTexture Specifies the source texture.
		 * @param dstTexture Specifies the destination texture.
		 * @param level Specifies the destination mipmap level of the texture.
		 */
		void copyTextureToTexture(const Vector2& position, const Texture::ptr& srcTexture, const Texture::ptr& dstTexture, int level = std::numeric_limits<int>::quiet_NaN());

		/**
		 * Initializes the given texture. Can be used to preload a texture rather than waiting until first render (which can cause noticeable lags due to decode and GPU upload overhead).
		 *
		 * @param texture The texture to Initialize.
		 */
		void initTexture(const Texture::ptr& texture);

		void initGLParameter(void);
	};
}
#endif