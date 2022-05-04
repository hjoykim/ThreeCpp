#ifndef INSTANCED_BUFFERGEOMETRY_THREE_H
#define INSTANCED_BUFFERGEOMETRY_THREE_H
#include <three/core/BufferGeometry.h>
namespace three {
	struct InstancedGroup {
		int start;
		int count;
		int instances;

		InstancedGroup(int start, int count, int instances) : start(start), count(count), instances(instances) {}
	};
	class InstancedBufferGeometry : public BufferGeometry {
	public:
		using ptr = std::shared_ptr<InstancedBufferGeometry>;

		std::vector<InstancedGroup> groups;

		int maxInstancedCount=-1;

		InstancedBufferGeometry() : BufferGeometry(),maxInstancedCount(-1) {

		}
		InstancedBufferGeometry(const InstancedBufferGeometry& source) : BufferGeometry(source) {
			if (source.groups.size() > 0) {
				groups.reserve(source.groups.size());
				std::copy(source.groups.begin(), source.groups.end(), groups.begin());
			}
			maxInstancedCount = source.maxInstancedCount;
		}
		void addGroup(int start, int count, int instances) {
			groups.push_back(InstancedGroup(start, count, instances));
		}

		InstancedBufferGeometry* clone() {
			return new InstancedBufferGeometry(*this);
		}

		InstancedBufferGeometry& copy(const InstancedBufferGeometry& source) {

			BufferGeometry::copy(source);

			if (source.groups.size() > 0) {
				groups.reserve(source.groups.size());
				std::copy(source.groups.begin(), source.groups.end(), groups.begin());
			}
			
			maxInstancedCount = source.maxInstancedCount;

			return *this;
		}
	};
}
#endif