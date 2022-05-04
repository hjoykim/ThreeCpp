#include "Lensflare.h"
#include <sstream>
#include <limits>
#include <three/core/InterleavedBufferAttribute.h>
#include <three/materials/MeshBasicMaterial.h>
#include <three/renderers/gl/GLRenderTarget.h>
#include <three/renderers/GLRenderer.h>
#include <three/scenes/Scene.h>
namespace three {
	LensflareElement::LensflareElement(Texture::ptr texture, float size, float distance, Color color)
		: GLShader(), texture(texture),size(size),distance(distance),color(color)
	{
		this->size = size != INF ? size : 1.0f;

		this->distance = distance != INF ? distance : 0.0f;

		this->color = !color.isNull() ? color : Color(0xffffff);

		LibUniformValues libUniforms = LibUniformValues({
			value<Texture::ptr>("map",nullptr),
			value<Texture::ptr>("occlusionMap",nullptr),
			value("color",Color()),
			value("scale",Vector2()),
			value("screenPosition",Vector3())
			});
		this->uniforms = UniformValues(libUniforms);
	
		vertexShader= 
		R"(				
		            uniform vec3 screenPosition;
		            uniform vec2 scale;
		            uniform sampler2D occlusionMap;
		            attribute vec3 position;
		            attribute vec2 uv;
		            varying vec2 vUV;
		            varying float vVisibility;
		            void main() {
		            	vUV = uv;
		            	vec2 pos = position.xy;
		            	vec4 visibility = texture2D( occlusionMap, vec2( 0.1, 0.1 ) );
		            	visibility += texture2D( occlusionMap, vec2( 0.5, 0.1 ) );
		            	visibility += texture2D( occlusionMap, vec2( 0.9, 0.1 ) );
		            	visibility += texture2D( occlusionMap, vec2( 0.9, 0.5 ) );
		            	visibility += texture2D( occlusionMap, vec2( 0.9, 0.9 ) );
		            	visibility += texture2D( occlusionMap, vec2( 0.5, 0.9 ) );
		            	visibility += texture2D( occlusionMap, vec2( 0.1, 0.9 ) );
		            	visibility += texture2D( occlusionMap, vec2( 0.1, 0.5 ) );
		            	visibility += texture2D( occlusionMap, vec2( 0.5, 0.5 ) );
		            	vVisibility =        visibility.r / 9.0;
		            	vVisibility *= 1.0 - visibility.g / 9.0;
		            	vVisibility *=       visibility.b / 9.0;
		            	gl_Position = vec4( ( pos * scale + screenPosition.xy ).xy, screenPosition.z, 1.0 );
		            }
		)";



		fragmentShader =
		R"(
		            uniform sampler2D map;
		            uniform vec3 color;
		            varying vec2 vUV;
		            varying float vVisibility;
		            void main() {
		        	   vec4 texture = texture2D( map, vUV );
		        	   texture.a *= vVisibility;
		        	   gl_FragColor = texture;
		        	   gl_FragColor.rgb *= color;
		            }
		)";


	}

	std::shared_ptr<BufferGeometry> Lensflare::lensflareGeometry()
	{
		std::vector<float> floatArray = {
				-1, -1, 0, 0, 0,
				1, -1, 0, 1, 0,
				1, 1, 0, 1, 1,
				-1, 1, 0, 0, 1
		};

		BufferGeometry::ptr geometry = BufferGeometry::create();
		InterleavedBuffer<float>::ptr interleavedBuffer = InterleavedBuffer<float>::create(floatArray,5);

		BufferAttribute<unsigned>::ptr index = BufferAttribute<unsigned>::create({ 0,1,2,0,2,3 }, 1);
		geometry->setIndex(index);
		geometry->setAttribute(AttributeName::position, InterleavedBufferAttribute<float>::create(interleavedBuffer, 3, 0, false));
		geometry->setAttribute(AttributeName::uv, InterleavedBufferAttribute<float>::create(interleavedBuffer, 2, 3, false));

		return geometry;
	}

	Lensflare::Lensflare() : Mesh()
	{
		frustumCulled = false;
		renderOrder = std::numeric_limits<int>::max();
		
		geometry = lensflareGeometry();
		material = MeshBasicMaterial::create();
		material->opacity = 0;
		material->transparent = true;

		type = "Lensflare";

		this->onBeforeRender.connect(*this, &Lensflare::beforeRender);

		std::vector<unsigned char> tempImage(16 * 16 * 4);
		std::vector<unsigned char> occlusionImage(16 * 16 * 4);
		
		tempMap = DataTexture::create(tempImage, 16, 16,TextureMapping::UVMapping,Wrapping::ClampToEdgeWrapping,Wrapping::ClampToEdgeWrapping,TextureFilter::NearestFilter,TextureFilter::NearestFilter, PixelFormat::RGBAFormat, TextureDataType::UnsignedByteType);
		tempMap->channel = 4;

		occlusionMap = DataTexture::create(occlusionImage, 16, 16, TextureMapping::UVMapping, Wrapping::ClampToEdgeWrapping, Wrapping::ClampToEdgeWrapping, TextureFilter::NearestFilter, TextureFilter::NearestFilter, PixelFormat::RGBAFormat, TextureDataType::UnsignedByteType);
		occlusionMap->channel = 4;
		material1a = RawShaderMaterial::create();
		material1a->uniforms->set("scale", Vector2());
		material1a->uniforms->set("screenPosition", Vector3());
		
		
		material1a->vertexShader = R"(
uniform vec3 screenPosition;
uniform vec2 scale;
attribute vec3 position;
void main() {
	gl_Position = vec4( position.xy * scale + screenPosition.xy, screenPosition.z, 1.0 );
}
)";
		material1a->fragmentShader = R"(
void main() {
	gl_FragColor = vec4( 1.0, 0.0, 1.0, 1.0 );
}
)";

		material1a->depthTest = true;
		material1a->depthWrite = false;
		material1a->transparent = false;

		material1b = RawShaderMaterial::create();		
		material1b->uniforms->set<Texture::ptr>("map", tempMap);
		material1b->uniforms->set("scale", Vector2());
		material1b->uniforms->set("screenPosition", Vector3());

		material1b->vertexShader = R"(
uniform vec3 screenPosition;
uniform vec2 scale;
attribute vec3 position;
attribute vec2 uv;
varying vec2 vUV;
void main() {
	vUV = uv;
    gl_Position = vec4( position.xy * scale + screenPosition.xy, screenPosition.z, 1.0 );
}
)";
		material1b->fragmentShader = R"(
uniform sampler2D map;
varying vec2 vUV;
void main() {
	gl_FragColor = texture2D( map, vUV );
}
)";

		material1b->depthTest = false;
		material1b->depthWrite = false;
		material1b->transparent = false;

		mesh1 = Mesh::create(geometry, material1a);

		auto shader = LensflareElement();
				
		material2 = RawShaderMaterial::create();
		material2->uniforms = std::make_shared<UniformValues>();
		material2->uniforms->set<Texture::ptr>("map", nullptr);
		material2->uniforms->set<Texture::ptr>("occlusionMap", occlusionMap);
		material2->uniforms->set("color", Color(0xffffff));
		material2->uniforms->set("scale", Vector2());
		material2->uniforms->set("screenPosition", Vector3());
		material2->vertexShader = shader.vertexShader;		
		material2->fragmentShader = shader.fragmentShader;

		material2->blending = Blending::AdditiveBlending;
		material2->transparent = true;
		material2->depthWrite = false;

		mesh2 = Mesh::create(geometry, material2);
	}
	
	void Lensflare::addElement(const LensflareElement::ptr& element)
	{
		elements.push_back(element);
	}

	void Lensflare::beforeRender(GLRenderer& renderer, const std::shared_ptr<Scene>& scene, const std::shared_ptr<Camera>& camera,const std::shared_ptr<Object3D>& object, const std::shared_ptr<BufferGeometry>& geometry, const std::shared_ptr<Material>& material, const std::shared_ptr<GLRenderTarget>& renderTarget, DrawRange* group)
	{
		renderer.getCurrentViewport(viewport);

		auto invAspect = viewport.w / viewport.z;
		auto halfViewportWidth = viewport.z / 2.0f;
		auto halfViewportHeight = viewport.w / 2.0f;

		auto size = 16 / viewport.w;
		scale.set(size * invAspect, size);

		validArea.min.set(viewport.x, viewport.y);
		validArea.max.set(viewport.x + (viewport.z - 16), viewport.y + (viewport.w - 16));

		// calculate position in screen space

		positionView.setFromMatrixPosition(this->matrixWorld);
		positionView.applyMatrix4(camera->matrixWorldInverse);

		if (positionView.z > 0) return; // lensflare is behind the camera

		positionScreen.copy(positionView).applyMatrix4(camera->projectionMatrix);

		// horizontal and vertical coordinate of the lower left corner of the pixels to copy

		screenPositionPixels.x = viewport.x + (positionScreen.x * halfViewportWidth) + halfViewportWidth - 8;
		screenPositionPixels.y = viewport.y + (positionScreen.y * halfViewportHeight) + halfViewportHeight - 8;

		// screen cull

		if (validArea.containsPoint(screenPositionPixels))
		{

			// save current RGB to temp texture

			renderer.copyFramebufferToTexture(screenPositionPixels, tempMap);

			// render pink quad

			material1a->uniforms->set("scale", scale);
			material1a->uniforms->set("screenPosition", positionScreen);

			Scene::ptr nullScene = nullptr;

			renderer.renderBufferDirect(camera, nullScene, geometry, material1a, mesh1, nullptr);

			// copy result to occlusionMap

			renderer.copyFramebufferToTexture(screenPositionPixels, occlusionMap);

			// restore graphics
			material1b->uniforms->set("scale", scale);
			material1b->uniforms->set("screenPosition", positionScreen);

			renderer.renderBufferDirect(camera, nullScene, geometry, material1b, mesh1, nullptr);

			// render elements

			auto vecX = -positionScreen.x * 2;
			auto vecY = -positionScreen.y * 2;

			for (int i = 0; i < elements.size(); i++)
			{

				auto& element = elements[i];

				
				material2->uniforms->set("color", element->color);
				material2->uniforms->set("map", element->texture);
				Vector3 prePosition = material2->uniforms->get("screenPosition").get<Vector3>();
				prePosition.x = positionScreen.x + vecX * element->distance;
				prePosition.y = positionScreen.y + vecY * element->distance;
				material2->uniforms->set("screenPosition", prePosition);						
				

				size = element->size / viewport.w;
				invAspect = viewport.w / viewport.z;

				material2->uniforms->set("scale", Vector2(size * invAspect, size));
				

				material2->uniformsNeedUpdate = true;

				renderer.renderBufferDirect(camera, nullScene, geometry, material2, mesh2, nullptr);

			}

		}
		
	}

}
