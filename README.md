StreamForElectrons
==================

Tutorial:

    https://indico.cern.ch/conferenceDisplay.py?confId=268771

Test sample:

    eos cp /eos/cms/store/group/comm_trigger/TriggerStudiesGroup/Skims/Top/SingleMuDS/pickevents_SemiMuon10k_1_1_Bbo.root ./


To install:

    cmsrel CMSSW_6_2_3
    cd CMSSW_6_2_3/src/
    cmsenv
    git cms-addpkg HLTrigger/Configuration
    git clone git@github.com:CMSElectronStream/StreamForElectrons.git


test:

process.streams = cms.PSet( 
  ALCAECALELE = cms.vstring( 'AlCaECALEle' ),
)

process.datasets = cms.PSet( 
  EcalLaser = cms.vstring( 'HLT_EcalElectron' )
)


... bla bla bla


process.source = cms.Source( "PoolSource",
    fileNames = cms.untracked.vstring(
        'file:pickevents_SemiMuon10k_1_1_Bbo.root',
    ),
    secondaryFileNames = cms.untracked.vstring(
    ),
    inputCommands = cms.untracked.vstring(
        'keep *'
    )
)