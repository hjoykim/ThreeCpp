#ifndef GLMATERIALS_THREE_H
#define GLMATERIALS_THREE_H
#include <three/renderers/gl/GLProperties.h>
namespace three {
	namespace gl {
		class GLMaterials {
		private :
			GLProperties::ptr properties;

			void refreshUniformsPhong(UniformValues& uniforms, Material& material);

			void refreshUniformsPhysical(UniformValues& uniforms, Material& material);

			void refreshUniformsStandard(UniformValues& uniforms, Material& material);

			void refreshUniformsMatcap(UniformValues& uniforms, Material& material);

			void refreshUniformsDepth(UniformValues& uniforms, Material& material);

			void refreshUniformsDistance(UniformValues& uniforms, Material& material);

			void refreshUniformsNormal(UniformValues& m_uniforms, Material& material);

			void refreshUniformsLine(UniformValues& uniforms, Material& material);

			void refreshUniformsDash(UniformValues& uniforms, Material& material);

			void refreshUniformsPoints(UniformValues& uniforms, Material& material,float _pixelRatio,float _height);

			void refreshUniformsSprites(UniformValues& uniforms, Material& material);

			void refreshUniformsCommon(UniformValues& uniforms, Material& material);

			void refreshUniformsLambert(UniformValues& uniforms, Material& material);

			void refreshUniformsToon(UniformValues& uniforms, Material& material);

		public:			

			GLMaterials() {}

			GLMaterials(const GLProperties::ptr properties) : properties(properties) {}

			~GLMaterials() = default;
						
			void refreshMaterialUniforms(UniformValues& m_uniforms, const Material::ptr& material, float pixelRatio, float height);

			void refreshUniformsFog(UniformValues& uniforms, Fog& fog);
			
		};
	}
}
#endif