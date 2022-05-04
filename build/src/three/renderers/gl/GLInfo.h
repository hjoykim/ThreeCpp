#ifndef GLINFO_THREE_H
#define GLINFO_THREE_H
#include <vector>
#include <memory>
namespace three {
	namespace gl {
		class GLProgram;

		using programPtr = std::shared_ptr<GLProgram>;
		
		class GLInfo {
			struct Memory {
				unsigned geometries;
				unsigned textures;
				Memory() :geometries(0), textures(0) {}
			};

			struct Render {
				unsigned frame;
				unsigned calls;
				unsigned triangles;
				unsigned points;
				unsigned lines;
				Render() :frame(0), calls(0), triangles(0), points(0), lines(0) {}
			};
		public:
			using ptr = std::shared_ptr<GLInfo>;
			Memory memory;

			Render render;

			std::vector<programPtr> programs;

			bool autoReset = true;

			GLInfo() {}

			//virtual ~GLInfo() = default;

			void update(const unsigned count, const unsigned mode, unsigned instanceCount = 0);
			void reset();
		};
	}
}
#endif