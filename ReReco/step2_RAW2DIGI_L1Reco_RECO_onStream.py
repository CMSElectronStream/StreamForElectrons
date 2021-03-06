# Auto generated configuration file
# using: 
# Revision: 1.381.2.28 
# Source: /local/reps/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: step2 -s RAW2DIGI,L1Reco,RECO --data --datatier RECO --eventcontent RECO --conditions GR10_P_V11::All --scenario pp --no_exec --magField AutoFromDBCurrent --process reRECO
import FWCore.ParameterSet.Config as cms

####### option parsing
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('python')
# add a list of strings for events to process
options.register ('isMC', False, VarParsing.multiplicity.singleton, VarParsing.varType.bool, 'is MC or is Data')
options.parseArguments()
print options



from Configuration.StandardSequences.Eras import eras

#process = cms.Process('reRECO')
process = cms.Process('reRECO',eras.Run2_2016)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')

if options.isMC :
    process.load('SimGeneral.MixingModule.mixNoPU_cfi')
    process.load('Configuration.StandardSequences.RawToDigi_cff')
    process.load('Configuration.StandardSequences.Reconstruction_cff')
else :
    process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
    process.load('Configuration.StandardSequences.Reconstruction_Data_cff')

process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring(options.inputFiles)
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
    fileName = cms.untracked.string(options.outputFile),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('RECO')
    )
)

# Additional output definition

# Other statements
#from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag as customiseGlobalTag
#process.GlobalTag = customiseGlobalTag(process.GlobalTag, globaltag = 'DESIGN71_V5',conditions='TrackerAlignmentExtendedError_2011Realistic_v1_mc,TrackerAlignmentErrorExtendedRcd,frontier://FrontierProd/CMS_CONDITIONS+MuonDTAPEObjectsExtended_v0_mc,DTAlignmentErrorExtendedRcd,frontier://FrontierProd/CMS_CONDITIONS+MuonCSCAPEObjectsExtended_v0_mc,CSCAlignmentErrorExtendedRcd,frontier://FrontierProd/CMS_CONDITIONS+EcalSamplesCorrelation_mc,EcalSamplesCorrelationRcd,frontier://FrontierProd/CMS_CONDITIONS+EcalPulseShapes_mc,EcalPulseShapesRcd,frontier://FrontierProd/CMS_CONDITIONS+EcalPulseCovariances_mc,EcalPulseCovariancesRcd,frontier://FrontierProd/CMS_CONDITIONS')
#process.GlobalTag = customiseGlobalTag(process.GlobalTag, globaltag = 'DESIGN71_V5',conditions='TrackerAlignmentExtendedError_2011Realistic_v1_mc,TrackerAlignmentErrorExtendedRcd,frontier://FrontierProd/CMS_CONDITIONS+MuonDTAPEObjectsExtended_v0_mc,DTAlignmentErrorExtendedRcd,frontier://FrontierProd/CMS_CONDITIONS+MuonCSCAPEObjectsExtended_v0_mc,CSCAlignmentErrorExtendedRcd,frontier://FrontierProd/CMS_CONDITIONS+EcalSamplesCorrelation_mc,EcalSamplesCorrelationRcd,frontier://FrontierProd/CMS_CONDITIONS+EcalPulseShapes_mc,EcalPulseShapesRcd,frontier://FrontierProd/CMS_CONDITIONS+EcalPulseCovariances_mc,EcalPulseCovariancesRcd,frontier://FrontierProd/CMS_CONDITIONS')
# fix 74X
# see https://hypernews.cern.ch/HyperNews/CMS/get/hlt/3749/1/1/1/1/1/1/1/1/2/1/1/2/1/1/1/1/1/1/1/1/1.html
# before it was:
from Configuration.AlCa.GlobalTag import GlobalTag

if options.isMC :
    #process.GlobalTag = GlobalTag(process.GlobalTag, 'POSTLS171_V15::All', '')
    #process.GlobalTag = GlobalTag(process.GlobalTag, 'POSTLS171_V16::All', '')
    #process.GlobalTag = GlobalTag(process.GlobalTag, 'DESIGN71_V5::All', '')
    #process.GlobalTag = GlobalTag(process.GlobalTag, 'DESIGN72_V1::All', '')
    #process.GlobalTag = GlobalTag(process.GlobalTag, 'MCRUN2_74_V9::All', '')
    #process.GlobalTag = GlobalTag(process.GlobalTag, 'MCRUN2_74_V9::All', '')
    process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc_GRun', '')
    
    
    #process.GlobalTag = GlobalTag(process.GlobalTag, 'PRE_LS171_V5A::All', '')
       
    #process.ecalMultiFitUncalibRecHit.algoPSet = cms.PSet(
                                             #useLumiInfoRunHeader = cms.bool(False),
                                             #activeBXs = cms.vint32(-4,-2,0,2,4)
                                             #)

    #process.ecalMultiFitUncalibRecHit.algoPSet = cms.PSet(
                                             #useLumiInfoRunHeader = cms.bool(False),
                                             #activeBXs = cms.vint32(-5,-4,-3,-2,-1,0,1,2,3,4)
                                             #)

#/''
    
else :
    #process.GlobalTag.globaltag = 'GR_R_71_V4::All'
    process.GlobalTag.globaltag = 'GR_R_74_V12A::All' 
    #process.GlobalTag.globaltag = 'FT_R_70_V1::All'
    #process.GlobalTag = GlobalTag(process.GlobalTag, 'MCRUN2_74_V9::All', '')
    
 

#Exception Message:
#Principal::getByToken: Found zero products matching all criteria
#Looking for type: FEDRawDataCollection
#Looking for module label: hltSelectedElectronFEDListProducerGsf
#Looking for productInstanceName: StreamElectronRawFed


#FEDRawDataCollection               "hltSelectedElectronFEDListProducerGsf"   "StreamElectronRawFed"   "TEST"       



### some fix for the stream
#process.csctfDigis.producer       = cms.InputTag("hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed")

process.dttfDigis.DTTF_FED_Source = cms.InputTag("hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed")

process.ecalDigis.InputLabel      = cms.InputTag("hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed")

process.ecalPreshowerDigis.sourceTag = cms.InputTag("hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed")

process.castorDigis.InputLabel    = cms.InputTag("hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed")

process.gctDigis.inputLabel       = cms.InputTag("hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed")

process.gtDigis.DaqGtInputTag     = cms.InputTag("hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed")

#process.gtEvmDigis.EvmGtInputTag  = cms.InputTag("hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed")

process.hcalDigis.InputLabel      = cms.InputTag("hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed")

process.muonCSCDigis.InputObjects = cms.InputTag("hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed")

process.muonDTDigis.inputLabel    = cms.InputTag("hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed")

process.muonRPCDigis.InputLabel   = cms.InputTag("hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed")

process.scalersRawToDigi.scalersInputTag = cms.InputTag("hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed")

process.siPixelDigis.InputLabel   = cms.InputTag("hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed")

process.siStripDigis.ProductLabel = cms.InputTag("hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed")


# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOoutput_step = cms.EndPath(process.RECOoutput)

##### change in the output
process.DigiToRawFEVT.outputCommands          += cms.untracked.vstring('keep FEDRawDataCollection_source_*_*',
                                                                       'keep FEDRawDataCollection_hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed_*_*')

process.FEVTDEBUGEventContent.outputCommands  += cms.untracked.vstring('keep FEDRawDataCollection_hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed_*_*')

process.FEVTEventContent.outputCommands       += cms.untracked.vstring('keep FEDRawDataCollection_hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed_*_*') 

process.FEVTHLTALLEventContent.outputCommands += cms.untracked.vstring('keep FEDRawDataCollection_hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed_*_*') 

process.FEVTSIMEventContent.outputCommands    += cms.untracked.vstring('keep FEDRawDataCollection_hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed_*_*') 

process.GENRAWEventContent.outputCommands     += cms.untracked.vstring('keep FEDRawDataCollection_hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed_*_*') 

process.HLTDEBUGEventContent.outputCommands   += cms.untracked.vstring('keep FEDRawDataCollection_hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed_*_*') 

process.HLTDebugFEVT.outputCommands           += cms.untracked.vstring('keep FEDRawDataCollection_hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed_*_*') 

process.HLTDebugRAW.outputCommands            += cms.untracked.vstring('keep FEDRawDataCollection_hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed_*_*') 

process.HLTriggerRAW.outputCommands           += cms.untracked.vstring('keep FEDRawDataCollection_hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed_*_*') 

process.L1TriggerRAW.outputCommands           += cms.untracked.vstring('keep FEDRawDataCollection_hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed_*_*') 

process.L1TriggerRAWDEBUG.outputCommands      += cms.untracked.vstring('keep FEDRawDataCollection_hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed_*_*') 

process.RAWDEBUGEventContent.outputCommands   += cms.untracked.vstring('keep FEDRawDataCollection_hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed_*_*') 

process.RAWDEBUGHLTEventContent.outputCommands += cms.untracked.vstring('keep FEDRawDataCollection_hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed_*_*') 

process.RAWEventContent.outputCommands         += cms.untracked.vstring('keep FEDRawDataCollection_hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed_*_*') 

process.RAWRECODEBUGHLTEventContent.outputCommands += cms.untracked.vstring('keep FEDRawDataCollection_hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed_*_*')

process.RAWRECOSIMHLTEventContent.outputCommands += cms.untracked.vstring('keep FEDRawDataCollection_hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed_*_*')

process.RAWSIMEventContent.outputCommands        += cms.untracked.vstring('keep FEDRawDataCollection_hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed_*_*')

process.RAWSIMHLTEventContent.outputCommands     += cms.untracked.vstring('keep FEDRawDataCollection_hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed_*_*')

process.REPACKRAWEventContent.outputCommands     += cms.untracked.vstring('keep FEDRawDataCollection_hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed_*_*')

process.REPACKRAWSIMEventContent.outputCommands  += cms.untracked.vstring('keep FEDRawDataCollection_hltSelectedElectronFEDListProducerGsf:StreamElectronRawFed_*_*')

### keep some more info from trigger level

process.RAWRECOEventContent.outputCommands +=  cms.untracked.vstring('keep *_*hltFixedGridRho*_*_*',
                                                                     'keep *_*hltEgammaEcalPFClusterIso_*_*',
                                                                     'keep *_*hltEgammaEleGsfTrackIso_*_*',
                                                                     'keep *_*hltEgammaHcalPFClusterIso_*_*',
                                                                     'keep *_*TriggerResults*_*_*',
                                                                     'keep *_*hltTriggerSummaryAOD*_*_*',
                                                                     'keep *_*hltPixelVertices*_*_*',
                                                                     'keep *_*hltPFMETProducer*_*_*')



process.RECOEventContent.outputCommands += cms.untracked.vstring('keep *_*hltFixedGridRho*_*_*',
                                                                 'keep *_*hltEgammaEcalPFClusterIso_*_*',
                                                                 'keep *_*hltEgammaEleGsfTrackIso_*_*',
                                                                 'keep *_*hltEgammaHcalPFClusterIso_*_*',
                                                                 'keep *_*TriggerResults*_*_*',
                                                                 'keep *_*hltTriggerSummaryAOD*_*_*',
                                                                 'keep *_*hltPixelVertices*_*_*',
                                                                 'keep *_*hltPFMETProducer*_*_*')

process.RECOoutput.outputCommands += cms.untracked.vstring('keep *_*hltFixedGridRho*_*_*',
                                                           'keep *_*hltEgammaEcalPFClusterIso_*_*',
                                                           'keep *_*hltEgammaEleGsfTrackIso_*_*',
                                                           'keep *_*hltEgammaHcalPFClusterIso_*_*',
                                                           'keep *_*TriggerResults*_*_*',
                                                           'keep *_*hltTriggerSummaryAOD*_*_*',
                                                           'keep *_*hltPixelVertices*_*_*',
                                                           'keep *_*hltPFMETProducer*_*_*')


process.RECOSIMEventContent.outputCommands += cms.untracked.vstring('keep *_*hltFixedGridRho*_*_*',
                                                           'keep *_*hltEgammaEcalPFClusterIso_*_*',
                                                           'keep *_*hltEgammaEleGsfTrackIso_*_*',
                                                           'keep *_*hltEgammaHcalPFClusterIso_*_*',
                                                           'keep *_*TriggerResults*_*_*',
                                                           'keep *_*hltTriggerSummaryAOD*_*_*',
                                                           'keep *_*hltPixelVertices*_*_*',
                                                           'keep *_*hltPFMETProducer*_*_*')

process.AODEventContent.outputCommands += cms.untracked.vstring('keep *_*hltFixedGridRho*_*_*',
                                                           'keep *_*hltEgammaEcalPFClusterIso_*_*',
                                                           'keep *_*hltEgammaEleGsfTrackIso_*_*',
                                                           'keep *_*hltEgammaHcalPFClusterIso_*_*',
                                                           'keep *_*TriggerResults*_*_*',
                                                           'keep *_*hltTriggerSummaryAOD*_*_*',
                                                           'keep *_*hltPixelVertices*_*_*',
                                                           'keep *_*hltPFMETProducer*_*_*')

process.AODSIMEventContent.outputCommands += cms.untracked.vstring('keep *_*hltFixedGridRho*_*_*',
                                                           'keep *_*hltEgammaEcalPFClusterIso_*_*',
                                                           'keep *_*hltEgammaEleGsfTrackIso_*_*',
                                                           'keep *_*hltEgammaHcalPFClusterIso_*_*',
                                                           'keep *_*TriggerResults*_*_*',
                                                           'keep *_*hltTriggerSummaryAOD*_*_*',
                                                           'keep *_*hltPixelVertices*_*_*',
                                                           'keep *_*hltPFMETProducer*_*_*')


# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.endjob_step,process.RECOoutput_step)

############################
## Dump the output Python ##
############################

processDumpFile = open('processDump.py', 'w')
print >> processDumpFile, process.dumpPython()

if options.isMC :

    # Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.postLS1Customs
    from SLHCUpgradeSimulations.Configuration.postLS1Customs import customisePostLS1 

    #call to customisation function customisePostLS1 imported from SLHCUpgradeSimulations.Configuration.postLS1Customs
    process = customisePostLS1(process)

    # fix for stream & csc MC
    process.csc2DRecHits.wireDigiTag = cms.InputTag("muonCSCDigis","MuonCSCWireDigi")
    process.csc2DRecHits.stripDigiTag = cms.InputTag("muonCSCDigis","MuonCSCStripDigi")

#process.csc2DRecHits.wireDigiTag = cms.InputTag("muonCSCDigis","MuonCSCWireDigi")
#process.csc2DRecHits.stripDigiTag = cms.InputTag("muonCSCDigis","MuonCSCStripDigi")

