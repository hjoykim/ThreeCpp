#ifndef LENSFLARE_THREE_H
#define LENSFLARE_THREE_H
#include <unordered_map>
#include <three/math/Box2.h>
#include <three/core/BufferGeometry.h>
#include <three/renderers/gl/GLShader.h>
#include <three/textures/DataTexture.h>
#include <three/objects/Mesh.h>
#include <three/renderers/shaders/UniformsLib.h>
#include <three/materials/RawShaderMaterial.h>

using namespace three::gl;
namespace three {
	class GLRenderer;

	namespace gl {
		class GLRenderTarget;
	}

	class LensflareElement : public GLShader {
	public:
		using ptr = std::shared_ptr<LensflareElement>;

		Texture::ptr texture;

		float size = 1;

		float distance = 0;

		Color color;

		LensflareElement(Texture::ptr texture = nullptr, float size = INF, float distance = INF, Color color = Color());

		static ptr create(Texture::ptr texture = nullptr, float size = INF, float distance = INF, Color color = Color()) {
			return std::make_shared<LensflareElement>(texture, size, distance, color);
		}
	};

	class Lensflare : public Mesh {
	private:		

		std::vector<LensflareElement::ptr> elements;

		Vector3 positionScreen;

		Vector3 positionView;

		Vector2 scale;

		Vector2 screenPositionPixels;

		Box2 validArea;

		Vector4 viewport;

		RawShaderMaterial::ptr material1a;

		RawShaderMaterial::ptr material1b;

		RawShaderMaterial::ptr material2;

		DataTexture::ptr tempMap;

		DataTexture::ptr occlusionMap;

		Mesh::ptr mesh1;

		Mesh::ptr mesh2;

		std::shared_ptr<BufferGeometry> lensflareGeometry();
	protected:
		//Signal<void(GLRenderer& renderer, Scene& scene, Camera& camera, Object3D& object, DrawRange* group)> onBeforeRender;
		void beforeRender(GLRenderer& renderer, const std::shared_ptr<Scene>& scene, const std::shared_ptr<Camera>& camera, const std::shared_ptr<Object3D>& obuject,const std::shared_ptr<BufferGeometry>& geometry, const std::shared_ptr<Material>& material, const std::shared_ptr<GLRenderTarget>& renderTarget,DrawRange* group = nullptr);
	public:
		std::unordered_map<std::string, LibUniformValues,StringHash> uniforms;

		using ptr = std::shared_ptr<Lensflare>;

		Lensflare();

		static ptr create() {
			return std::make_shared<Lensflare>();
		}
		virtual ~Lensflare() = default;

		void addElement(const LensflareElement::ptr& element);

		

	};
}
#endif