#ifndef PLOTTER_HPP
#define PLOTTER_HPP

// MY IMPORTS
#include "TrackBuilder.hpp"

// CPP IMPORTS
#include <functional>


namespace emtf {

	class Plotter {
	private:
		const std::string suffix;

		const std::function<bool (const emtf::Track & track, const float & pt_max) > filter;
	public:
		TH1F chi_sqr_1d;
		TH1F dr_dz_1d;
		TH1F z_for_min_r_1d;
		TH1F min_r_1d;

		TH1F r0_1d;
		TH1F max_pt_1d;
		TH1F bx_me4_1d;
		TH2Poly r0_2d;

		Plotter(const std::string& sufix, const std::function<bool (const emtf::Track & track, const float & pt_max) > filter);
		~Plotter();

		void process(const emtf::Track& track, const float & pt_max);

		void write();
	};
}

#endif /* PLOTTER_HPP */

