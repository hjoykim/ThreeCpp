#include "GLMorphtargets.h"
#include <three/core/Object3D.h>
#include <three/core/BufferGeometry.h>
#include <three/materials/Material.h>
#include <three/renderers/gl/GLProgram.h>

namespace three {
	namespace gl {
		void GLMorphtargets::update(Object3D& object, BufferGeometry& geometry, Material& material, GLProgram& program)
		{
			auto objectInfluences = object.morphTargetInfluences;

			// When object doesn't have morph target influences defined, we treat it as a 0-length array
			// This is important to make sure we set up morphTargetBaseInfluence / morphTargetInfluences

			unsigned length = objectInfluences.size();

			
			std::vector<std::vector<size_t>> influences = influencesList[geometry.id];


			if (influencesList.empty() || influencesList.count(geometry.id) == 0) {

				// initialise list

				influences = std::vector<std::vector<size_t>>();

				for (size_t i = 0; i < length; i++) {
					
					influences.push_back({i, 0 });

				}

				influencesList[geometry.id] = influences;

			}

			// Collect influences
			for (int i = 0; i < length; i++) {
				auto influence = influences[i];
				influence[0] = i;
				influence[1] = objectInfluences[i];

			}

			std::sort(influences.begin(), influences.end(), absNumericalSort);

			for (int i = 0; i < 8; i++) {
				if (i < length&& influences[i][1]) {
					workInfluences[i][0] = influences[i][0];
					workInfluences[i][1] = influences[i][1];
				}
				else {
					workInfluences[i][0] = std::numeric_limits<unsigned>::max();
					workInfluences[i][1] = 0;
				}
			}
			std::sort(workInfluences.begin(), workInfluences.end(), numericalSort);



			auto morphTargets = material.morphTargets && geometry.morphAttributes.count("position") > 0 ? &geometry.morphAttributes["position"] : nullptr;
			auto morphNormals = material.morphNormals && geometry.morphAttributes.count("normal") > 0 ? &geometry.morphAttributes["normal"] : nullptr;

			size_t morphInfluencesSum = 0;
			

			for (auto i = 0; i < length; i++) {

				auto influence = workInfluences[i];
				auto index = influence[0];
				auto value = influence[1];

				if (index != std::numeric_limits<unsigned>::max() && value) {
					if (morphTargets!=nullptr && geometry.getAttribute(AttributeName::morphTarget, i) != morphTargets->at(index)) {
						geometry.setAttribute(AttributeName::morphTarget, i, morphTargets->at(index));
					}
					
					if (morphNormals!=nullptr && geometry.getAttribute(AttributeName::morphNormal, i) != morphNormals->at(index)) {
						geometry.setAttribute(AttributeName::morphNormal, i, morphNormals->at(index));
					}

					morphInfluences[i] = value;
					morphInfluencesSum += value;
				}
				
				else {
					if (morphTargets!=nullptr && geometry.getAttribute(AttributeName::morphTarget, i) != nullptr) {
						geometry.deleteAttribute(AttributeName::morphTarget, i);
					}

					if (morphNormals!=nullptr && geometry.getAttribute(AttributeName::morphNormal, i) != nullptr) {
						geometry.deleteAttribute(AttributeName::morphNormal, i);
					}

					morphInfluences[i] = 0;
				}
			}

			

			// GLSL shader uses formula baseinfluence * base + sum(target * influence)
			// This allows us to switch between absolute morphs and relative morphs without changing shader code
			// When baseinfluence = 1 - sum(influence), the above is equivalent to sum((target - base) * influence)
			auto morphBaseInfluence = geometry.morphTargetsRelative ? 1 : (int)(1 - morphInfluencesSum);
			program.cachedUniforms->setUniformValue("morphTargetBaseInfluence", morphBaseInfluence);
			program.cachedUniforms->setUniformValue("morphTargetInfluences", morphInfluences);
			
		}
	}
}
