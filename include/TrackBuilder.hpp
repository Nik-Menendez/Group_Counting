#ifndef TRACKBUILDER_HPP
#define TRACKBUILDER_HPP

// MY IMPORTS
#include "FlatNTuple.hpp"

// CPP IMPORTS
#include <array>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

// ROOT IMPORTS
#include <TF1.h>
#include <TGraph.h>
#include <TFitResult.h>

namespace emtf {

	class LinearFitter {
	public:
		LinearFitter();

		void add(const double& x, const double& y);
		void calculate();

		const float predict(const double& x) const;
		const TFitResultPtr getFit() const;
	private:
		unsigned int n;
		std::vector<double> x_vector;
		std::vector<double> y_vector;

		const double* min_x_ptr;
		const double* max_x_ptr;

		TFitResultPtr fit_ptr;
	};

	class Track {
	public:
		explicit Track(const FlatNTuple& ntuple, const int& endcap);

		void addHit(const unsigned int& i_hit);

		void analyze();

		const int getEndcap() const;

		const std::vector<int> getHitBX() const;

		const std::vector<std::array<float, 3 >> getHitCoords() const;

		const std::array<float, 3> getCoordAtZeq0() const;

		const std::array<float, 3> getDirection() const;

		const LinearFitter getZXFitter() const;

		const LinearFitter getZYFitter() const;

		float getChiSquare() const;

	private:
		const FlatNTuple& ntuple;
		const int endcap;

		std::vector<unsigned int> i_hits;

		LinearFitter zx_fitter;
		LinearFitter zy_fitter;

	};

	class TrackBuilder {
	public:
		explicit TrackBuilder(const FlatNTuple& ntuple);

		void addHit(const unsigned int& i_hit);

		void forEachTrack(const std::function<void(const Track&) >& consumer) const;

	private:
		const FlatNTuple& ntuple;

		std::map<std::string, std::vector<unsigned int>> station_hits;

		std::string getStationForHit(const unsigned int& i_hit) const;

		const std::vector<unsigned int>* getStationHits(std::string station) const;
	};
} // namespace emtf


#endif
