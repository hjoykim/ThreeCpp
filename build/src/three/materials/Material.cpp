#include "Material.h"
#include <three/utils/sole.h>
#include <three/renderers/shaders/ShaderLib.h>
namespace three {
	size_t Material::materialId = 0;
	Material::Material()
	{
		id = materialId++;
		uuid = sole::uuid4();
		name = "Material";
		type = "Material";

		fog = true;

		blending = Blending::NormalBlending;
		side = Side::FrontSide;
		flatShading = false;
		vertexColors = false;

		opacity = 1;
		transparent = false;

		blendSrc = BlendingDstFactor::SrcAlphaFactor;
		blendDst = BlendingDstFactor::OneMinusSrcAlphaFactor;
		blendEquation = BlendingEquation::AddEquation;
		blendSrcAlpha = BlendingDstFactor::None;
		blendDstAlpha = BlendingDstFactor::None;
		blendEquationAlpha = BlendingEquation::None;

		depthFunc = DepthModes::LessEqualDepth;
		depthTest = true;
		depthWrite = true;

		stencilWriteMask = 0xff;
		stencilFunc = StencilFunc::AlwaysStencilFunc;
		stencilRef = 0;
		stencilFuncMask = 0xff;
		stencilFail = StencilOp::KeepStencilOp;
		stencilZFail = StencilOp::KeepStencilOp;
		stencilZPass = StencilOp::KeepStencilOp;
		stencilWrite = false;
				
		clipIntersection = false;
		clipShadows = false;

		shadowSide = Side::None;

		colorWrite = true;

		precision = ""; // override the renderer's default precision for this material

		polygonOffset = false;
		polygonOffsetFactor = 0;
		polygonOffsetUnits = 0;

		dithering = false;

		alphaTest = 0;
		premultipliedAlpha = false;

		visible = true;

		toneMapped = true;

		//userData = {};

		version = 0;

	}
	Material::Material(const Material& source)
	{
		name = source.name;

		fog = source.fog;

		blending = source.blending;
		side = source.side;
		flatShading = source.flatShading;
		vertexColors = source.vertexColors;

		opacity = source.opacity;
		transparent = source.transparent;

		blendSrc = source.blendSrc;
		blendDst = source.blendDst;
		blendEquation = source.blendEquation;
		blendSrcAlpha = source.blendSrcAlpha;
		blendDstAlpha = source.blendDstAlpha;
		blendEquationAlpha = source.blendEquationAlpha;

		depthFunc = source.depthFunc;
		depthTest = source.depthTest;
		depthWrite = source.depthWrite;

		stencilWriteMask = source.stencilWriteMask;
		stencilFunc = source.stencilFunc;
		stencilRef = source.stencilRef;
		stencilFuncMask = source.stencilFuncMask;
		stencilFail = source.stencilFail;
		stencilZFail = source.stencilZFail;
		stencilZPass = source.stencilZPass;
		stencilWrite = source.stencilWrite;

		clippingPlanes.assign(source.clippingPlanes.begin(), source.clippingPlanes.end());

		//clippingPlanes = dstPlanes;
		clipIntersection = source.clipIntersection;
		clipShadows = source.clipShadows;

		shadowSide = source.shadowSide;

		colorWrite = source.colorWrite;

		precision = source.precision;

		polygonOffset = source.polygonOffset;
		polygonOffsetFactor = source.polygonOffsetFactor;
		polygonOffsetUnits = source.polygonOffsetUnits;

		dithering = source.dithering;

		alphaTest = source.alphaTest;
		premultipliedAlpha = source.premultipliedAlpha;

		visible = source.visible;

		toneMapped = source.toneMapped;

		definesString = source.definesString;
		/*if (source.defines.size() > 0) {
			defines.clear();
			defines.reserve(source.defines.size());
			std::copy(source.defines.begin(), source.defines.end(), defines.begin());
		}*/
	}
	Material* Material::clone()
	{
		return new Material(*this);
	}
	Material& Material::copy(const Material& source)
	{
		type = source.type;
		name = source.name;
		shaderId = source.shaderId;
		fog = source.fog;

		blending = source.blending;
		side = source.side;
		flatShading = source.flatShading;
		vertexColors = source.vertexColors;

		opacity = source.opacity;
		transparent = source.transparent;

		blendSrc = source.blendSrc;
		blendDst = source.blendDst;
		blendEquation = source.blendEquation;
		blendSrcAlpha = source.blendSrcAlpha;
		blendDstAlpha = source.blendDstAlpha;
		blendEquationAlpha = source.blendEquationAlpha;

		depthFunc = source.depthFunc;
		depthTest = source.depthTest;
		depthWrite = source.depthWrite;

		stencilWriteMask = source.stencilWriteMask;
		stencilFunc = source.stencilFunc;
		stencilRef = source.stencilRef;
		stencilFuncMask = source.stencilFuncMask;
		stencilFail = source.stencilFail;
		stencilZFail = source.stencilZFail;
		stencilZPass = source.stencilZPass;
		stencilWrite = source.stencilWrite;

		clippingPlanes.clear();

		clippingPlanes.assign(source.clippingPlanes.begin(), source.clippingPlanes.end());		

		//clippingPlanes = dstPlanes;
		clipIntersection = source.clipIntersection;
		clipShadows = source.clipShadows;

		shadowSide = source.shadowSide;

		colorWrite = source.colorWrite;

		precision = source.precision;

		polygonOffset = source.polygonOffset;
		polygonOffsetFactor = source.polygonOffsetFactor;
		polygonOffsetUnits = source.polygonOffsetUnits;

		dithering = source.dithering;

		alphaTest = source.alphaTest;
		premultipliedAlpha = source.premultipliedAlpha;

		visible = source.visible;

		toneMapped = source.toneMapped;

		//userData = JSON.parse(JSON.stringify(source.userData));

		return *this;
	}
}