
// MY IMPORTS
#include "FlatNTuple.hpp"
#include "TrackBuilder.hpp"

// CPP IMPORTS
#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <math.h>
#include <sstream>
#include <string>
#include <vector>

namespace emtf {

LinearFitter::LinearFitter() {
	n = 0;
	min_x_ptr = nullptr;
	max_x_ptr = nullptr;
}

void LinearFitter::add(const double& x, const double& y) {
	// ADD
	x_vector.push_back(x);
	y_vector.push_back(y);
	n++;

	// GET MIN X
	if (min_x_ptr == nullptr) {
		min_x_ptr = &x;
	} else if (*min_x_ptr > x) {
		min_x_ptr = &x;
	}

	// GET MAX X
	if (max_x_ptr == nullptr) {
		max_x_ptr = &x;
	} else if (*max_x_ptr < x) {
		max_x_ptr = &x;
	}
}

void LinearFitter::calculate() {
	TGraph graph(n, x_vector.data(), y_vector.data());
	TF1 function("func", "[0] + [1] * x", *min_x_ptr, *max_x_ptr);

	// FIT
	fit_ptr = graph.Fit(&function, "NQRS");
}

const float LinearFitter::predict(const double& x) const {
	return (float) (fit_ptr->Value(0) + fit_ptr->Value(1) * x);
}

const TFitResultPtr LinearFitter::getFit() const {
	return fit_ptr;
}

Track::Track(const FlatNTuple& _ntuple, const int& _endcap) :
ntuple(_ntuple),
endcap(_endcap) {

}

void Track::addHit(const unsigned int& i_hit) {
	i_hits.push_back(i_hit);

	const unsigned int i_seg = ntuple.I("hit_match_iSeg", i_hit);

	const float seg_globX = ntuple.F("seg_globX", i_seg);
	const float seg_globY = ntuple.F("seg_globY", i_seg);
	const float seg_globZ = ntuple.F("seg_globZ", i_seg);

	zx_fitter.add(seg_globZ, seg_globX);
	zy_fitter.add(seg_globZ, seg_globY);
}

void Track::analyze() {
	zx_fitter.calculate();
	zy_fitter.calculate();
}

const int Track::getEndcap() const {
	return endcap;
}

const std::vector<int> Track::getHitBX() const {
	std::vector<int> bx_vector;

	for (auto & i_hit : i_hits) {
		bx_vector.push_back(ntuple.I("hit_BX", i_hit));
	}

	return bx_vector;
}

const std::vector<std::array<float, 3 >> Track::getHitCoords() const {
	std::vector<std::array<float, 3 >> coords;

	for (auto & i_hit : i_hits) {
		const unsigned int i_seg = ntuple.I("hit_match_iSeg", i_hit);

		const float seg_globX = ntuple.F("seg_globX", i_seg);
		const float seg_globY = ntuple.F("seg_globY", i_seg);
		const float seg_globZ = ntuple.F("seg_globZ", i_seg);

		coords.push_back({seg_globX, seg_globY, seg_globZ});
	}

	return coords;
}

const std::array<float, 3> Track::getCoordAtZeq0() const {
	return {(float) zx_fitter.getFit()->Value(0), (float) zy_fitter.getFit()->Value(0), 0.};
}

const std::array<float, 3> Track::getDirection() const {
	const float dx_dz = (float) zx_fitter.getFit()->Value(1);
	const float dy_dz = (float) zy_fitter.getFit()->Value(1);
	return {dx_dz, dy_dz, 1};
}

const LinearFitter Track::getZXFitter() const {
	return zx_fitter;
}

const LinearFitter Track::getZYFitter() const {
	return zy_fitter;
}

float Track::getChiSquare() const {
	return (float) ((zx_fitter.getFit()->Chi2() + zy_fitter.getFit()->Chi2()) / 2.);
}

TrackBuilder::TrackBuilder(const FlatNTuple& _ntuple) :
ntuple(_ntuple) {

}

void TrackBuilder::addHit(const unsigned int& i_hit) {
	std::string station = getStationForHit(i_hit);

	std::map<std::string, std::vector<unsigned int>>::iterator it = station_hits.find(station);

	if (it == station_hits.end()) {
		std::vector<unsigned int> buffer;

		buffer.push_back(i_hit);

		station_hits.insert(std::pair<std::string, std::vector<unsigned int>>(station, buffer));
	} else {
		std::vector<unsigned int> buffer(it->second);
		buffer.push_back(i_hit);
	}
}

std::string TrackBuilder::getStationForHit(const unsigned int& i_hit) const {
	const int endcap = ntuple.I("hit_endcap", i_hit);
	const int station = ntuple.I("hit_station", i_hit);

	std::stringstream os;
	os << "ME" << (endcap > 0 ? 'p' : 'm') << station;
	return os.str();
}

const std::vector<unsigned int>* TrackBuilder::getStationHits(std::string station) const {
	std::map<std::string, std::vector<unsigned int>>::const_iterator it = station_hits.find(station);

	if (it == station_hits.end()) {
		return nullptr;
	} else {
		return &it->second;
	}
}

void TrackBuilder::forEachTrack(const std::function<void(const Track&) >& consumer) const {
	const std::vector<unsigned int>* mep1_hits = getStationHits("MEp1");
	const std::vector<unsigned int>* mep2_hits = getStationHits("MEp2");
	const std::vector<unsigned int>* mep3_hits = getStationHits("MEp3");
	const std::vector<unsigned int>* mep4_hits = getStationHits("MEp4");

	if (mep1_hits != nullptr && mep2_hits != nullptr && mep3_hits != nullptr && mep4_hits != nullptr) {
		for (auto & i_hit1 : *mep1_hits) {
			for (auto & i_hit2 : *mep2_hits) {
				for (auto & i_hit3 : *mep3_hits) {
					for (auto & i_hit4 : *mep4_hits) {
						Track track(ntuple, 1);
						track.addHit(i_hit1);
						track.addHit(i_hit2);
						track.addHit(i_hit3);
						track.addHit(i_hit4);
						track.analyze();

						if (isnan(track.getZXFitter().getFit()->Chi2())) return;
						if (!isfinite(track.getZXFitter().getFit()->Chi2())) return;
						if (isnan(track.getZYFitter().getFit()->Chi2())) return;
						if (!isfinite(track.getZYFitter().getFit()->Chi2())) return;

						consumer(track);
					}
				}
			}
		}
	}

	const std::vector<unsigned int>* mem1_hits = getStationHits("MEm1");
	const std::vector<unsigned int>* mem2_hits = getStationHits("MEm2");
	const std::vector<unsigned int>* mem3_hits = getStationHits("MEm3");
	const std::vector<unsigned int>* mem4_hits = getStationHits("MEm4");

	if (mem1_hits != nullptr && mem2_hits != nullptr && mem3_hits != nullptr && mem4_hits != nullptr) {
		for (auto & i_hit1 : *mem1_hits) {
			for (auto & i_hit2 : *mem2_hits) {
				for (auto & i_hit3 : *mem3_hits) {
					for (auto & i_hit4 : *mem4_hits) {
						Track track(ntuple, -1);
						track.addHit(i_hit1);
						track.addHit(i_hit2);
						track.addHit(i_hit3);
						track.addHit(i_hit4);
						track.analyze();

						if (isnan(track.getZXFitter().getFit()->Chi2())) return;
						if (!isfinite(track.getZXFitter().getFit()->Chi2())) return;
						if (isnan(track.getZYFitter().getFit()->Chi2())) return;
						if (!isfinite(track.getZYFitter().getFit()->Chi2())) return;

						consumer(track);
					}
				}
			}
		}
	}
}
} // namespace emtf
