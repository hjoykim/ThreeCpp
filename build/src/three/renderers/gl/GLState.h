#ifndef GLSTATE_THREE_H
#define GLSTATE_THREE_H
#include <three/Constants.h>
#include <three/math/Vector4.h>
#include <three/materials/Material.h>
#include <three/Types.h>
#include <vector>
#include <unordered_map>
#include <memory>
namespace three {
	namespace gl {
		class GLState {
		public:
			using ptr = std::shared_ptr<GLState>;
			struct ColorBuffer {
				bool locked = false;
				GLboolean currentColorMask = 0;
				math::Vector4 currentColorClear = { 0,0,0,0 };

				ColorBuffer& setMask(bool _colorMask) {
					GLboolean colorMask = _colorMask ? 1 : 0;
					if (currentColorMask != colorMask && !locked) {
						glColorMask(colorMask, colorMask, colorMask, colorMask);
						currentColorMask = colorMask;
					}
					return *this;
				}
				ColorBuffer& setLocked(bool locked) {
					this->locked = locked;
					return *this;
				}

				ColorBuffer& setClear(float r, float g, float b, float a, bool premultipliedAppliedAlpha = false) {
					if (premultipliedAppliedAlpha) {
						r *= a;
						g *= a;
						b *= a;
					}
					math::Vector4 color(r, g, b, a);

					if (currentColorClear != color) {
						glClearColor(r, g, b, a);
						currentColorClear.copy(color);
					}

					return *this;
				}
				void reset() {
					locked = false;
					currentColorMask = 0;
					currentColorClear.set(-1, 0, 0, 0);
				}
			};
			struct DepthBuffer {
				GLState& state;

				bool locked = false;

				GLboolean currentDepthMask = false;

				DepthModes currentDepthFunc = DepthModes::LessEqualDepth;

				double currentDepthClear = 0.0f;

				DepthBuffer(GLState& state) : state(state) {}

				DepthBuffer& copy(const DepthBuffer& source) {
					locked = source.locked;
					currentDepthMask = source.currentDepthMask;
					currentDepthClear = source.currentDepthClear;
					state = source.state;
					return *this;
				}
				DepthBuffer& operator = (const DepthBuffer& source) {
					return copy(source);
				}

				DepthBuffer& setTest(bool depthTest) {
					if (depthTest) {
						state.enable(GL_DEPTH_TEST);
					}
					else {
						state.disable(GL_DEPTH_TEST);
					}

					return *this;
				}
				DepthBuffer& setMask(GLboolean depthMask) {
					if (currentDepthMask != depthMask && !locked) {
						glDepthMask(depthMask);
						currentDepthMask = depthMask;
					}
					return *this;
				}

				DepthBuffer& setFunc(DepthModes depthFunc) {
					if (currentDepthFunc != depthFunc) {
						glDepthFunc((GLenum)depthFunc);
						currentDepthFunc = depthFunc;
					}
					return *this;
				}
				DepthBuffer& setLocked(bool locked) {
					this->locked = locked;
					return *this;
				}

				DepthBuffer& setClear(double depth) {
					if (currentDepthClear != depth) {
						glClearDepth(depth);
						currentDepthClear = depth;
					}
					return *this;
				}
				void reset() {
					locked = false;

					currentDepthMask = false;
					currentDepthFunc = DepthModes::NeverDepth;
					currentDepthClear = 0;

				}
			};
			struct StencilBuffer {
				GLState& state;

				bool locked = false;

				GLuint currentStencilMask = 0;
				StencilFunc currentStencilFunc = StencilFunc::NeverStencilFunc;
				GLint currentStencilRef = 0;
				GLuint currentStencilFuncMask = 0;
				StencilOp currentStencilFail = StencilOp::ZeroStencilOp;
				StencilOp currentStencilZFail = StencilOp::ZeroStencilOp;
				StencilOp currentStencilZPass = StencilOp::ZeroStencilOp;
				GLint currentStencilClear = 0;

				StencilBuffer(GLState& state) : state(state) {}

				StencilBuffer& copy(const StencilBuffer& source) {
					state = source.state;
					locked = source.locked;

					currentStencilMask = source.currentStencilMask;
					currentStencilFunc = source.currentStencilFunc;
					currentStencilRef = source.currentStencilRef;
					currentStencilFuncMask = source.currentStencilFuncMask;
					currentStencilFail = source.currentStencilFail;
					currentStencilZFail = source.currentStencilZFail;
					currentStencilZPass = source.currentStencilZPass;
					currentStencilClear = source.currentStencilClear;

					return *this;
				}
				StencilBuffer& operator = (const StencilBuffer& source) {
					return copy(source);
				}
				StencilBuffer& setTest(bool stencilTest) {
					if (!locked) {
						if (stencilTest) {
							glEnable(GL_STENCIL_TEST);
						}
						else {
							glDisable(GL_STENCIL_TEST);
						}
					}
					return *this;
				}

				StencilBuffer& setMask(GLuint stencilMask) {
					if (currentStencilMask != stencilMask && !locked) {
						glStencilMask(stencilMask);
						currentStencilMask = stencilMask;
					}
					return *this;
				}

				StencilBuffer& setFunc(StencilFunc stencilFunc, GLint stencilRef, GLuint stencilMask) {
					if (currentStencilFunc != stencilFunc ||
						currentStencilRef != stencilRef ||
						currentStencilFuncMask != stencilMask) {

						glStencilFunc((GLenum)stencilFunc, stencilRef, stencilMask);

						currentStencilFunc = stencilFunc;
						currentStencilRef = stencilRef;
						currentStencilFuncMask = stencilMask;						
					}
					return *this;
				}

				StencilBuffer& setOp(StencilOp stencilFail, StencilOp stencilZFail, StencilOp stencilZPass) {
					if (currentStencilFail != stencilFail ||
						currentStencilZFail != stencilZFail ||
						currentStencilZPass != stencilZPass) {

						glStencilOp((GLenum)stencilFail, (GLenum)stencilZFail, (GLenum)stencilZPass);
						currentStencilFail = stencilFail;
						currentStencilZFail = stencilZFail;
						currentStencilZPass = stencilZPass;
					}
					return *this;
				}
				StencilBuffer& setLocked(bool lock) {
					locked = lock;
					return *this;
				}
				StencilBuffer& setClear(GLint stencil) {
					if (currentStencilClear != stencil) {
						glClearStencil(stencil);
						currentStencilClear = stencil;
					}
					return *this;
				}

				void reset() {
					locked = false;

					currentStencilFuncMask = 0;
					currentStencilFunc = StencilFunc::NeverStencilFunc;
					currentStencilRef = 0;
					currentStencilFuncMask = 0;
					currentStencilFail = StencilOp::ZeroStencilOp;
					currentStencilZFail = StencilOp::ZeroStencilOp;
					currentStencilZPass = StencilOp::ZeroStencilOp;
					currentStencilClear =0;
					
				}
			};
			
			struct BoundTexture {
				TextureTarget target;
				GLint texture;

				BoundTexture(TextureTarget target, GLint texture) : target(target), texture(texture) {}
				BoundTexture() :target(TextureTarget::Texture2D), texture(-1) {}
			};
			ColorBuffer colorBuffer = ColorBuffer();
			DepthBuffer depthBuffer = DepthBuffer(*this);
			StencilBuffer stencilBuffer = StencilBuffer(*this);

			GLint maxVertexAttributes;

			std::vector<GLuint> newAttributes;
			std::vector<GLuint> enabledAttributes;
			std::vector<GLuint> attributeDivisors;


			std::unordered_map<GLenum, bool> enabledCapabilities;

			GLuint currentProgram = 0;

			bool currentBlendingEnabled = false;
			Blending currentBlending = Blending::NoBlending;
			BlendingEquation currentBlendEquation = BlendingEquation::None;
			BlendingDstFactor currentBlendSrc = BlendingDstFactor::None;
			BlendingDstFactor currentBlendDst = BlendingDstFactor::None;
			BlendingEquation currentBlendEquationAlpha = BlendingEquation::None;
			BlendingDstFactor currentBlendSrcAlpha = BlendingDstFactor::None;
			BlendingDstFactor currentBlendDstAlpha = BlendingDstFactor::None;
			bool currentPremultipliedAlpha = false;

			bool currentFlipSided = false;
			CullFace currentCullFace = CullFace::CullFaceNone;
			float currentLineWidth;
			float currentPolygonOffsetFactor;
			float currentPolygonOffsetUnits;
			GLint maxTextures;

			bool lineWidthAvailable = false;
			GLuint version = 0;
			std::string glVersion;

			int currentTextureSlot;

			math::Vector4 currentScissor;
			math::Vector4 currentViewport;

			std::unordered_map<int, BoundTexture> currentBoundTextures;
			std::unordered_map<GLuint, GLuint> emptyTextures;
			std::vector<int> compressedTextureFormats;

			GLState() : depthBuffer(*this),stencilBuffer(*this),currentTextureSlot(-1)
			{
				glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTextures);
				glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttributes);

				lineWidthAvailable = true;

				newAttributes.resize(maxVertexAttributes);
				enabledAttributes.resize(maxVertexAttributes);
				attributeDivisors.resize(maxVertexAttributes);

				emptyTextures[(GLuint)TextureTarget::Texture2D] = createTexture(TextureTarget::Texture2D, TextureTarget::Texture2D, 1);
				emptyTextures[(GLuint)TextureTarget::cubeMap] = createTexture(TextureTarget::cubeMap, TextureTarget::cubeMapNegativeX, 6);

				colorBuffer.setClear(0, 0, 0, 1);
				depthBuffer.setClear(1);
				stencilBuffer.setClear(0);
				

				enable(GL_DEPTH_TEST);
				depthBuffer.setFunc(DepthModes::LessEqualDepth);

				setFlipSided(false);
				setCullFace(CullFace::CullFaceBack);
				enable(GL_CULL_FACE);

				setBlending(Blending::NoBlending);
			}
			virtual ~GLState() = default;

			GLState& copy(const GLState& source) {
				colorBuffer = source.colorBuffer;
				depthBuffer = source.depthBuffer;
				stencilBuffer = source.stencilBuffer;

				maxVertexAttributes = source.maxVertexAttributes;

				if (source.newAttributes.size() > 0) {
					newAttributes.resize(source.newAttributes.size());
					std::copy(source.newAttributes.begin(), source.newAttributes.end(), newAttributes.begin());
				}
				if (source.enabledAttributes.size() > 0) {
					enabledAttributes.resize(source.enabledAttributes.size());
					std::copy(source.enabledAttributes.begin(), source.enabledAttributes.end(), enabledAttributes.begin());
				}

				if (source.attributeDivisors.size() > 0) {
					attributeDivisors.resize(source.attributeDivisors.size());
					std::copy(source.attributeDivisors.begin(), source.attributeDivisors.end(), attributeDivisors.begin());
				}
								
				
				enabledCapabilities = source.enabledCapabilities;				
				

				currentProgram = source.currentProgram;

				currentBlendingEnabled = source.currentBlendingEnabled;
				currentBlending = source.currentBlending;
				currentBlendEquation = source.currentBlendEquation;
				currentBlendSrc = source.currentBlendSrc;
				currentBlendDst = source.currentBlendDst;
				currentBlendEquationAlpha = source.currentBlendEquationAlpha;
				currentBlendSrcAlpha = source.currentBlendSrcAlpha;
				currentBlendDstAlpha = source.currentBlendDstAlpha;
				currentPremultipliedAlpha = source.currentPremultipliedAlpha;

				currentFlipSided = source.currentFlipSided;
				currentCullFace = source.currentCullFace;
				currentLineWidth = source.currentLineWidth;
				currentPolygonOffsetFactor = source.currentPolygonOffsetFactor;
				currentPolygonOffsetUnits = source.currentPolygonOffsetUnits;
				maxTextures = source.maxTextures;

				lineWidthAvailable = source.lineWidthAvailable;
				version = source.version;
				glVersion = source.glVersion;
				currentTextureSlot = source.currentTextureSlot;

				currentScissor = source.currentScissor;
				currentViewport = source.currentViewport;

				currentBoundTextures = source.currentBoundTextures;
				emptyTextures = source.emptyTextures;

				if (source.compressedTextureFormats.size() > 0)
					compressedTextureFormats.resize(source.compressedTextureFormats.size());
				std::copy(source.compressedTextureFormats.begin(), source.compressedTextureFormats.end(),compressedTextureFormats.begin());			

				return *this;
			}
			GLState& operator = (const GLState& source) {
				return copy(source);
			}
			GLuint createTexture(TextureTarget type, TextureTarget target, unsigned count) {
				byte data[4] = { 0,0,0,0 };
				GLuint texture;
				glGenTextures(1, &texture);
				glBindTexture((GLenum)type, texture);
				glTexParameteri((GLenum)type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri((GLenum)type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

				for (unsigned i = 0;i < count;i++) {
					glTexImage2D((GLenum)target + i, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				}

				return texture;
			}
			void initAttributes() {
				for (unsigned i = 0;i< newAttributes.size(); i++) {
					newAttributes[i] = 0;
				}
			}

			void enableAttribute(GLuint attribute) {
				enableAttributeAndDivisor(attribute, 0);
			}
			void enableAttributeAndDivisor(GLuint attribute, GLuint meshPerAttribute) {
				newAttributes[attribute] = 1;

				if (enabledAttributes[attribute] == 0) {
					glEnableVertexAttribArray(attribute);
					enabledAttributes[attribute] = 1;
				}

				if (attributeDivisors[attribute] != meshPerAttribute) {
					//glVertexAttribDivisor(attribute, meshPerAttribute);
					attributeDivisors[attribute] = meshPerAttribute;
				}
			}
			void disableUnusedAttributes() {
				for (unsigned i = 0;i< enabledAttributes.size(); ++i) {

					if (enabledAttributes[i] !=  newAttributes[i]) {

						glDisableVertexAttribArray(i);
						enabledAttributes[i] = 0;
					}
				}
			}
			void vertexAttribPointer(GLuint index,GLint size,GLenum type,bool normalized,GLuint stride,GLuint offset) {
				//if(type==GL_INT || type==GL_UNSIGNED_INT)
				//	glVertexAttribIPointer(index, size, type, normalized,(const void*)offset);
				//else
					glVertexAttribPointer(index, size, type, normalized, stride, (void*)offset);
			}
			void enable(GLenum id) {
				if (!enabledCapabilities[id]) {
					glEnable(id);
					enabledCapabilities[id] = true;
				}
			}
			
			void disable(GLenum id) {
				if (enabledCapabilities[id]) {
					glDisable(id);
					enabledCapabilities[id] = false;
				}
			}

			bool useProgram(GLuint program) {
				if (currentProgram != program) {
					glUseProgram(program);
					currentProgram = program;
					return true;
				}
				return false;
			}

			void setBlending(Blending blending, BlendingEquation blendEquation=BlendingEquation::None, BlendingDstFactor blendSrc=BlendingDstFactor::None, BlendingDstFactor blendDst=BlendingDstFactor::None, BlendingEquation blendEquationAlpha=BlendingEquation::None,BlendingDstFactor blendSrcAlpha=BlendingDstFactor::None,BlendingDstFactor blendDstAlpha=BlendingDstFactor::None, bool premultipliedAlpha = false) {
				
				if (blending == Blending::NoBlending) {
					if (currentBlendingEnabled) {
						disable(GL_BLEND);
						currentBlendingEnabled = false;
						return;
					}
					return;
				}
				

				if (!currentBlendingEnabled) {
					enable(GL_BLEND);
					currentBlendingEnabled = true;
				}
				if (blending != Blending::CustomBlending) {
					if (blending != currentBlending || premultipliedAlpha != currentPremultipliedAlpha) {
						if (currentBlendEquation != BlendingEquation::AddEquation || currentBlendEquationAlpha != BlendingEquation::AddEquation) {
							glBlendEquation((GLenum)BlendingEquation::AddEquation);
							currentBlendEquation = BlendingEquation::AddEquation;
							currentBlendEquationAlpha = BlendingEquation::AddEquation;
						}

						if (premultipliedAlpha) {
							switch (blending) {
								case Blending::NormalBlending:
									glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
									break;
								case Blending::AdditiveBlending:
									glBlendFunc(GL_ONE, GL_ONE);
									break;

								case Blending::SubtractiveBlending:
									glBlendFuncSeparate(GL_ZERO, GL_ZERO, GL_ONE_MINUS_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);
									break;

								case Blending::MultiplyBlending:
									glBlendFuncSeparate(GL_ZERO, GL_SRC_COLOR, GL_ZERO, GL_SRC_ALPHA);
									break;														
							}
						}
						else {
							switch (blending) {

							case Blending::NormalBlending:
								glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
								break;

							case Blending::AdditiveBlending:
								glBlendFunc(GL_SRC_ALPHA, GL_ONE);
								break;

							case Blending::SubtractiveBlending:
								glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
								break;

							case Blending::MultiplyBlending:
								glBlendFunc(GL_ZERO, GL_SRC_COLOR);
								break;						
							}
						}
						currentBlendSrc = BlendingDstFactor::None;
						currentBlendDst = BlendingDstFactor::None;
						currentBlendSrcAlpha = BlendingDstFactor::None;
						currentBlendDstAlpha = BlendingDstFactor::None;

						currentBlending = blending;
						currentPremultipliedAlpha = premultipliedAlpha;
					}
					return;
				}
				//custom blending
				blendEquationAlpha = blendEquationAlpha!=BlendingEquation::None ? blendEquationAlpha:blendEquation;
				blendSrcAlpha = blendSrcAlpha!=BlendingDstFactor::None? blendSrcAlpha : blendSrc;
				blendDstAlpha = blendDstAlpha!=BlendingDstFactor::None? blendDstAlpha : blendDst;

				if (blendEquation !=  currentBlendEquation || blendEquationAlpha !=  currentBlendEquationAlpha) {

					glBlendEquationSeparate((GLenum)blendEquation, (GLenum)blendEquationAlpha);

					currentBlendEquation = blendEquation;
					currentBlendEquationAlpha = blendEquationAlpha;

				}

				if (blendSrc != currentBlendSrc || blendDst !=  currentBlendDst || blendSrcAlpha !=  currentBlendSrcAlpha || blendDstAlpha !=  currentBlendDstAlpha) {

					glBlendFuncSeparate((GLenum)blendSrc, (GLenum)blendDst, (GLenum)blendSrcAlpha, (GLenum)blendDstAlpha);

					currentBlendSrc = blendSrc;
					currentBlendDst = blendDst;
					currentBlendSrcAlpha = blendSrcAlpha;
					currentBlendDstAlpha = blendDstAlpha;

				}

				currentBlending = blending;
				currentPremultipliedAlpha = false;
			}
			void setMaterial(const Material& material, bool frontFaceCW=false) {
				//material.side == Side::DoubleSide ? disable(GL_CULL_FACE) : enable(GL_CULL_FACE);

				if (material.side == Side::DoubleSide) {
					glDisable(GL_CULL_FACE);
				}
				else {
					glEnable(GL_CULL_FACE);
				}

				bool flipSided = material.side == Side::BackSide;

				if (frontFaceCW) flipSided = !flipSided;

				setFlipSided(flipSided);

				if (material.blending == Blending::NormalBlending && material.transparent == false) {
					setBlending(Blending::NoBlending);
				}
				else {
					setBlending(material.blending, material.blendEquation, material.blendSrc, material.blendDst, material.blendEquationAlpha, material.blendSrcAlpha, material.blendDstAlpha, material.premultipliedAlpha);
				}

				depthBuffer.setFunc(material.depthFunc);
				depthBuffer.setTest(material.depthTest);
				depthBuffer.setMask(material.depthWrite);
				colorBuffer.setMask(material.colorWrite);

				if(material.depthTest)
					glEnable(GL_DEPTH_TEST);
				else
					glDisable(GL_DEPTH_TEST);

				auto stencilWrite = material.stencilWrite;

				stencilBuffer.setTest(stencilWrite);

				/*if (stencilWrite) {
					glEnable(GL_STENCIL_TEST);
				}
				else {
					glDisable(GL_STENCIL_TEST);
				}*/

				if (stencilWrite) {
					stencilBuffer.setMask(material.stencilWriteMask);
					stencilBuffer.setFunc(material.stencilFunc, material.stencilRef, material.stencilFuncMask);
					stencilBuffer.setOp(material.stencilFail, material.stencilZFail, material.stencilZPass);
				}

				setPolygonOffset(material.polygonOffset, material.polygonOffsetFactor, material.polygonOffsetUnits);
			}

			void setFlipSided(bool flipSided) {
				if (currentFlipSided != flipSided) {
					if (flipSided) {
						glFrontFace(GL_CW);
					}
					else {
						glFrontFace(GL_CCW);
					}
					currentFlipSided = flipSided;
				}
			}
			void setCullFace(CullFace cullFace) {
				if (cullFace != CullFace::CullFaceNone) {

					enable(GL_CULL_FACE);

					if (cullFace != currentCullFace) {
						if (cullFace == CullFace::CullFaceBack) {
							glCullFace(GL_BACK);
						}
						else if (cullFace == CullFace::CullFaceFront) {
							glCullFace(GL_FRONT);
						}
						else {
							glCullFace(GL_FRONT_AND_BACK);
						}
					}
				}
				else {
					disable(GL_CULL_FACE);
				}
				currentCullFace = cullFace;
			}
			
			void setLineWidth(float width) {

				if (width != currentLineWidth) {

					if (lineWidthAvailable) glLineWidth(width);

					currentLineWidth = width;

				}

			}
			void setPolygonOffset(bool polygonOffset, float factor=std::numeric_limits<float>::quiet_NaN(), float units=std::numeric_limits<float>::quiet_NaN()) {

				if (polygonOffset) {

					enable(GL_POLYGON_OFFSET_FILL);

					if ((!std::isnan(factor) && currentPolygonOffsetFactor != factor) || (!std::isnan(units) && currentPolygonOffsetUnits != units)) {

						glPolygonOffset(factor, units);

						currentPolygonOffsetFactor = factor;
						currentPolygonOffsetUnits = units;
					}
				}
				else {
					disable(GL_POLYGON_OFFSET_FILL);
				}

			}
			void setScissorTest(bool scissorTest) {

				if (scissorTest) {
					enable(GL_SCISSOR_TEST);
				}
				else {
					disable(GL_SCISSOR_TEST);
				}
			}

			void activeTexture(int glSlot=-1) {
				if (glSlot == -1) glSlot = GL_TEXTURE0 + maxTextures - 1;
				if (currentTextureSlot !=  glSlot) {
					glActiveTexture(glSlot);
					currentTextureSlot = glSlot;
				}
			}

			void bindTexture(TextureTarget target, GLint texture = -1) {

				if (currentTextureSlot <0) {
					activeTexture();
				}

				/*if (currentTextureSlot == 33985)
					std::cout << "CurrentTextureSlot :" << currentTextureSlot << " texture:" << texture << std::endl;*/

				BoundTexture* boundTexture;

				if (texture == 0)
					std::cout << "Texture is 0" << std::endl;

				auto found = currentBoundTextures.find(currentTextureSlot);
				if (found == currentBoundTextures.end()) {
					currentBoundTextures.emplace(currentTextureSlot, BoundTexture(TextureTarget::None, -1));
					boundTexture = &currentBoundTextures[currentTextureSlot];
				}
				else {
					boundTexture = &found->second;
				}						

				if (boundTexture->target != target || boundTexture->texture !=  texture) {
					if(texture>0)
						glBindTexture((GLenum)target, (GLuint)texture);					
					else
						glBindTexture((GLenum)target,emptyTextures[(GLuint)target]);

					boundTexture->target = target;
					boundTexture->texture = texture;

				}
			}
			void unbindTexture() {

				BoundTexture* boundTexture;
				auto found = currentBoundTextures.find(currentTextureSlot);
				if (found != currentBoundTextures.end()) {
					boundTexture = &found->second;
					glBindTexture((GLenum)boundTexture->target, 0);
					
					boundTexture->target = TextureTarget::None;
					boundTexture->texture = -1;
				}			
			}
			void compressedTexImage2D(GLenum target, GLint level, GLint internalFormat,GLsizei width, GLsizei height, const std::vector<unsigned char>& data)
			{
				glCompressedTexImage2D(target, level, internalFormat, width, height, 0, data.size(), data.data());
			}

			void texImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border,GLenum format, GLenum type, byte* pixels)
			{
				glTexImage2D(target, level, internalFormat, width, height, border, format, type, (const void*)pixels);
			}
			void texImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const std::vector<unsigned char>& pixels)
			{
				glTexImage2D(target, level, internalFormat, width, height, border, format, type, (const void*) &pixels[0]);
			}

			void texImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type)
			{
				glTexImage2D(target, level, internalFormat, width, height, border, format, type, nullptr);
			}

			void texImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const MipMap& mipmap)
			{
				glTexImage2D(target, level, internalFormat, width, height, border, format, type, mipmap.data.data());
			}

			void texImage3D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth,GLint border,GLenum format,GLenum type, byte* pixels) 
			{
				glTexImage3D(target, level, format, width, height, depth, border, format, type, pixels);
			}

			void texImage3D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const MipMap& mipmap)
			{
				glTexImage3D(target, level, (GLenum)format, width, height, depth, border, (GLenum)format, (GLenum)type, mipmap.data.data());
			}

			void texImage3D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const std::vector<byte>& pixels)
			{
				glTexImage3D(target, level, format, width, height, depth, border, format, type, &pixels[0]);
			}

			void scissor(const math::Vector4& scissor) {
				if (currentScissor != scissor) {
					glScissor((GLint)scissor.x, (GLint)scissor.y, (GLsizei)scissor.z, (GLsizei)scissor.w);
					currentScissor.copy(scissor);
				}
			}

			void viewport(const math::Vector4& viewport) {
				if (currentViewport != viewport) {
					glViewport((GLint)viewport.x, (GLint)viewport.y, (GLsizei)viewport.z, (GLsizei)viewport.w);
					currentViewport.copy(viewport);
				}
			}

			void reset() {
				for (unsigned i = 0; i < enabledAttributes.size(); i++) {
					glDisableVertexAttribArray(i);
					enabledAttributes[i] = 0;
				}

				enabledCapabilities.clear();

				compressedTextureFormats.clear();

				currentTextureSlot = -1;

				currentBoundTextures.clear();

				currentProgram = -1;

				currentBlending = Blending::NoBlending;

				currentFlipSided = false;

				currentCullFace = CullFace::CullFaceNone;

				colorBuffer.reset();
				depthBuffer.reset();
				stencilBuffer.reset();
			}

		};
	}
}
#endif