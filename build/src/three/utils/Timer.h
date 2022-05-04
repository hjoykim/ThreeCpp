#ifndef TIMER_H
#define TIMER_H
#include <ctime>
#include <chrono>

namespace three {
	class timer
	{
	public:

		using clock_t = std::chrono::high_resolution_clock;
		using duration_t = clock_t::duration;

	private:

		clock_t::time_point start_time;

	public:

		timer() :
			start_time(clock_t::now()) { }

		duration_t get_elapsed_time() const { return (clock_t::now() - start_time); }

		void reset() { start_time = clock_t::now(); }
	};
}
#endif