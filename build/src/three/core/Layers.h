#ifndef LAYERS_THREE_H
#define LAYERS_THREE_H
namespace three {
	class Layers {
	public:
		unsigned mask = 1;
		Layers() {}
		virtual ~Layers() = default;

		void set(unsigned channel) {
			mask = 1 << channel;
		}
		void enable(unsigned channel) {
			mask |= 1 << channel;
		}
		void toggle(unsigned int channel) {
			mask ^= 1 << channel;
		}

		void disable(unsigned int channel) {
			mask &= ~(1 << channel);
		}

		bool test(const Layers& layers) const {
			return (mask & layers.mask) != 0;
		}
		Layers& copy(const Layers& source) {
			mask = source.mask;
			return *this;
		}
	};
}
#endif