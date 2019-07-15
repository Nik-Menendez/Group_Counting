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
        if (v1[9] != v2[9]) {
		return v1[9] < v2[9];
	}

	return v1[10] < v2[10];
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

	TH1F* zone0Station1 = new TH1F("zone0Station1", "nHits in a Single Sector in Station 1 in Zone 0 CSCs;nHits;Frequency", 15, 0, 15);
	TH1F* zone0Station2 = new TH1F("zone0Station2", "nHits in a Single Sector in Station 2 in Zone 0 CSCs;nHits;Frequency", 15, 0, 15);
	TH1F* zone0Station3 = new TH1F("zone0Station3", "nHits in a Single Sector in Station 3 in Zone 0 CSCs;nHits;Frequency", 15, 0, 15);
        TH1F* zone0Station4 = new TH1F("zone0Station4", "nHits in a Single Sector in Station 4 in Zone 0 CSCs;nHits;Frequency", 15, 0, 15);
	TH1F* zone1Station1 = new TH1F("zone1Station1", "nHits in a Single Sector in Station 1 in Zone 1 CSCs;nHits;Frequency", 15, 0, 15);
        TH1F* zone1Station2 = new TH1F("zone1Station2", "nHits in a Single Sector in Station 2 in Zone 1 CSCs;nHits;Frequency", 15, 0, 15);
        TH1F* zone1Station3 = new TH1F("zone1Station3", "nHits in a Single Sector in Station 3 in Zone 1 CSCs;nHits;Frequency", 15, 0, 15);
        TH1F* zone1Station4 = new TH1F("zone1Station4", "nHits in a Single Sector in Station 4 in Zone 1 CSCs;nHits;Frequency", 15, 0, 15);
	TH1F* zone2Station1 = new TH1F("zone2Station1", "nHits in a Single Sector in Station 1 in Zone 2 CSCs;nHits;Frequency", 15, 0, 15);
        TH1F* zone2Station2 = new TH1F("zone2Station2", "nHits in a Single Sector in Station 2 in Zone 2 CSCs;nHits;Frequency", 15, 0, 15);
        TH1F* zone2Station3 = new TH1F("zone2Station3", "nHits in a Single Sector in Station 3 in Zone 2 CSCs;nHits;Frequency", 15, 0, 15);
        TH1F* zone2Station4 = new TH1F("zone2Station4", "nHits in a Single Sector in Station 4 in Zone 2 CSCs;nHits;Frequency", 15, 0, 15);
        TH1F* zone3Station1 = new TH1F("zone3Station1", "nHits in a Single Sector in Station 1 in Zone 3 CSCs;nHits;Frequency", 15, 0, 15);
        TH1F* zone3Station2 = new TH1F("zone3Station2", "nHits in a Single Sector in Station 2 in Zone 3 CSCs;nHits;Frequency", 15, 0, 15);
        TH1F* zone3Station3 = new TH1F("zone3Station3", "nHits in a Single Sector in Station 3 in Zone 3 CSCs;nHits;Frequency", 15, 0, 15);
        TH1F* zone3Station4 = new TH1F("zone3Station4", "nHits in a Single Sector in Station 4 in Zone 3 CSCs;nHits;Frequency", 15, 0, 15);

	TH1F* zone0Minus1 = new TH1F("zone0Minus1", "nHits in a Single Sector in Zone 0 CSCs without Station 1;nHits;Frequency", 25, 0, 25);
	TH1F* zone1Minus1 = new TH1F("zone1Minus1", "nHits in a Single Sector in Zone 1 CSCs without Station 1;nHits;Frequency", 25, 0, 25);
	TH1F* zone2Minus1 = new TH1F("zone2Minus1", "nHits in a Single Sector in Zone 2 CSCs without Station 1;nHits;Frequency", 25, 0, 25);
        TH1F* zone3Minus1 = new TH1F("zone3Minus1", "nHits in a Single Sector in Zone 3 CSCs without Station 1;nHits;Frequency", 25, 0, 25);

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
	TH2I* delt_pos_with_OL = new TH2I("delt_pos_with_OL", "Difference in Phi and Theta Values with Overlap", 120, -120, 120, 10, -5, 5);
	TH2I* delt_pos_wout_OL = new TH2I("delt_pos_wout_OL", "Difference in Phi and Theta Values without Overlap", 120, -120, 120, 10, -5, 5);
	TH1I* delt_phi_with_OL = new TH1I("delt_phi_with_OL", "Difference in Phi Values with Overlap", 20, -10, 10);
	TH1I* delt_phi_wout_OL = new TH1I("delt_phi_wout_OL", "Difference in Phi Values without Overlap", 20, -10, 10);
	TH1I* delt_the_with_OL = new TH1I("delt_the_with_OL", "Difference in Theta Values with Overlap", 20, -10, 10);
	TH1I* delt_the_wout_OL = new TH1I("delt_the_wout_OL", "Difference in Theta Values without Overlap", 20, -10, 10);
	// END DECLARE HISTOGRAMS

	// EVENTS
	engine.forEachEvent([&](unsigned int i_event, const emtf::FlatNTuple & ntuple) -> void
	{
		emtf::TrackBuilder track_builder(ntuple);

                vector< vector<float> > trk_location; //Vector to store information on the hit
		vector< vector<float> > grouping; //Vector to compare hits in a grouping to each other
		vector<int> zone_counts; //Vector to store how many hits in a zone
		for (int i=0;i<12;i++) {zone_counts.push_back(0);}
		vector<int> station_counts; //Vector to store hits by station
		for (int i=0;i<16;i++) {station_counts.push_back(0);}
		vector<int> zone_counts_minus1;
		for (int i=0;i<4;i++) {zone_counts_minus1.push_back(0);}
		int cham_overlap=0;
		bool flag = false;
		unsigned int last_j = 0;
		int zone_add = 1;
		int station_int = 0;
	
		// STORE HIT INFORMATION IN VECTOR
                for (unsigned int i_track = 0; i_track < ntuple.I("nHits"); i_track++) {
			trk_location.push_back(vector<float>());
			trk_location[i_track].push_back(ntuple.F("hit_eta",i_track));  		// 0
			trk_location[i_track].push_back(ntuple.I("hit_phi_int",i_track));	// 1
			trk_location[i_track].push_back(ntuple.I("hit_station",i_track));	// 2
			trk_location[i_track].push_back(ntuple.I("hit_chamber",i_track));	// 3
			trk_location[i_track].push_back(ntuple.I("hit_neighbor",i_track));	// 4
                	trk_location[i_track].push_back(ntuple.I("hit_BX",i_track));		// 5
			trk_location[i_track].push_back(ntuple.I("hit_isCSC",i_track));		// 6
			trk_location[i_track].push_back(ntuple.I("hit_quality",i_track));	// 7
			trk_location[i_track].push_back(ntuple.I("hit_theta_int",i_track));	// 8
			trk_location[i_track].push_back(ntuple.I("hit_sector_index",i_track));	// 9
			trk_location[i_track].push_back(ntuple.I("hit_endcap",i_track));	// 10
			trk_location[i_track].push_back(ntuple.I("hit_sector",i_track));	// 11
			trk_location[i_track].push_back(ntuple.I("hit_ring",i_track));		// 12
			trk_location[i_track].push_back(ntuple.F("hit_phi", i_track)); 		// 13
			trk_location[i_track].push_back(ntuple.I("hit_quality", i_track));	// 14
		}
		
		for (unsigned int i_track=0;i_track<trk_location.size();i_track++) {
			if (trk_location[i_track][5]!=0) {
				trk_location.erase(trk_location.begin() + i_track);
				i_track=i_track-1;
			}
		}

		for (unsigned int i=1;i<trk_location.size();i++) {
			for (unsigned int j=0;j<i;j++) {
				if (trk_location[j][6]==trk_location[i][6] && trk_location[j][3]==trk_location[i][3] && trk_location[j][2]==trk_location[i][2] && trk_location[j][12]==trk_location[i][12]) {
					cham_overlap++;
				}
			}
			if (cham_overlap>=2) {
				trk_location.erase(trk_location.begin() + i);
				i=i-1;
			}
			cham_overlap=0;
		}

		////////////////////////////////////////////// LCT ZONE COUNTS LOGIC SECTION /////////////////////////////////////////////////

		// SORT HITS BY SECTOR FOR EASY GROUPING
		sort(trk_location.begin(), trk_location.end(), sortcol2);

		// LOOP OVER HITS
		for (unsigned int i=0;i<trk_location.size();i++) {
			// Compares the hits to each other to check if they're in the same sector and then record what zone they're in
			for (unsigned int j=(i+1);j<trk_location.size();j++) {

				//Checks for overlap in the Chambers
				for (unsigned int k=0;k<j;k++) {
					if (abs(trk_location[j][1]-trk_location[k][1])<=30 && abs(trk_location[j][8]-trk_location[k][8])<=1 && ( (int(abs(trk_location[j][3]-trk_location[k][3])))==1 || (int(abs(trk_location[j][3]-trk_location[k][3]))%18)==17 )) {
						flag=true;
					}
					if (trk_location[j][3]==trk_location[k][3]+1 || trk_location[j][3]==trk_location[k][3]-1) {
						delt_pos_with_OL->Fill(trk_location[k][1] - trk_location[j][1], trk_location[k][8] - trk_location[j][8]);
						delt_phi_with_OL->Fill(trk_location[k][1] - trk_location[j][1]);
						delt_the_with_OL->Fill(trk_location[k][8] - trk_location[j][8]);
					}
				}

				//If two hits are the same particle through two chambers, skip it
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

				//Fill histograms when no longer in the same sector
				if (trk_location[j-1][9] != trk_location[j][9] || trk_location[j-1][10] != trk_location[j][10] /*|| trk_location[j-1][2] != trk_location[j][2]*/) {

					for (unsigned int pi=2;pi<zone_counts.size();pi+=3) {
						unsigned int csc_count=0;
						unsigned int rpc_count=0;
						unsigned int minus1_count=0;

						//Separate CSC and RPC hits as well as hits by station
						for (unsigned int pj=i;pj<(i+zone_counts[pi]);pj++) {

							if (trk_location[pj][6]==1) {csc_count++;}
                                                        if (trk_location[pj][6]==0) {rpc_count++;}
									
							station_int = (pi/3) * 4 + (trk_location[pj][2]-1);
							station_counts[station_int]++;	

							if (trk_location[pj][6]==1 && trk_location[pj][2]!=1) {
								minus1_count++;
							}
						}

						zone_counts[pi-2] = csc_count;
						zone_counts[pi-1] = rpc_count;
						zone_counts_minus1[pi/3] = minus1_count;

//						//Read out large events
//			                        if (zone_counts[pi-2] >= 7){
//						if  (pi/3==2) {
//			                                cout << "//////////////////////////////////////////" << endl;
//			                                cout << "Zone " << pi/3 << " has " << zone_counts[pi] << " hits, " << zone_counts[pi-2] << " CSC hits, and " << zone_counts[pi-1] << " RPC hits." << endl;
//			                                flag=true;
//
//							for (unsigned int pj=i;pj<(i+zone_counts[pi]);pj++) {
//								if (trk_location[pj][6] == 1) {
//								cout << "Hit has isCSC " << trk_location[pj][6] << ", endcap " << trk_location[pj][10] << ", sector " << trk_location[pj][11] << ", sector index " << trk_location[pj][9] << ", station " << trk_location[pj][2] << ", ring " << trk_location[pj][12] << ", chamber " << trk_location[pj][3] << ", phi_int " << trk_location[pj][1] << ", theta_int " << trk_location[pj][8] << ", phi " << trk_location[pj][13] << ", eta " << trk_location[pj][0] << ", quality " << trk_location[pj][14] << ", zone " << trk_location[pj][15] << endl;
//								}
//							}
//
//			                	}
//						}
					}
//			
//			                if (flag) {
//			                        cout << "//////////////////////////////////////////" << endl;
//			                        flag=false;
//			                }
					
					//Fill Histograms
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

					zone0Station1->Fill(station_counts[0]);
					zone0Station2->Fill(station_counts[1]);
					zone0Station3->Fill(station_counts[2]);
					zone0Station4->Fill(station_counts[3]);
					zone1Station1->Fill(station_counts[4]);
                                        zone1Station2->Fill(station_counts[5]);
                                        zone1Station3->Fill(station_counts[6]);
                                        zone1Station4->Fill(station_counts[7]);
					zone2Station1->Fill(station_counts[8]);
                                        zone2Station2->Fill(station_counts[9]);
                                        zone2Station3->Fill(station_counts[10]);
                                        zone2Station4->Fill(station_counts[11]);
                                        zone3Station1->Fill(station_counts[12]);
                                        zone3Station2->Fill(station_counts[13]);
                                        zone3Station3->Fill(station_counts[14]);
                                        zone3Station4->Fill(station_counts[15]);

					zone0Minus1->Fill(zone_counts_minus1[0]);
					zone1Minus1->Fill(zone_counts_minus1[1]);
					zone2Minus1->Fill(zone_counts_minus1[2]);
					zone3Minus1->Fill(zone_counts_minus1[3]);

               				for (unsigned int clear=0;clear<zone_counts.size();clear++) {zone_counts[clear]=0;}
					for (unsigned int clear=0;clear<station_counts.size();clear++) {station_counts[clear]=0;}
					for (unsigned int clear=0;clear<zone_counts_minus1.size();clear++) {station_counts[clear]=0;}
					break;
				} else {
					if (trk_location[j][8] >= 0 && trk_location[j][8] <= 43  && trk_location[i][8] >= 0 && trk_location[i][8] <= 43) {
                                		if (trk_location[j][6] == 1) {
                                        		zone_counts[0]+=zone_add;
                                		} else {
							zone_counts[1]+=zone_add;
						}
                                		zone_counts[2]+=zone_add;
						trk_location[i].push_back(0);
						trk_location[j].push_back(0);
                        		} else if (trk_location[j][8] >= 40 && trk_location[j][8] <= 51 && trk_location[i][8] >= 40 && trk_location[i][8] <= 51) {
                                		if (trk_location[j][6] == 1) {
                                        		zone_counts[3]+=zone_add;
                                		} else {
							zone_counts[4]+=zone_add;
						}
                                		zone_counts[5]+=zone_add;
						trk_location[i].push_back(1);
                                                trk_location[j].push_back(1);
                        		} else if (trk_location[j][8] >= 48 && trk_location[j][8] <= 89 && trk_location[i][8] >= 48 && trk_location[i][8] <= 89) {
                                		if (trk_location[j][6] == 1) {
                                        		zone_counts[6]+=zone_add;
                                		} else {
							zone_counts[7]+=zone_add;
						}
                                		zone_counts[8]+=zone_add;
						trk_location[i].push_back(2);
                                                trk_location[j].push_back(2);
                        		} else if (trk_location[j][8] >= 86 && trk_location[j][8] <= 127 && trk_location[i][8] >= 86 && trk_location[i][8] <= 127) {
                                		if (trk_location[j][6] == 1) {
                                       			zone_counts[9]+=zone_add;
                                		} else {
							zone_counts[10]+=zone_add;
						}
                         		        zone_counts[11]+=zone_add;
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
//			if (zone_counts[i] >= 10) {
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
//		sort(trk_location.begin(), trk_location.end(), sortcol1);

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

	TDirectory* Zone_Counts = myPlot.mkdir("Zone Counts");
	TDirectory* Zone_Counts_Station = myPlot.mkdir("Zone Counts by Station");
	TDirectory* Zone_Counts_Minus_Station_1 = myPlot.mkdir("Zone Counts minus Station 1");
	TDirectory* Chamber_Overlap = myPlot.mkdir("Chamber Overlap");

	TCanvas* c = new TCanvas("canvas", "canvas", 1024, 768);
	gPad->SetLogy();

	Zone_Counts->cd();

	zone0CSC->Write();zone0CSC->Draw();c->SaveAs("plots/pdfs/zone0csc.pdf");
	zone0RPC->Write();zone0RPC->Draw();c->SaveAs("plots/pdfs/zone0rpc.pdf");
	zone0tot->Write();zone0tot->Draw();c->SaveAs("plots/pdfs/zone0tot.pdf");
	zone1CSC->Write();zone1CSC->Draw();c->SaveAs("plots/pdfs/zone1csc.pdf");
	zone1RPC->Write();zone1RPC->Draw();c->SaveAs("plots/pdfs/zone1rpc.pdf");
        zone1tot->Write();zone1tot->Draw();c->SaveAs("plots/pdfs/zone1tot.pdf");
	zone2CSC->Write();zone2CSC->Draw();c->SaveAs("plots/pdfs/zone2csc.pdf");
	zone2RPC->Write();zone2RPC->Draw();c->SaveAs("plots/pdfs/zone2rpc.pdf");
        zone2tot->Write();zone2tot->Draw();c->SaveAs("plots/pdfs/zone2tot.pdf");
	zone3CSC->Write();zone3CSC->Draw();c->SaveAs("plots/pdfs/zone3csc.pdf");
	zone3RPC->Write();zone3RPC->Draw();c->SaveAs("plots/pdfs/zone3rpc.pdf");
        zone3tot->Write();zone3tot->Draw();c->SaveAs("plots/pdfs/zone3tot.pdf");

	Zone_Counts_Station->cd();

	zone0Station1->Write();zone0Station1->Draw();c->SaveAs("plots/pdfs/zone0Station1.pdf");
	zone0Station2->Write();zone0Station2->Draw();c->SaveAs("plots/pdfs/zone0Station2.pdf");
	zone0Station3->Write();zone0Station3->Draw();c->SaveAs("plots/pdfs/zone0Station3.pdf");
	zone0Station4->Write();zone0Station4->Draw();c->SaveAs("plots/pdfs/zone0Station4.pdf");
	zone1Station1->Write();zone1Station1->Draw();c->SaveAs("plots/pdfs/zone1Station1.pdf");
        zone1Station2->Write();zone1Station2->Draw();c->SaveAs("plots/pdfs/zone1Station2.pdf");
        zone1Station3->Write();zone1Station3->Draw();c->SaveAs("plots/pdfs/zone1Station3.pdf");
        zone1Station4->Write();zone1Station4->Draw();c->SaveAs("plots/pdfs/zone1Station4.pdf");
	zone2Station1->Write();zone2Station1->Draw();c->SaveAs("plots/pdfs/zone2Station1.pdf");
        zone2Station2->Write();zone2Station2->Draw();c->SaveAs("plots/pdfs/zone2Station2.pdf");
        zone2Station3->Write();zone2Station3->Draw();c->SaveAs("plots/pdfs/zone2Station3.pdf");
        zone2Station4->Write();zone2Station4->Draw();c->SaveAs("plots/pdfs/zone2Station4.pdf");
        zone3Station1->Write();zone3Station1->Draw();c->SaveAs("plots/pdfs/zone3Station1.pdf");
        zone3Station2->Write();zone3Station2->Draw();c->SaveAs("plots/pdfs/zone3Station2.pdf");
        zone3Station3->Write();zone3Station3->Draw();c->SaveAs("plots/pdfs/zone3Station3.pdf");
        zone3Station4->Write();zone3Station4->Draw();c->SaveAs("plots/pdfs/zone3Station4.pdf");

	Zone_Counts_Minus_Station_1->cd();

	zone0Minus1->Write();zone0Minus1->Draw();c->SaveAs("plots/pdfs/zone0Minus1.pdf");
	zone1Minus1->Write();zone1Minus1->Draw();c->SaveAs("plots/pdfs/zone1Minus1.pdf");
	zone2Minus1->Write();zone2Minus1->Draw();c->SaveAs("plots/pdfs/zone2Minus1.pdf");
	zone3Minus1->Write();zone3Minus1->Draw();c->SaveAs("plots/pdfs/zone3Minus1.pdf");

//	Total_Counts->cd();
//
//	eta->Write();
//	phi->Write();
//	station->Write();
//	chamber->Write();
//	lct_count->Write();
//
	Chamber_Overlap->cd();
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
