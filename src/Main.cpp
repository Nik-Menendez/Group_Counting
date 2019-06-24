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
	TH1F* zone0CSC = new TH1F("zone0CSC", "nHits in a Single Sector in Zone 0 CSCs;nHits;Frequency", 25, 0, 25);
	TH1F* zone0RPC = new TH1F("zone0RPC", "nHits in a Single Sector in Zone 0 RPCs;nHits;Frequency", 25, 0, 25);
	TH1F* zone0tot = new TH1F("zone0tot", "nHits in a Single Sector in Zone 0;nHits;Frequency", 25, 0, 25);
	TH1F* zone1CSC = new TH1F("zone1CSC", "nHits in a Single Sector in Zone 1 CSCs;nHits;Frequency", 25, 0, 25);
	TH1F* zone1RPC = new TH1F("zone1RPC", "nHits in a Single Sector in Zone 1 RPCs;nHits;Frequency", 25, 0, 25);
        TH1F* zone1tot = new TH1F("zone1tot", "nHits in a Single Sector in Zone 1;nHits;Frequency", 25, 0, 25);
	TH1F* zone2CSC = new TH1F("zone2CSC", "nHits in a Single Sector in Zone 2 CSCs;nHits;Frequency", 25, 0, 25);
        TH1F* zone2RPC = new TH1F("zone2RPC", "nHits in a Single Sector in Zone 2 RPCs;nHits;Frequency", 25, 0, 25);
	TH1F* zone2tot = new TH1F("zone2tot", "nHits in a Single Sector in Zone 2;nHits;Frequency", 25, 0, 25);
	TH1F* zone3CSC = new TH1F("zone3CSC", "nHits in a Single Sector in Zone 3 CSCs;nHits;Frequency", 25, 0, 25);
        TH1F* zone3RPC = new TH1F("zone3RPC", "nHits in a Single Sector in Zone 3 RPCs;nHits;Frequency", 25, 0, 25);
	TH1F* zone3tot = new TH1F("zone3tot", "nHits in a Single Sector in Zone 3;nHits;Frequency", 25, 0, 25);
	TH1F* zone0sec = new TH1F("zone0sec", "Sectors of Hits in Zone 0", 12, 0, 12);
	TH1F* zone1sec = new TH1F("zone1sec", "Sectors of Hits in Zone 1", 12, 0, 12);
	TH1F* zone2sec = new TH1F("zone2sec", "Sectors of Hits in Zone 2", 12, 0, 12);
	TH1F* zone3sec = new TH1F("zone3sec", "Sectors of Hits in Zone 3", 12, 0, 12);

//	TH1F* eta = new TH1F("eta", "Eta values of hits", 50, -2.5, 2.5);
//	TH1F* phi = new TH1F("phi", "Phi values of hits", 100, -200, 200);
//	TH1F* station = new TH1F("station", "Station hit", 5, 0, 5);
//	TH1F* chamber = new TH1F("chamber", "Chamber hit", 36, 0, 36);
//	TH1F* group_count = new TH1F("group_count", "Group Count", 10, -.5, 9.5);
//	TH1F* lct_count = new TH1F("lct_count", "Total LCT Count", 50, 0, 50);
//	TH1F* eta_group = new TH1F("eta_group", "Eta values of hits", 50, -2.5, 2.5);
//	TH1F* phi_group = new TH1F("phi_group", "Phi values of hits", 100, -200, 200);
//	TH1F* station_group = new TH1F("station_group", "Station hit", 5, 0, 5);
//	TH1F* chamber_group = new TH1F("chamber_group", "Chamber hit", 36, 0, 36);
//	TH1F* group_size = new TH1F("group_size", "Number of Hits in a Group", 6, 0, 6);
//	TH1F* delt_phi = new TH1F("delt_phi", "Difference in Phi Values", 200, 0, 4);
	TH2I* delt_pos_with_OL = new TH2I("delt_pos_with_OL", "Difference in Phi and Theta Values with Overlap", 20, -10, 10, 10, -5, 5);
	TH2I* delt_pos_wout_OL = new TH2I("delt_pos_wout_OL", "Difference in Phi and Theta Values without Overlap", 20, -10, 10, 10, -5, 5);
	TH1I* delt_phi_with_OL = new TH1I("delt_phi_with_OL", "Difference in Phi Values with Overlap", 20, -10, 10);
	TH1I* delt_phi_wout_OL = new TH1I("delt_phi_wout_OL", "Difference in Phi Values without Overlap", 20, -10, 10);
	TH1I* delt_the_with_OL = new TH1I("delt_the_with_OL", "Difference in Theta Values with Overlap", 20, -10, 10);
	TH1I* delt_the_wout_OL = new TH1I("delt_the_wout_OL", "Difference in Theta Values without Overlap", 20, -10, 10);
	// END DECLARE HISTOGRAMS

	unsigned int group_cnt = 0;

	// EVENTS
	engine.forEachEvent([&](unsigned int i_event, const emtf::FlatNTuple & ntuple) -> void
	{
		emtf::TrackBuilder track_builder(ntuple);

                vector< vector<float> > trk_location; //Vector to store information on the hit
		vector< vector<float> > grouping; //Vector to compare hits in a grouping to each other
		vector<int> zone_counts; //Vector to store how many hits in a zone
		for (int i=0;i<12;i++) {zone_counts.push_back(0);}
       		unsigned int vec_row = -1;
		bool flag = false;
		int lct_cnt = 0;
		unsigned int last_j = 0;
		int zone_add = 1;
		int BX=0;
		
		// STORE HIT INFORMATION IN VECTOR
                for (unsigned int i_track = 0; i_track < ntuple.I("nHits"); i_track++) {
			trk_location.push_back(vector<float>());
			trk_location[i_track].push_back(ntuple.F("hit_eta",i_track));
			trk_location[i_track].push_back(ntuple.I("hit_phi_int",i_track));
			trk_location[i_track].push_back(ntuple.I("hit_station",i_track));
			trk_location[i_track].push_back(ntuple.I("hit_chamber",i_track));
			trk_location[i_track].push_back(ntuple.I("hit_neighbor",i_track));
                	trk_location[i_track].push_back(ntuple.I("hit_BX",i_track));
			trk_location[i_track].push_back(ntuple.I("hit_isCSC",i_track));
			trk_location[i_track].push_back(ntuple.I("hit_quality",i_track));
			trk_location[i_track].push_back(ntuple.I("hit_theta_int",i_track));
			trk_location[i_track].push_back(ntuple.I("hit_sector_index",i_track));
		}
		
		for (unsigned int i_track=0;i_track<trk_location.size();i_track++) {
			if (trk_location[i_track][5]!=0) {
				trk_location.erase(trk_location.begin() + i_track);
				i_track=i_track-1;
			}
		}

		////////////////////////////////////////////// LCT ZONE COUNTS LOGIC SECTION /////////////////////////////////////////////////

		// SORT HITS BY SECTOR FOR EASY GROUPING
		sort(trk_location.begin(), trk_location.end(), sortcol2);

		// LOOP OVER HITS
		for (unsigned int i=0;i<trk_location.size();i++) {
                        //if (/*trk_location[i][4] == 1 ||*/ !(trk_location[i][5] == 0 /*&& trk_location[i][6] == 1 && trk_location[i][7] > -100*/)) {continue;} //checks they aren't neighbor and are BX0			
			// Compares the hits to each other to check if they're in the same sector and then record what zone they're in
			for (unsigned int j=(i+1);j<trk_location.size();j++) {

				//if (!(trk_location[i][5] == 0)) {continue;}
				//if (trk_location[i][1] - trk_location[j][1] == 0 && trk_location[i][8] - trk_location[j][8] == 0) {continue;}

				for (unsigned int k=0;k<j;k++) {
					if (abs(trk_location[j][1]-trk_location[k][1])<=2 && abs(trk_location[j][8]-trk_location[k][8])<=0 && (trk_location[j][3]==trk_location[k][3]+1 || trk_location[j][3]==trk_location[k][3]-1)) {
						flag=true;
						//cout << "That's an overlap in phi of " << abs(trk_location[j][1]-trk_location[k][1]) << ", theta of " << abs(trk_location[j][8]-trk_location[k][8]) << ", between chambers " << trk_location[j][3] << " and " << trk_location[k][3] << endl;
					}
					if (trk_location[j][3]==trk_location[k][3]+1 || trk_location[j][3]==trk_location[k][3]-1) {
						delt_pos_with_OL->Fill(trk_location[k][1] - trk_location[j][1], trk_location[k][8] - trk_location[j][8]);
						delt_phi_with_OL->Fill(trk_location[k][1] - trk_location[j][1]);
						delt_the_with_OL->Fill(trk_location[k][8] - trk_location[j][8]);
					}
				}

				if (flag){
					flag=false;
					continue;
				}

				for (unsigned int k=0;k<j;k++) {
					if (trk_location[j][3]==trk_location[k][3]+1 || trk_location[j][3]==trk_location[k][3]-1) {
						delt_pos_wout_OL->Fill(trk_location[k][1] - trk_location[j][1], trk_location[k][8] - trk_location[j][8]);
						delt_phi_wout_OL->Fill(trk_location[k][1] - trk_location[j][1]);
						delt_the_wout_OL->Fill(trk_location[k][8] - trk_location[j][8]);
					}
				}

				if (trk_location[j-1][9] != trk_location[j][9]) {
					zone0CSC->Fill(zone_counts[0]);
                			zone0RPC->Fill(zone_counts[1]);
               				zone0tot->Fill(zone_counts[2]);
               				zone1CSC->Fill(zone_counts[3]);
               				zone1RPC->Fill(zone_counts[4]);
               				zone1tot->Fill(zone_counts[5]);
               				zone2CSC->Fill(zone_counts[6]);
               				zone2RPC->Fill(zone_counts[7]);
               				zone2tot->Fill(zone_counts[8]);
               				zone3CSC->Fill(zone_counts[9]);
               				zone3RPC->Fill(zone_counts[10]);
               				zone3tot->Fill(zone_counts[11]);

					for (unsigned int pi=0;pi<zone_counts.size();pi++) {
			                        if (zone_counts[pi] >= 15) {
			                                cout << "//////////////////////////////////////////" << endl;
			                                cout << "Zone " << pi/3 << " has " << zone_counts[pi] << " hits." << endl;
			                                for (unsigned int pj=i;pj<(i+zone_counts[pi]);pj++) {
			                                        //if (trk_location[pj][5]!=0) {continue;}
			
			                                        //if (trk_location[pj][10]==(pi/3)) {
			                                                cout << "Hit has phi " << trk_location[pj][1] << ", theta " << trk_location[pj][8] << ", chamber " << trk_location[pj][3] << ", sector " << trk_location[pj][9] << " in zone " << trk_location[pj][10] << endl;
			                                        //}
			                                }
			                                flag=true;
			                        }
			                }
			
			                if (flag) {
			                        cout << "//////////////////////////////////////////" << endl;
			                        flag=false;
			                }
				

               				for (unsigned int clear=0;clear<zone_counts.size();clear++) {zone_counts[clear]=0;}
					break;
				} else {
					if (trk_location[j][8] >= 0 && trk_location[j][8] <= 43  && trk_location[i][8] >= 0 && trk_location[i][8] <= 43) {
                                		if (trk_location[j][6] == 0) {
                                        		zone_counts[0]+=zone_add;
                                		} else {
							zone_counts[1]+=zone_add;
						}
                                		zone_counts[2]+=zone_add;
						zone0sec->Fill(trk_location[j][9]);
						trk_location[i].push_back(0);
						trk_location[j].push_back(0);
                        		} else if (trk_location[j][8] >= 40 && trk_location[j][8] <= 51 && trk_location[i][8] >= 40 && trk_location[i][8] <= 51) {
                                		if (trk_location[j][6] == 0) {
                                        		zone_counts[3]+=zone_add;
                                		} else {
							zone_counts[4]+=zone_add;
						}
                                		zone_counts[5]+=zone_add;
						zone1sec->Fill(trk_location[j][9]);
						trk_location[i].push_back(1);
                                                trk_location[j].push_back(1);
                        		} else if (trk_location[j][8] >= 48 && trk_location[j][8] <= 89 && trk_location[i][8] >= 48 && trk_location[i][8] <= 89) {
                                		if (trk_location[j][6] == 0) {
                                        		zone_counts[6]+=zone_add;
                                		} else {
							zone_counts[7]+=zone_add;
						}
                                		zone_counts[8]+=zone_add;
						zone2sec->Fill(trk_location[j][9]);
						trk_location[i].push_back(2);
                                                trk_location[j].push_back(2);
                        		} else if (trk_location[j][8] >= 86 && trk_location[j][8] <= 127 && trk_location[i][8] >= 86 && trk_location[i][8] <= 127) {
                                		if (trk_location[j][6] == 0) {
                                       			zone_counts[9]+=zone_add;
                                		} else {
							zone_counts[10]+=zone_add;
						}
                         		        zone_counts[11]+=zone_add;
						zone3sec->Fill(trk_location[j][9]);
						trk_location[i].push_back(3);
                                                trk_location[j].push_back(3);
                        		}
				}
				last_j = j;
				//cout << "delt_pos = " << trk_location[i][1]-trk_location[j][1] << " phi and " << trk_location[i][8]-trk_location[j][8] << " theta." << endl;
			}

			if (i<last_j) {i=last_j;}

		}

//		for (unsigned int i=0;i<zone_counts.size();i++) {
//			if (zone_counts[i] >= 12) {
//				cout << "//////////////////////////////////////////" << endl;
//				cout << "Zone " << i/3 << " has " << zone_counts[i] << " hits." << endl;
//				for (unsigned int j=0;j<trk_location.size();j++) {
//					if (trk_location[j][5]!=0) {continue;}
//
//					if (trk_location[j][10]==(i/3)) {
//						cout << "Hit has phi " << trk_location[j][1] << ", theta " << trk_location[j][8] << ", chamber " << trk_location[j][3] << ", sector " << trk_location[j][9] << " in zone " << trk_location[j][10] << endl;
//					}
//				}
//				flag=true;
//			}
//		}
//
//		if (flag) {
//			cout << "//////////////////////////////////////////" << endl;
//			flag=false;
//		}

		////////////////////////////////////////////////// GENERAL GROUPINGS SECTION ///////////////////////////////////////////

		// SORT BY ETA TO SEE FOR EASY GENERAL GROUPINGS
		sort(trk_location.begin(), trk_location.end(), sortcol1);

//		for (unsigned int i=0;i<trk_location.size();i++) {
//			if (trk_location[i][4] == 1 || !(trk_location[i][5] == 0 /*&& trk_location[i][6] == 1 && trk_location[i][7] > -100*/)) {continue;}
//
//			if (!(trk_location[i][6] == 1)) {continue;}
//
//			eta->Fill(trk_location[i][0]);
//			phi->Fill(trk_location[i][1]);
//			station->Fill(trk_location[i][2]);
//			chamber->Fill(trk_location[i][3]);
//			lct_cnt++;
//
//			for (unsigned int j=i;j<trk_location.size();j++) {
//
//				if (abs(trk_location[i][0]-trk_location[j][0]) < .1 && abs(trk_location[i][1]-trk_location[j][1]) < 4 /*&& trk_location[i][2] > 1 */&& trk_location[i][2] == trk_location[j][2] /*&& trk_location[i][3] == trk_location[j][3]*/){
//					flag = false;
//					delt_phi->Fill(trk_location[i][1] - trk_location[j][1]);
//					for (unsigned int test=0;test<grouping.size();test++) {
//						if (grouping[test][0] == trk_location[j][0] && abs(grouping[test][1] - trk_location[j][1]) < 1 && grouping[test][3] != trk_location[j][3] && (abs(fmod(grouping[test][1],10)) < 1 || abs(fmod(grouping[test][1],10) > 9))) {
//							cout << "We got an overlap yo at phi " << trk_location[j][1] << " and phi " << grouping[test][1] << endl;
//							flag = true;
//							break;
//						}
//					} if (flag) {continue;}
//					grouping.push_back(vector<float>());
//					vec_row++;
//					for (unsigned int k=0;k<4;k++) {
//						grouping[vec_row].push_back(trk_location[j][k]);
//					}
//				}
//			}
//			if (grouping.size() >= 3) {
//				group_cnt++;
//				i=i+grouping.size();
//				for (unsigned int k=0;k<grouping.size();k++) {
//					eta_group->Fill(grouping[k][0]);
//					phi_group->Fill(grouping[k][1]);
//					station_group->Fill(grouping[k][2]);
//					chamber_group->Fill(grouping[k][3]);
//				}
//				group_size->Fill(grouping.size());
//			}
//			grouping.clear();
//			vec_row = -1;
//		}

//		for (unsigned int i=0;i<trk_location.size();i++) {
//			if (!(trk_location[i][5] == 0)) {continue;}
//
//			for (unsigned int j=i+1;j<trk_location.size();j++) {
//				if (!(trk_location[j][5] == 0)) {continue;}
//
//				delt_pos->Fill(trk_location[i][1] - trk_location[j][1], trk_location[i][0] - trk_location[j][0]);
//			}
//	
//		}

		// Fill zone count histograms. If it has 1 or 0 hits, just fills 0.
//		zone0CSC->Fill(zone_counts[0]);
//		zone0RPC->Fill(zone_counts[1]);
//		zone0tot->Fill(zone_counts[2]);
//		zone1CSC->Fill(zone_counts[3]);
//		zone1RPC->Fill(zone_counts[4]);
//		zone1tot->Fill(zone_counts[5]);
//		zone2CSC->Fill(zone_counts[6]);
//		zone2RPC->Fill(zone_counts[7]);
//		zone2tot->Fill(zone_counts[8]);
//		zone3CSC->Fill(zone_counts[9]);
//		zone3RPC->Fill(zone_counts[10]);
//		zone3tot->Fill(zone_counts[11]);
//		zone_counts.clear();

//		group_count->Fill(group_cnt);
//		lct_count->Fill(lct_cnt);
//		group_cnt = 0;
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
	zone0RPC->Write();
	zone0tot->Write();
	zone1CSC->Write();
	zone1RPC->Write();
        zone1tot->Write();
	zone2CSC->Write();
	zone2RPC->Write();
        zone2tot->Write();
	zone3CSC->Write();
	zone3RPC->Write();
        zone3tot->Write();

	zone0sec->Write();
	zone1sec->Write();
	zone2sec->Write();
	zone3sec->Write();

//	Total_Counts->cd();
//
//	eta->Write();
//	phi->Write();
//	station->Write();
//	chamber->Write();
//	lct_count->Write();
//
	Cluster_Counts->cd();
//
//	eta_group->Write();
//	phi_group->Write();
//	station_group->Write();
//	chamber_group->Write();
//	group_count->Write();
//
//	delt_phi->Write();
	delt_pos_with_OL->Write();
	delt_pos_wout_OL->Write();	
	delt_phi_with_OL->Write();
	delt_phi_wout_OL->Write();
	delt_the_with_OL->Write();
	delt_the_wout_OL->Write();

	myPlot.Close();
	// END OUTPUT
	
	return 0;
}
