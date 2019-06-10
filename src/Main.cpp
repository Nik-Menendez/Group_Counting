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

// SORTER BY ETA
bool sortcol1 ( const vector<float>& v1, const vector<float>& v2 ) {
	return v1[0] < v2[0];
}

// SORTER BY SECTOR
bool sortcol2 ( const vector<float>& v1, const vector<float>& v2 ) {
        return v1[9] < v2[9];
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
	TH1F* zone0CSC = new TH1F("zone0CSC", "nHits in Zone 0 CSCs", 50, 0, 50);
	TH1F* zone0tot = new TH1F("zone0tot", "nHits in Zone 0", 50, 0, 50);
	TH1F* zone1CSC = new TH1F("zone1CSC", "nHits in Zone 1 CSCs", 50, 0, 50);
        TH1F* zone1tot = new TH1F("zone1tot", "nHits in Zone 1", 50, 0, 50);
	TH1F* zone2CSC = new TH1F("zone2CSC", "nHits in Zone 2 CSCs", 50, 0, 50);
        TH1F* zone2tot = new TH1F("zone2tot", "nHits in Zone 2", 50, 0, 50);
	TH1F* zone3CSC = new TH1F("zone3CSC", "nHits in Zone 3 CSCs", 50, 0, 50);
        TH1F* zone3tot = new TH1F("zone3tot", "nHits in Zone 3", 50, 0, 50);
	TH1F* zone0sec = new TH1F("zone0sec", "Sectors of Hits in Zone 0", 12, 0, 12);
	TH1F* zone1sec = new TH1F("zone1sec", "Sectors of Hits in Zone 1", 12, 0, 12);
	TH1F* zone2sec = new TH1F("zone2sec", "Sectors of Hits in Zone 2", 12, 0, 12);
	TH1F* zone3sec = new TH1F("zone3sec", "Sectors of Hits in Zone 3", 12, 0, 12);

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
	TH1F* delt_phi = new TH1F("delt_phi", "Difference in Phi Values", 200, 0, 4);
	// END DECLARE HISTOGRAMS

	unsigned int group_cnt = 0;

	// EVENTS
	engine.forEachEvent([&](unsigned int i_event, const emtf::FlatNTuple & ntuple) -> void
	{
		emtf::TrackBuilder track_builder(ntuple);

                vector< vector<float> > trk_location; //Vector to store information on the hit
		vector< vector<float> > grouping; //Vector to compare hits in a grouping to each other
		vector<int> zone_counts; //Vector to store how many hits in a zone
		for (int i=0;i<8;i++) {zone_counts.push_back(0);}
       		unsigned int vec_row = -1;
		bool flag = false;
		int lct_cnt = 0;
		int last_j = 0;
		int zone_add = 1;
		
		// STORE HIT INFORMATION IN VECTOR
                for (unsigned int i_track = 0; i_track < ntuple.I("nHits"); i_track++) {
			trk_location.push_back(vector<float>());
			trk_location[i_track].push_back(ntuple.F("hit_eta",i_track));
			trk_location[i_track].push_back(ntuple.F("hit_phi",i_track));
			trk_location[i_track].push_back(ntuple.I("hit_station",i_track));
			trk_location[i_track].push_back(ntuple.I("hit_chamber",i_track));
			trk_location[i_track].push_back(ntuple.I("hit_neighbor",i_track));
                	trk_location[i_track].push_back(ntuple.I("hit_BX",i_track));
			trk_location[i_track].push_back(ntuple.I("hit_isCSC",i_track));
			trk_location[i_track].push_back(ntuple.I("hit_quality",i_track));
			trk_location[i_track].push_back(ntuple.F("hit_theta",i_track));
			trk_location[i_track].push_back(ntuple.I("hit_sector_index",i_track));
		}

		////////////////////////////////////////////// LCT ZONE COUNTS LOGIC SECTION /////////////////////////////////////////////////

		// SORT HITS BY SECTOR FOR EASY GROUPING
		sort(trk_location.begin(), trk_location.end(), sortcol2);

		// LOOP OVER HITS
		for (unsigned int i=0;i<trk_location.size();i++) {
                        if (trk_location[i][4] == 1 || !(trk_location[i][5] == 0 /*&& trk_location[i][6] == 1 && trk_location[i][7] > -100*/)) {continue;} //checks they aren't neighbor and are BX0			
			// Compares the hits to each other to check if they're in the same sector and then record what zone they're in
			for (unsigned int j=i;j<trk_location.size();j++) {

				if (trk_location[i][9] != trk_location[j][9]) {
					break;
				} else if (trk_location[i][9] == trk_location[j][9]) {
					if (trk_location[i][8] >= 8.5 && trk_location[i][8] <= 20.8) {
                                		if (trk_location[i][6] == 0) {
                                        		zone_counts[0]+=zone_add;
                                		}
                                		zone_counts[1]+=zone_add;
						zone0sec->Fill(trk_location[i][9]);
                        		} else if (trk_location[i][8] >= 19.9 && trk_location[i][8] <= 23.0) {
                                		if (trk_location[i][6] == 0) {
                                        		zone_counts[2]+=zone_add;
                                		}
                                		zone_counts[3]+=zone_add;
						zone1sec->Fill(trk_location[i][9]);
                        		} else if (trk_location[i][8] >= 22.2 && trk_location[i][8] <= 33.9) {
                                		if (trk_location[i][6] == 0) {
                                        		zone_counts[4]+=zone_add;
                                		}
                                		zone_counts[5]+=zone_add;
						zone2sec->Fill(trk_location[i][9]);
                        		} else if (trk_location[i][8] >= 33.0 && trk_location[i][8] <= 44.7) {
                                		if (trk_location[i][6] == 0) {
                                       		 zone_counts[6]+=zone_add;
                                		}
                         		        zone_counts[7]+=zone_add;
						zone3sec->Fill(trk_location[i][9]);
                        		}
				}
				last_j = j;
			}

			i=last_j;

			
		} 

		////////////////////////////////////////////////// GENERAL GROUPINGS SECTION ///////////////////////////////////////////

		// SORT BY ETA TO SEE FOR EASY GENERAL GROUPINGS
		sort(trk_location.begin(), trk_location.end(), sortcol1);

		for (unsigned int i=0;i<trk_location.size();i++) {
			if (trk_location[i][4] == 1 || !(trk_location[i][5] == 0 /*&& trk_location[i][6] == 1 && trk_location[i][7] > -100*/)) {continue;}

			if (!(trk_location[i][6] == 1)) {continue;}

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

		// Fill zone count histograms. If it has 1 or 0 hits, just fills 0.
		if (zone_counts[0] >= 2) {zone0CSC->Fill(zone_counts[0]);
		} else {zone0CSC->Fill(0);}
		if (zone_counts[1] >= 2) {zone0tot->Fill(zone_counts[1]);
                } else {zone0tot->Fill(0);}
		if (zone_counts[2] >= 2) {zone1CSC->Fill(zone_counts[2]);
                } else {zone1CSC->Fill(0);}
		if (zone_counts[3] >= 2) {zone1tot->Fill(zone_counts[3]);
                } else {zone1tot->Fill(0);}
		if (zone_counts[4] >= 2) {zone2CSC->Fill(zone_counts[4]);
                } else {zone2CSC->Fill(0);}
		if (zone_counts[5] >= 2) {zone2tot->Fill(zone_counts[5]);
                } else {zone2tot->Fill(0);}
		if (zone_counts[6] >= 2) {zone3CSC->Fill(zone_counts[6]);
                } else {zone3CSC->Fill(0);}
		if (zone_counts[7] >= 2) {zone3tot->Fill(zone_counts[7]);
                } else {zone3tot->Fill(0);}
		zone_count.clear();

		group_count->Fill(group_cnt);
		lct_count->Fill(lct_cnt);
		group_cnt = 0;
		trk_location.clear();


	});

	// END EVENTS

	// OUTPUT
	TFile myPlot("plots/eta_cpp.root", "RECREATE");

	TDirectory* Total_Counts = myPlot.mkdir("Total Counts");
	TDirectory* Zone_Counts = myPlot.mkdir("Zone Counts");
	TDirectory* Cluster_Counts = myPlot.mkdir("Cluster Counts");

	Zone_Counts->cd();

	zone0CSC->Write();
	zone0tot->Write();
	zone1CSC->Write();
        zone1tot->Write();
	zone2CSC->Write();
        zone2tot->Write();
	zone3CSC->Write();
        zone3tot->Write();

	zone0sec->Write();
	zone1sec->Write();
	zone2sec->Write();
	zone3sec->Write();

	Total_Counts->cd();

	eta->Write();
	phi->Write();
	station->Write();
	chamber->Write();
	lct_count->Write();

	Cluster_Counts->cd();

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
