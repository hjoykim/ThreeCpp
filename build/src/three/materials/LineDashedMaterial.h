#ifndef LINE_DASHED_MATERIAL_THREE_H
#define LINE_DASHED_MATERIAL_THREE_H
#include <three/materials/LineBasicMaterial.h>
namespace three {
	class LineDashedMaterial : public LineBasicMaterial {
		float scale = 1.0f;
		float dashSize = 3.0f;
		float gapSize = 1.0f;
	public:
		LineDashedMaterial() : LineBasicMaterial(),scale(1), dashSize(3), gapSize(1) {
			this->type = "LineDashedMaterial";
			this->name = "LineDashedMaterial";
			shaderId = "dashed";
		}
		LineDashedMaterial(const LineDashedMaterial& source) :LineBasicMaterial(source) {
			this->type = "LineDashedMaterial";
			this->name = "LineDashedMaterial";
			scale = source.scale;
			dashSize = source.dashSize;
			gapSize = source.gapSize;
		}
		virtual ~LineDashedMaterial() = default;

		virtual LineDashedMaterial* clone() {
			return new LineDashedMaterial(*this);
		}
		virtual LineDashedMaterial& copy(const LineDashedMaterial& source) {
			LineBasicMaterial::copy(source);
			scale = source.scale;
			dashSize = source.dashSize;
			gapSize = source.gapSize;
		}
	};
}
#endif