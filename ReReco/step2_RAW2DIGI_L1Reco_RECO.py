# Auto generated configuration file
# using: 
# Revision: 1.381.2.28 
# Source: /local/reps/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: step2 -s RAW2DIGI,L1Reco,RECO --data --datatier RECO --eventcontent RECO --conditions GR10_P_V11::All --scenario pp --no_exec --magField AutoFromDBCurrent --process reRECO
import FWCore.ParameterSet.Config as cms

process = cms.Process('reRECO')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring(
       'MYINPUTFILE'
   )
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.381.2.28 $'),
    annotation = cms.untracked.string('step2 nevts:1'),
    name = cms.untracked.string('PyReleaseValidation')
)

# Output definition

process.RECOoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RECOEventContent.outputCommands,
    fileName = cms.untracked.string('MYOUTPUTFILE'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('RECO')
    )
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag.globaltag = 'GR_R_62_V1::All'

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOoutput_step = cms.EndPath(process.RECOoutput)

### keep some more info from trigger level
process.RAWRECOEventContent.outputCommands +=  cms.untracked.vstring('keep *_*hltKT6PFJets*_*_*',
                                                                     'keep *_*hltGsfEleAnyL1SeededElectronTrackIso*_*_*',
                                                                     'keep *_*hltL1SeededGsfTrackVars*_*_*',
                                                                     'keep *_*hltL1SeededPhotonEcalIso*_*_*',
                                                                     'keep *_*hltL1SeededPhotonHcalForHE*_*_*',
                                                                     'keep *_*hltL1SeededPhotonHcalIso*_*_*',
                                                                     'keep *_*hltPFMETProducer_*_*',
                                                                     'keep *_*hltFastPVPixelVertices_*_*')

process.RECOEventContent.outputCommands += cms.untracked.vstring('keep *_*hltKT6PFJets*_*_*',
                                                                 'keep *_*hltGsfEleAnyL1SeededElectronTrackIso*_*_*',
                                                                 'keep *_*hltL1SeededGsfTrackVars*_*_*',
                                                                 'keep *_*hltL1SeededPhotonEcalIso*_*_*',
                                                                 'keep *_*hltL1SeededPhotonHcalForHE*_*_*',
                                                                 'keep *_*hltL1SeededPhotonHcalIso*_*_*',
                                                                 'keep *_*hltPFMETProducer*_*_*',
                                                                 'keep *_*hltFastPVPixelVertices*_*_*')

process.RECOoutput.outputCommands += cms.untracked.vstring('keep *_**hltKT6PFJets*_*_*',
                                                           'keep *_*hltGsfEleAnyL1SeededElectronTrackIso*_*_*',
                                                           'keep *_*hltL1SeededGsfTrackVars*_*_*',
                                                           'keep *_*hltL1SeededPhotonEcalIso*_*_*',
                                                           'keep *_*hltL1SeededPhotonHcalForHE*_*_*',
                                                           'keep *_*hltL1SeededPhotonHcalIso*_*_*',
                                                           'keep *_*hltPFMETProducer*_*_*',
                                                           'keep *_*hltFastPVPixelVertices*_*_*')


process.RECOSIMEventContent.outputCommands += cms.untracked.vstring('keep *_*hltKT6PFJets*_*_*',
                                                                    'keep *_*hltGsfEleAnyL1SeededElectronTrackIso*_*_*',
                                                                    'keep *_*hltL1SeededGsfTrackVars_*_*',
                                                                    'keep *_*hltL1SeededPhotonEcalIso*_*_*',
                                                                    'keep *_*hltL1SeededPhotonHcalForHE*_*_*',
                                                                    'keep *_*hltL1SeededPhotonHcalIso*_*_*',
                                                                    'keep *_*hltPFMETProducer*_*_*',
                                                                    'keep *_*hltFastPVPixelVertices*_*_*')


process.AODEventContent.outputCommands += cms.untracked.vstring('keep *_*hltKT6PFJets*_*_*',
                                                                'keep *_*hltGsfEleAnyL1SeededElectronTrackIso*_*_*',
                                                                'keep *_*hltL1SeededGsfTrackVars*_*_*',
                                                                'keep *_*hltL1SeededPhotonEcalIso*_*_*',
                                                                'keep *_*hltL1SeededPhotonHcalForHE*_*_*',
                                                                'keep *_*hltL1SeededPhotonHcalIso*_*_*',
                                                                'keep *_*hltPFMETProducer*_*_*',
                                                                'keep *_*hltFastPVPixelVertices*_*_*')


process.AODSIMEventContent.outputCommands += cms.untracked.vstring('keep *_*hltKT6PFJets*_*_*',
                                                                   'keep *_*hltGsfEleAnyL1SeededElectronTrackIso_*_*',
                                                                   'keep *_*hltL1SeededGsfTrackVars*_*_*',
                                                                   'keep *_*hltL1SeededPhotonEcalIso*_*_*',
                                                                   'keep *_*hltL1SeededPhotonHcalForHE*_*_*',
                                                                   'keep *_*hltL1SeededPhotonHcalIso*_*_*',
                                                                   'keep *_*hltPFMETProducer*_*_*',
                                                                   'keep *_*hltFastPVPixelVertices*_*_*')


# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.endjob_step,process.RECOoutput_step)

