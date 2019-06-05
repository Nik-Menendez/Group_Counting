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
	const std::string data_store = "data/FlatNtuple_Run_2018D_v2_2018_10_25_ZeroBias_PU50_postSep26/181025_144026/0000/";
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
	TH1F* lct_count = new TH1F("lct_count", "Total LCT Count", 50, 0, 50);
	TH1F* eta_group = new TH1F("eta_group", "Eta values of hits", 50, -2.5, 2.5);
	TH1F* phi_group = new TH1F("phi_group", "Phi values of hits", 100, -200, 200);
	TH1F* station_group = new TH1F("station_group", "Station hit", 5, 0, 5);
	TH1F* chamber_group = new TH1F("chamber_group", "Chamber hit", 36, 0, 36);
	TH1F* group_size = new TH1F("group_size", "Number of Hits in a Group", 6, 0, 6);
	TH1F* delt_phi = new TH1F("delt_phi", "Difference in Phi Values", 20, 0, 4);
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
		int trk_BX = 0;
		int isCSC = 0;
		int trk_qua = 0;
		unsigned int vec_row = -1;
		bool flag = false;
		int lct_cnt = 0;
		
                for (unsigned int i_track = 0; i_track < ntuple.I("nHits"); i_track++) {
			trk_BX = ntuple.I("hit_BX", i_track);
			isCSC = ntuple.I("hit_isCSC", i_track);
			trk_qua = ntuple.I("hit_quality", i_track);
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
                	trk_location[i_track].push_back(trk_BX);
			trk_location[i_track].push_back(isCSC);
			trk_location[i_track].push_back(trk_qua);
		}

		sort(trk_location.begin(), trk_location.end(), sortcol);

		for (unsigned int i=0;i<trk_location.size();i++) {
			if (trk_location[i][4] == 1 || !(trk_location[i][5] == 0 && trk_location[i][6] == 1 && trk_location[i][7] > -100)) {continue;}

			eta->Fill(trk_location[i][0]);
			phi->Fill(trk_location[i][1]);
			station->Fill(trk_location[i][2]);
			chamber->Fill(trk_location[i][3]);
			lct_cnt++;

			for (unsigned int j=i;j<trk_location.size();j++) {
				if (abs(trk_location[i][0]-trk_location[j][0]) < .1 && abs(trk_location[i][1]-trk_location[j][1]) < 4 /*&& trk_location[i][2] > 1 */&& trk_location[i][2] == trk_location[j][2] /*&& trk_location[i][3] == trk_location[j][3]*/){
					flag = false;
					delt_phi->Fill(trk_location[i][1] - trk_location[j][1]);
					for (unsigned int test=0;test<grouping.size();test++) {
						if (grouping[test][0] == trk_location[j][0] && abs(grouping[test][1] - trk_location[j][1]) < 1 && grouping[test][3] != trk_location[j][3] && (abs(fmod(grouping[test][1],10)) < 1 || abs(fmod(grouping[test][1],10) > 9))) {
							cout << "We got an overlap yo at phi " << trk_location[j][1] << " and phi " << grouping[test][1] << endl;
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
			if (grouping.size() >= 3) {
				group_cnt++;
				i=i+grouping.size();
				for (unsigned int k=0;k<grouping.size();k++) {
					eta_group->Fill(grouping[k][0]);
					phi_group->Fill(grouping[k][1]);
					station_group->Fill(grouping[k][2]);
					chamber_group->Fill(grouping[k][3]);
				}
				group_size->Fill(grouping.size());
			}
			grouping.clear();
			vec_row = -1;
		}

		group_count->Fill(group_cnt);
		lct_count->Fill(lct_cnt);
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
	lct_count->Write();

	eta_group->Write();
	phi_group->Write();
	station_group->Write();
	chamber_group->Write();
	group_count->Write();

	delt_phi->Write();
	
	myPlot.Close();
	// END OUTPUT

	return 0;
}
