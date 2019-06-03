#! /usr/bin/env python

# import sys
# import math
from ROOT import *
# import numpy as np
# from array import *
# # from eff_modules import *

MAX_FILE =     7
MAX_EVT  = 100
PRT_EVT  =  1000

def main():

###################
## Initialize files
###################

  # file_name = 'root://eoscms.cern.ch//store/user/abrinke1/EMTF/Emulator/ntuples/SingleMuon/FlatNtuple_Run_2018B_v1_2018_07_04_SingleMuon_2018_emul/NTuple_0.root'
  # in_file   = TFile.Open(file_name)
  # in_tree   = in_file.Get('FlatNtupleData/tree')
  
  file_names = []
  #store  = 'root://eoscms.cern.ch//store/user/abrinke1/EMTF/Emulator/ntuples/'
  #in_dir = 'SingleMuon/FlatNtuple_Run_2018B_v1_2018_07_04_SingleMuon_2018_emul/'
  
  store = 'data/'
  in_dir = 'FlatNtuple_Run_2018D_v2_2018_10_25_ZeroBias_PU50_postSep26_OOT_LCT/181025_182945/0000/'
  for i in range(10):
      if (i >= MAX_FILE): break
      j = i+1
      file_names.append(store+in_dir+'tuple_%d.root' % j)
      print 'Opening file: '+store+in_dir+file_names[i]
      
  in_chains = []
  for i in range(len(file_names)):
      in_chains.append( TChain('FlatNtupleData/tree') )
      in_chains[i].Add( file_names[i] )
      
#  out_file = TFile('plots/Read_FlatNtuples.root', 'recreate')
  out_file = TFile('plots/Cluster_Counting.root', 'recreate')
    
#############
## Histograms
#############

  eta_bins = [50, -2.5, 2.5]

  h_trk_eta = TH1D('h_trk_eta', 'EMTF emulated track #eta', eta_bins[0], eta_bins[1], eta_bins[2])

  cluster_count = TH1D('cluster_count', 'Number of Clusters Per Event', 10, 0, 10)
                    
#############
## Event loop
#############
  
#  iEvt = -1
#  for ch in in_chains:
#      
#      if iEvt > MAX_EVT: break
#              
#      for jEvt in range(ch.GetEntries()):
#          iEvt += 1
#          
#          if iEvt > MAX_EVT: break
#          if iEvt % PRT_EVT is 0: print 'Event #', iEvt
#
#          ch.GetEntry(jEvt)
#
#          nUnpHits = int(ch.nHits)
#          nEmuHits = int(ch.nSimHits)
#
#          nUnpTrks = int(ch.nUnpTracks)
#          nEmuTrks = int(ch.nTracks)
#
#          print 'Event has %d unpacked (%d emulated) hits, %d unpacked (%d emulated) tracks' % (nUnpHits, nEmuHits, nUnpTrks, nEmuTrks)
#
#          for iTrk in range(nEmuTrks):
#              #print '  - Emulated track #%d has eta %.2f' % (iTrk+1, ch.trk_eta[iTrk])
#              print 'eta = %.2f' % ch.trk_eta[iTrk] 
#              h_trk_eta.Fill(ch.trk_eta[iTrk])
                
        ## End loop over events in chain (jEvt)
    ## End loop over chains (ch)

######################
## Event Loop Clusters
######################

  iEvt = -1
  for ch in in_chains:

    if iEvt > MAX_EVT: break

    for jEvt in range(ch.GetEntries()):
      iEvt += 1
      
      if iEvt > MAX_EVT: break
      if iEvt % PRT_EVT is 0: print 'Event #', iEvt

      ch.GetEntry(jEvt)

      nTrks = int(ch.nTracks)

      eta = []
      trk = [[] for i in range(2)]
      for iTrk in range(nTrks):
        eta.append(ch.trk_eta[iTrk])
        trk[0].append(float(ch.trk_eta[iTrk]))
        trk[1].append(ch.trk_phi[iTrk])
        #print 'trk = %.2f %.2f' % (trk[0][iTrk], trk[1][iTrk])
        #print 'eta = %d' % ch.trk_eta[iTrk] 

      eta.sort()
      trk_sort = sorted(trk,key=lambda l:l[0])
      print(trk)
      #for i in range(len(eta)):
        #print 'trk = %.2f %.2f, trk_sorted = %.2f %.2f' % (trk[0][i], trk[1][i], trk_sort[0][i], trk_sort[1][i])
        #print 'eta = %.2f' % eta[i]

      print 'Next event:'

      #print 'We have %d etas sorted' % len(eta)
      
      group_cnt = 0
      for i in range(len(eta)):
        if i > len(eta)-5: break
       
        for j in range(i,len(eta)):
          if abs(eta[i] - eta[j]) > .2: break
          num_in_cluster = j
  
        if num_in_cluster >= 3: 
          group_cnt += 1
          i = i+num_in_cluster
         
      cluster_count.Fill(group_cnt);

      

######################
## Save the histograms
######################

  out_file.cd()

#  h_trk_eta.Write()
  cluster_count.Write()  
  cluster_count.Draw()

  del out_file


if __name__ == '__main__':
    main()
