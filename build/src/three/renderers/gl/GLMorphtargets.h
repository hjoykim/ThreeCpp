#ifndef GL_MORPHTARGETS_THREE_H
#define GL_MORPHTARGETS_THREE_H
#include <vector>
#include <unordered_map>
#include <memory>
#include <cstdlib>
#include <cmath>
namespace three {
	class Object3D;
	class BufferGeometry;
	class Material;
	
	namespace gl {
		class GLProgram;
		class GLMorphtargets {
		public:
			using ptr = std::shared_ptr<GLMorphtargets>;

			std::unordered_map<unsigned, std::vector<std::vector<size_t>>> influencesList;
			std::vector<float> morphInfluences;
			
			std::vector<std::vector<size_t>> workInfluences;

			GLMorphtargets() {
				influencesList.clear();
				morphInfluences.resize(8);
				for (unsigned int i = 0; i < 8; i++) {					
					workInfluences.push_back({i,0 });
				}
			}
			GLMorphtargets(const GLMorphtargets& source) {
				influencesList = source.influencesList;
				morphInfluences = source.morphInfluences;
			}

			GLMorphtargets* clone() {
				return new GLMorphtargets(*this);
			}

			GLMorphtargets& copy(const GLMorphtargets& source) {
				influencesList = source.influencesList;
				morphInfluences = source.morphInfluences;
				return *this;
			}
			virtual ~GLMorphtargets() = default;

			void update(Object3D& object, BufferGeometry& geometry, Material& material, GLProgram& program);

		};

		inline bool absNumericalSort(std::vector<size_t> a, std::vector<size_t> b) {

			return std::abs((long long)b[1]) - std::abs((long long)a[1]);

		}
		inline bool numericalSort(std::vector<size_t> a, std::vector<size_t> b) {
			return a[0] - b[0];
		}
	}
}
#endif