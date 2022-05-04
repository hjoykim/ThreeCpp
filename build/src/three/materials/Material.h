#ifndef MATERIAL_THREE_H
#define MATERIAL_THREE_H
#include <three/utils/osdecl.h>
#include <three/utils/simplesignal.h>
#include <three/utils/sole.h>
#include <three/Constants.h>
#include <three/math/Plane.h>
#include <three/math/Color.h>
#include <three/math/Vector2.h>
#include <three/Textures/Texture.h>
#include <vector>
#include <string>
//#include <three/renderers/shaders/ShaderLib.h>
namespace three {
	using namespace math;

	namespace gl {
		class GLShader;
		class GLUniforms;
		class GLProgram;
		class UniformValues;
	}
	using namespace gl;
	
	class GLRenderer;
	
	class Material {
		static size_t materialId;

	public:		
		struct ShaderMaterialExtension {
			bool derivatives = false;
			bool fragDepth = false;
			bool drawBuffers = false;
			bool shaderTextureLOD = false;
		};
		using ptr = std::shared_ptr<Material>;
		using UniformValuesPtr= std::shared_ptr<UniformValues>;

		Texture::ptr alphaMap = nullptr;

		std::string shaderId;
		/**
		* Sets the alpha value to be used when running an alpha test. Default is 0.
		*/
		float alphaTest = 0;

		Texture::ptr aoMap=nullptr;

		float aoMapIntensity;

		/**
		 * Blending destination. It's one of the blending mode constants defined in Three.js. Default is {@link OneMinusSrcAlphaFactor}.
		 */
		BlendingDstFactor blendDst = BlendingDstFactor::OneMinusSrcAlphaFactor;

		/**
		 * The tranparency of the .blendDst. Default is null.
		 */
		BlendingDstFactor blendDstAlpha = BlendingDstFactor::None;

		/**
		 * Blending equation to use when applying blending. It's one of the constants defined in Three.js. Default is {@link AddEquation}.
		 */
		BlendingEquation blendEquation = BlendingEquation::AddEquation;

		/**
		 * The tranparency of the .blendEquation. Default is null.
		 */
		BlendingEquation blendEquationAlpha = BlendingEquation::None;

		/**
		 * Which blending to use when displaying objects with this material. Default is {@link NormalBlending}.
		 */
		Blending blending = Blending::NormalBlending;

		/**
		 * Blending source. It's one of the blending mode constants defined in Three.js. Default is {@link SrcAlphaFactor}.
		 */
		BlendingDstFactor blendSrc = BlendingDstFactor::SrcAlphaFactor;;

		/**
		 * The tranparency of the .blendSrc. Default is null.
		 */
		BlendingDstFactor blendSrcAlpha = BlendingDstFactor::None;

		Texture::ptr bumpMap = nullptr;

		float bumpScale;

		Combine combine;


		float clearcoat;

		Texture::ptr clearcoatMap = nullptr;

		float clearcoatRoughness;

		Texture::ptr clearcoatRoughnessMap = nullptr;

		Vector2 clearcoatNormalScale;

		Texture::ptr clearcoatNormalMap=nullptr;

		/**
		 * Changes the behavior of clipping planes so that only their intersection is clipped, rather than their union. Default is false.
		 */
		bool clipIntersection = false;

		/**
		 * User-defined clipping planes specified as THREE.Plane objects in world space. These planes apply to the objects this material is attached to. Points in space whose signed distance to the plane is negative are clipped (not rendered). See the WebGL / clipping /intersection example. Default is null.
		 */
		std::vector<three::math::Plane> clippingPlanes;

		/**
		 * Defines whether to clip shadows according to the clipping planes specified on this material. Default is false.
		 */
		bool clipShadows = false;

		/**
		 * Material Color
		 */
		
		Color color;
		/**
		 * Whether to render the material's color. This can be used in conjunction with a mesh's .renderOrder property to create invisible objects that occlude other objects. Default is true.
		 */
		bool colorWrite = true;

		float dashSize;

		/**
		 * Custom defines to be injected into the shader. These are passed in form of an object literal, with key/value pairs. { MY_CUSTOM_DEFINE: '' , PI2: Math.PI * 2 }.
		 * The pairs are defined in both vertex and fragment shaders. Default is undefined.
		 */
		std::unordered_map<std::string, std::string> definesString;
		std::unordered_map<std::string, float> definesFloat;

		/**
		 * Which depth function to use. Default is {@link LessEqualDepth}. See the depth mode constants for all possible values.
		 */
		DepthModes depthFunc = DepthModes::LessDepth;

		/**
		 * Whether to have depth test enabled when rendering this material. Default is true.
		 */
		bool depthTest = true;

		/**
		 * Whether rendering this material has any effect on the depth buffer. Default is true.
		 * When drawing 2D overlays it can be useful to disable the depth writing in order to layer several things together without creating z-index artifacts.
		 */
		bool depthWrite = true;

		Texture::ptr displacementMap = nullptr;

		DepthPackingStrategies depthPacking;

		float displacementScale;

		float displacementBias;

		Texture::ptr emissiveMap=nullptr;

		Color emissive;

		float emissiveIntensity;

		Texture::ptr envMap=nullptr;

		float envMapIntensity;
		/**
		 * Whether the material is affected by fog. Default is true.
		 */
		bool fog = true;

		float gapSize;

		/**
		 * Unique number of this material instance.
		 */
		unsigned id;

		Texture::ptr lightMap=nullptr;

		float lightMapIntensity;

		float linewidth=1;

		std::string linecap;

		std::string linejoin;

		Texture::ptr map=nullptr;

		Texture::ptr matcap=nullptr;

		float nearDistance;

		float farDistance;

		Texture::ptr normalMap=nullptr;

		NormalMapTypes normalMapType = NormalMapTypes::None;

		Vector2 normalScale;

		Texture::ptr gradientMap=nullptr;

		Vector3 referencePosition;

		float reflectivity;

		float refractionRatio;

		Texture::ptr roughnessMap = nullptr;

		Texture::ptr metalnessMap = nullptr;

		float roughness;

		float metalness;


		float scale;

		Texture::ptr specularMap = nullptr;

		Color specular;

		float shininess;

		Color sheen; // shared_ptr<Color>

		Texture::ptr transmissionMap = nullptr;

		float transparency;

		/**
	   * Whether rendering this material has any effect on the stencil buffer. Default is *false*.
	   */
		bool stencilWrite = false;

		unsigned stencilWriteMask = 0xff;

		unsigned stencilFuncMask = 0xff;
		/**
	   * The stencil comparison function to use. Default is {@link AlwaysStencilFunc}. See stencil operation constants for all possible values.
	   */
		StencilFunc stencilFunc = StencilFunc::AlwaysStencilFunc;

		/**
	   * The value to use when performing stencil comparisons or stencil operations. Default is *0*.
	   */
		unsigned stencilRef = 0;
			

		/**
	   * Which stencil operation to perform when the comparison function returns false. Default is {@link KeepStencilOp}. See the stencil operation constants for all possible values.
	   */
		StencilOp stencilFail = StencilOp::KeepStencilOp;

		/**
	   * Which stencil operation to perform when the comparison function returns true but the depth test fails. Default is {@link KeepStencilOp}. See the stencil operation constants for all possible values.
	   */
		StencilOp stencilZFail = StencilOp::KeepStencilOp;

		/**
	   * Which stencil operation to perform when the comparison function returns true and the depth test passes. Default is {@link KeepStencilOp}. See the stencil operation constants for all possible values.
	   */
		StencilOp stencilZPass = StencilOp::KeepStencilOp;

		/**
		 * Used to check whether this or derived classes are materials. Default is true.
		 * You should not change this, as it used internally for optimisation.
		 */
		//readonly isMaterial : true;

		bool morphTargets=false;

		bool morphNormals=false;

		bool vertexTangents = false;
		/**
		 * Material name. Default is an empty string.
		 */
		std::string name;

		/**
		 * Specifies that the material needs to be updated, WebGL wise. Set it to true if you made changes that need to be reflected in WebGL.
		 * This property is automatically set to true when instancing a new material.
		 */
		bool needsUpdate;

		/**
		 * Opacity. Default is 1.
		 */
		float opacity = 1;

		/**
		 * Whether to use polygon offset. Default is false. This corresponds to the POLYGON_OFFSET_FILL WebGL feature.
		 */
		bool polygonOffset = false;

		/**
		 * Sets the polygon offset factor. Default is 0.
		 */
		unsigned polygonOffsetFactor = 0.0f;

		/**
		 * Sets the polygon offset units. Default is 0.
		 */
		unsigned polygonOffsetUnits = 0.0f;

		/**
		 * Override the renderer's default precision for this material. Can be "highp", "mediump" or "lowp". Defaults is null.
		 */
		std::string precision;//: 'highp' | 'mediump' | 'lowp' | null;

		/**
		 * Whether to premultiply the alpha (transparency) value. See WebGL / Materials / Transparency for an example of the difference. Default is false.
		 */
		bool premultipliedAlpha=false;

		/**
		 * Whether to apply dithering to the color to remove the appearance of banding. Default is false.
		 */
		bool dithering = false;

		/**
		 * Define whether the material is rendered with flat shading. Default is false.
		 */
		bool flatShading = false;

		/**
		 * Defines which of the face sides will be rendered - front, back or both.
		 * Default is THREE.FrontSide. Other options are THREE.BackSide and THREE.DoubleSide.
		 */
		Side side = Side::FrontSide;

		/**
		 * Defines which of the face sides will cast shadows. Default is *null*.
		 * If *null*, the value is opposite that of side, above.
		 */
		Side shadowSide = Side::None;

		bool skinning = false;

		/**
		 * Defines whether this material is tone mapped according to the renderer's toneMapping setting.
		 * Default is true.
		 */
		bool toneMapped = true;

		/**
		 * Defines whether this material is transparent. This has an effect on rendering as transparent objects need special treatment and are rendered after non-transparent objects.
		 * When set to true, the extent to which the material is transparent is controlled by setting it's .opacity property.
		 * Default is false.
		 */
		bool transparent=false;

		/**
		 * Value is the string 'Material'. This shouldn't be changed, and can be used to find all objects of this type in a scene.
		 */
		std::string type;

		/**
		 * UUID of this material instance. This gets automatically assigned, so this shouldn't be edited.
		 */
		sole::uuid uuid;

		/**
		 * Defines whether vertex coloring is used. Default is false.
		 */
		bool vertexColors = false;

		/**
		 * Defines whether this material is visible. Default is true.
		 */
		bool visible = true;

		/**
		 * An object that can be used to store custom data about the Material. It should not hold references to functions as these will not be cloned.
		 */
		//userData: any;
		bool wireframe = false;

		float wireframeLinewidth;

		std::string wireframeLinecap;

		std::string wireframeLinejoin;

		float size = 1;

		float rotation;

		bool sizeAttenuation = false;

		bool isShaderMaterial = false;

		bool isRawShaderMaterial = false;


		bool lights = true; // set to use scene lights

		bool clipping = false; // set to use user-defined clipping planes
		/**
		 * This starts at 0 and counts how many times .needsUpdate is set to true.
		 */
		int version;

		UniformValuesPtr uniforms;

		bool uniformsNeedUpdate = false;

		std::string vertexShader;/* =
			R"(void main() {"
			"   gl_Position = projectionMatrix*modelViewMatrix*vec4(position,1.0);\n"
			"})";*/
		std::string fragmentShader;/* =
			R"(void main() {"
			"   gl_FragColor = vec4(1.0,0.0,0.0,1.0);\n"
			"})";*/

		
		std::shared_ptr<GLProgram> program;

		int numSupportedMorphTargets = 0;

		int numSupportedMorphNormals = 0;

		GLSLVersion glslVersion = GLSLVersion::GLSL3;

		std::string indexOfAttributeName="";

		ShaderMaterialExtension extensions;

		std::string customProgramCacheKey = "";

		Material();

		Material(const Material& source);

		static ptr create() {
			return std::make_shared<Material>();
		}
		virtual ~Material() = default;
		/**
		 * Return a new material with the same parameters as this material.
		 */
		virtual Material* clone();

		/**
		 * Copy the parameters from the passed material into this material.
		 * @param material
		 */
		virtual Material& copy(const Material& source);


		/**
		 * An optional callback that is executed immediately before the shader program is compiled. This function is called with the shader source code as a parameter. Useful for the modification of built-in materials.
		 * @param shader Source code of the shader
		 * @param renderer WebGLRenderer Context that is initializing the material
		 */
		//onBeforeCompile(shader : Shader, renderer : WebGLRenderer) : void;

		/**
		 * Sets the properties based on the values.
		 * @param values A container with parameters.
		 */
		//setValues(values: MaterialParameters) : void;

		Signal<void(const gl::GLShader&,const GLRenderer&)> onBeforeCompile;

		Signal<void(Material&)> onDispose;
		/**
		 * This disposes the material. Texture::ptrs of a material don't get disposed. These needs to be disposed by {@link Texture::ptr}.
		 */
		virtual void dispose() {
			onDispose.emitSignal(*this);
			onDispose.disconnectAll();
		}
	};
}
#endif