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
//#include "Plotter.hpp"

// CPP IMPORTS
#include <algorithm>
//#include <filesystem>
#include <functional>
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

// SORTER
bool sortcol ( const vector<float>& v1, const vector<float>& v2 ) {
	return v1[0] < v2[0];
}

// MAIN
int main() {
	// RUN VARS
	const std::string data_store = "data/FlatNtuple_Run_2018D_v2_2018_10_25_ZeroBias_PU50_postSep26_OOT_LCT/181025_182945/0000/";
	// END RUN VARS

	emtf::FlatNTupleEngine engine;

	// LOAD FILES
	for (unsigned int j = 1; j <= 7; j++) {
		engine.addRootFile(data_store + "tuple_" + std::to_string(j) + ".root");
	}
	// END LOAD FILES

	// DECLARE HISTOGRAMS	
	TH1F* eta = new TH1F("eta", "Eta values of hits", 50, -2.5, 2.5);
	TH1F* phi = new TH1F("phi", "Phi values of hits", 100, -200, 200);
	TH1F* station = new TH1F("station", "Station hit", 5, 0, 5);
	TH1F* chamber = new TH1F("chamber", "Chamber hit", 36, 0, 36);
	TH1F* group_count = new TH1F("group_count", "Group Count", 10, -.5, 9.5);
	TH1F* lct_count = new TH1F("lct_count", "Total LCT Count", 10, 0, 10);
	// END DECLARE HISTOGRAMS

	unsigned int group_cnt = 0;

	// EVENTS
	engine.forEachEvent([&](unsigned int i_event, const emtf::FlatNTuple & ntuple) -> void
	{
		emtf::TrackBuilder track_builder(ntuple);

                vector< vector<float> > trk_location;
		vector< vector<float> > grouping;
       		float trk_eta = 0;      
		float trk_phi = 0;        
		int trk_sta = 0;
		int trk_chm = 0;
		int trk_nei = 0;
		unsigned int vec_row = -1;
		bool flag = false;
		
                for (unsigned int i_track = 0; i_track < ntuple.I("nHits"); i_track++) {
                	trk_eta = ntuple.F("hit_eta", i_track);
			trk_phi = ntuple.F("hit_phi", i_track);
			trk_sta = ntuple.I("hit_station", i_track);
			trk_chm = ntuple.I("hit_chamber", i_track);
			trk_nei = ntuple.I("hit_neighbor", i_track);
			trk_location.push_back(vector<float>());
			trk_location[i_track].push_back(trk_eta);
			trk_location[i_track].push_back(trk_phi);
			trk_location[i_track].push_back(trk_sta);
			trk_location[i_track].push_back(trk_chm);
			trk_location[i_track].push_back(trk_nei);
                }

		sort(trk_location.begin(), trk_location.end(), sortcol);

		for (unsigned int i=0;i<trk_location.size();i++) {
			if (trk_location[i][4] == 1) {continue;}
			for (unsigned int j=i;j<trk_location.size();j++) {
				if (abs(trk_location[i][0]-trk_location[j][0]) < .1 && abs(trk_location[i][1]-trk_location[j][1]) < 1 /*&& trk_location[i][2] > 1 */&& trk_location[i][2] == trk_location[j][2] && trk_location[i][3] == trk_location[j][3]){
					flag = false;
					for (unsigned int test=0;test<grouping.size();test++) {
						if (grouping[test][0] == trk_location[j][0] && grouping[test][1] == trk_location[j][1] && grouping[test][2] == trk_location[j][2]) {
							//cout << "We got an overlap yo in chan " << trk_location[j][3] << " and chan " << grouping[test][3] << endl;
							flag = true;
							break;
						}
					} if (flag) {continue;}
					grouping.push_back(vector<float>());
					vec_row++;
					for (unsigned int k=0;k<4;k++) {
						grouping[vec_row].push_back(trk_location[j][k]);
					}
				}
			}
			if (grouping.size() >= 5) {
				group_cnt++;
				i=i+grouping.size();
				for (unsigned int k=0;k<grouping.size();k++) {
					eta->Fill(grouping[k][0]);
					phi->Fill(grouping[k][1]);
					station->Fill(grouping[k][2]);
					chamber->Fill(grouping[k][3]);
				}
			}
			grouping.clear();
			vec_row = -1;
		}

		group_count->Fill(group_cnt);
		group_cnt = 0;
		trk_location.clear();


	});

	// END EVENTS

	// OUTPUT
	TFile myPlot("plots/eta_cpp.root", "RECREATE");

	eta->Write();
	phi->Write();
	station->Write();
	chamber->Write();
	group_count->Write();
	
	myPlot.Close();
	// END OUTPUT

	return 0;
}
