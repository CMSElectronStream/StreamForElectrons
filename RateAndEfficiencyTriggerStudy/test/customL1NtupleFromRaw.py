import FWCore.ParameterSet.Config as cms

from L1TriggerDPG.L1Menu.customL1Ntuple_cfg import *

process.p.remove(process.l1RecoTreeProducer)
process.p.remove(process.l1MuonRecoTreeProducer)

# edit here

OUTFILE="L1Tree_data.root"
NEVTS=1000

process.TFileService.fileName=OUTFILE
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(NEVTS) )

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100

## readFiles.extend( ['file:///data2/battilan/L1Trigger/62X_RAW_RECO.root'] )
#readFiles.extend( ['/store/data/Run2012D/ZeroBias25ns1/RAW/v1/000/209/151/F44A29B9-AD47-E211-901E-5404A63886D4.root'] )
#readFiles.extend( ['root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00005/02B79593-F47F-E311-8FF6-003048FFD796.root'] )
readFiles.extend( ['file:/afs/cern.ch/work/b/beaucero/public/L1NeutrinoMC4L1Studies/0A53FC3E-0780-E311-9D84-0025905A60AA.root'])

process.UCT2015Producer.puMultCorrect = cms.bool(True)
process.UCT2015Producer.puETMax = cms.uint32(7)
process.UCT2015Producer.jetSeed = cms.uint32(2)

processDumpFile = open('config.dump', 'w')
print >> processDumpFile, process.dumpPython()
