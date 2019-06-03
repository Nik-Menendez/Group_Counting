// Root Imports
#include <TCanvas.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2Poly.h>
#include <TPad.h>

// MY IMPORTS
#include "FlatNTupleEngine.hpp"
#include "FlatNTuple.hpp"
#include "TrackBuilder.hpp"
#include "Plotter.hpp"

// CPP IMPORTS
#include <algorithm>
#include <filesystem>
#include <functional>
#include <iostream>
#include <math.h>
#include <vector>

// FOWARD DECLARATION
bool keepAllTracks(const emtf::Track & track, const float & pt_max) {
	return true;
}

bool keepChiCutsTracks(const emtf::Track & track, const float & pt_max) {
	if (track.getChiSquare() > 5) return false; // ONLY chi sqr < 2

	return true;
}

bool keepHaloTracks(const emtf::Track & track, const float & pt_max) {
	const float chi_sqr = track.getChiSquare();

	const std::array<float, 3> coord_at_z_eq_0 = track.getCoordAtZeq0();
	const std::array<float, 3> direction = track.getDirection();

	const float a = coord_at_z_eq_0[0];
	const float c = coord_at_z_eq_0[1];

	const float b = direction[0];
	const float d = direction[1];

	const float dr_dz = sqrt(pow(b, 2) + pow(d, 2));

	const float z_for_min_r = -1 * (a * b + c * d) / (pow(b, 2) + pow(d, 2));

	if (chi_sqr > 5) return false; // ONLY chi sqr < 2
	if (dr_dz > 0.1) return false; // ONLY dr/dz <= 0.1
	if (fabs(z_for_min_r) < 250) return false; // ONLY z_for_min_r >= 250

	return true;
}

bool keepHighPTTracks(const emtf::Track & track, const float & pt_max) {
	if (track.getChiSquare() > 5) return false; // ONLY chi sqr < 2
	if (pt_max < 22) return false; // HIGH PT

	return true;
}

// MAIN
int main() {
	// RUN VARS
	// const std::string data_store = "/home/omiguelc/Work/CERN/CMS/L1T/EMTF/development/emtf-beam-halo/data/FlatNtuple_Run_2018D_v2_2018_10_25_ZeroBias_PU50_postSep26/181025_144026/0000/";
	const std::string data_store = "/home/omiguelc/Work/CERN/CMS/L1T/EMTF/development/emtf-beam-halo/data/FlatNtuple_Run_2018D_v2_2018_10_25_SingleMuon_PU50_postSep26/181025_143812/0000/";
	// END RUN VARS

	emtf::FlatNTupleEngine engine;

	// LOAD FILES
	for (unsigned int j = 1; j <= 25; j++) {
		engine.addRootFile(data_store + "tuple_" + std::to_string(j) + ".root");
	}
	// END LOAD FILES

	// DECLARE PLOTS
	emtf::Plotter all_tracks_plotter("all", keepAllTracks);
	emtf::Plotter chi_2_tracks_plotter("chi2", keepChiCutsTracks);
	emtf::Plotter halo_tracks_plotter("halo", keepHaloTracks);
	emtf::Plotter high_pt_tracks_plotter("high_pt", keepHighPTTracks);
	// END DECLARE PLOTS

	// EVENTS
	engine.forEachEvent([&](unsigned int i_event, const emtf::FlatNTuple & ntuple) -> void
	{
		emtf::TrackBuilder track_builder(ntuple);

		for (unsigned int i_hit = 0; i_hit < ntuple.I("nHits"); i_hit++) {
			const int hit_bx = ntuple.I("hit_BX", i_hi<=t);
			const int station = ntuple.I("hit_station", i_hit);

			/**
			 * Skips any hit that doesn't match any of the following conditions:
			 *
			 * 1. Station == 0 && BX == 0
			 * 2. Station != 0 && BX <= 0
			 *
			 */
			if (!((station == 0 && hit_bx == 0) || (station != 0 && hit_bx <= 0))) {
				continue;
			}

			if (ntuple.I("hit_isCSC", i_hit) == 1 && ntuple.I("hit_match_iSeg", i_hit) >= 0) {
				track_builder.addHit(i_hit);
			}
		}

		float emtf_pt_max = -1;
		for (unsigned int i_track = 0; i_track < ntuple.I("nTracks"); i_track++) {
			if (ntuple.I("trk_qual", i_track) >= 12 && ntuple.I("trk_BX", i_track) == 0 && ntuple.F("trk_pt", i_track) >= emtf_pt_max) {
				emtf_pt_max = ntuple.F("trk_pt", i_track);
			}
		}

		track_builder.forEachTrack([&](const emtf::Track & track) -> void
		{
			all_tracks_plotter.process(track, emtf_pt_max);
			chi_2_tracks_plotter.process(track, emtf_pt_max);
			halo_tracks_plotter.process(track, emtf_pt_max);
			high_pt_tracks_plotter.process(track, emtf_pt_max);

			// std::cout << "****************************************" << std::endl;

			// const std::vector<std::array<float, 3>> coords = track.getHitCoords();

			// std::for_each(coords.begin(), coords.end(), [&](const std::array<float, 3> coord) -> void {
			//     std::cout << "X: " << coord[0] << " Y: " << coord[1] << " Z: " << coord[2] << std::endl;
			// });

			// std::cout << "a: " << a << " b: " << b << " c: " << c << " d: " << d << std::endl;
			// std::cout << "X_sqr=" << chi_sqr  << " r0=" << r0 << " ||dr/dz||=" << dr_dz << " z_for_min_r=" << z_for_min_r << " min_r=" << min_r << std::endl;
			// std::cout << "****************************************" << std::endl;
		});
	});

	// END EVENTS

	// PLOTS

	// END PLOTS

	// OUTPUT
	TFile myPlot("plots.root", "RECREATE");

	all_tracks_plotter.write();
	chi_2_tracks_plotter.write();
	halo_tracks_plotter.write();
	high_pt_tracks_plotter.write();

	TCanvas z_for_min_r_1d_canvas("z_for_min_r_1d");
	z_for_min_r_1d_canvas.cd(0);

	chi_2_tracks_plotter.z_for_min_r_1d.SetLineColor(kBlack);
	high_pt_tracks_plotter.z_for_min_r_1d.SetLineColor(kBlue);
	halo_tracks_plotter.z_for_min_r_1d.SetLineColor(kRed);

	chi_2_tracks_plotter.z_for_min_r_1d.Draw();
	high_pt_tracks_plotter.z_for_min_r_1d.Draw("SAME");
	halo_tracks_plotter.z_for_min_r_1d.Draw("SAME");

	z_for_min_r_1d_canvas.Write();

	myPlot.Close();
	// END OUTPUT

	return 0;
}
