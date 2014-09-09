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
options.register ('isMC',                         False, VarParsing.multiplicity.singleton, VarParsing.varType.bool, 'is MC or is Data')
options.parseArguments()
print options




process = cms.Process('reRECO')

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
    input = cms.untracked.int32(5)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring(
        options.inputFiles
       #'/store/group/alca_ecalcalib/ecalMIBI/rgerosa/ElectronStreamStudy/TriggerLevel/SingleElectron_8TeV_Run2012C/AlcaElectronStream/streamElectronRAW_alcastream_170_2_6Fi.root'
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
    fileName = cms.untracked.string(options.outputFile),
    #fileName = cms.untracked.string('step2_rereco_onStream.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('RECO')
    )
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag

if options.isMC :
    #process.GlobalTag = GlobalTag(process.GlobalTag, 'POSTLS171_V15::All', '')
    #process.GlobalTag = GlobalTag(process.GlobalTag, 'POSTLS171_V16::All', '')
    process.GlobalTag = GlobalTag(process.GlobalTag, 'DESIGN71_V5::All', '')
    print "MC GT"
    #process.GlobalTag = GlobalTag(process.GlobalTag, 'PRE_LS171_V5A::All', '')
    
else :
    process.GlobalTag.globaltag = 'GR_R_62_V1::All'
 


### some fix for the stream
process.csctfDigis.producer       = cms.InputTag("HLTselectedElectronFEDList:StremElectronRawFedData")

process.dttfDigis.DTTF_FED_Source = cms.InputTag("HLTselectedElectronFEDList:StremElectronRawFedData")

process.ecalDigis.InputLabel      = cms.InputTag("HLTselectedElectronFEDList:StremElectronRawFedData")

process.ecalPreshowerDigis.sourceTag = cms.InputTag("HLTselectedElectronFEDList:StremElectronRawFedData")

process.castorDigis.InputLabel    = cms.InputTag("HLTselectedElectronFEDList:StremElectronRawFedData")

process.gctDigis.inputLabel       = cms.InputTag("HLTselectedElectronFEDList:StremElectronRawFedData")

process.gtDigis.DaqGtInputTag     = cms.InputTag("HLTselectedElectronFEDList:StremElectronRawFedData")

process.gtEvmDigis.EvmGtInputTag  = cms.InputTag("HLTselectedElectronFEDList:StremElectronRawFedData")

process.hcalDigis.InputLabel      = cms.InputTag("HLTselectedElectronFEDList:StremElectronRawFedData")

process.muonCSCDigis.InputObjects = cms.InputTag("HLTselectedElectronFEDList:StremElectronRawFedData")

process.muonDTDigis.inputLabel    = cms.InputTag("HLTselectedElectronFEDList:StremElectronRawFedData")

process.muonRPCDigis.InputLabel   = cms.InputTag("HLTselectedElectronFEDList:StremElectronRawFedData")

process.scalersRawToDigi.scalersInputTag = cms.InputTag("HLTselectedElectronFEDList:StremElectronRawFedData")

process.siPixelDigis.InputLabel   = cms.InputTag("HLTselectedElectronFEDList:StremElectronRawFedData")

process.siStripDigis.ProductLabel = cms.InputTag("HLTselectedElectronFEDList:StremElectronRawFedData")

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOoutput_step = cms.EndPath(process.RECOoutput)

##### change in the output
process.DigiToRawFEVT.outputCommands          += cms.untracked.vstring('keep FEDRawDataCollection_source_*_*',
                                                                       'keep FEDRawDataCollection_HLTselectedElectronFEDList:StremElectronRawFedData_*_*')

process.FEVTDEBUGEventContent.outputCommands  += cms.untracked.vstring('keep FEDRawDataCollection_HLTselectedElectronFEDList:StremElectronRawFedData_*_*')

process.FEVTEventContent.outputCommands       += cms.untracked.vstring('keep FEDRawDataCollection_HLTselectedElectronFEDList:StremElectronRawFedData_*_*') 

process.FEVTHLTALLEventContent.outputCommands += cms.untracked.vstring('keep FEDRawDataCollection_HLTselectedElectronFEDList:StremElectronRawFedData_*_*') 

process.FEVTSIMEventContent.outputCommands    += cms.untracked.vstring('keep FEDRawDataCollection_HLTselectedElectronFEDList:StremElectronRawFedData_*_*') 

process.GENRAWEventContent.outputCommands     += cms.untracked.vstring('keep FEDRawDataCollection_HLTselectedElectronFEDList:StremElectronRawFedData_*_*') 

process.HLTDEBUGEventContent.outputCommands   += cms.untracked.vstring('keep FEDRawDataCollection_HLTselectedElectronFEDList:StremElectronRawFedData_*_*') 

process.HLTDebugFEVT.outputCommands           += cms.untracked.vstring('keep FEDRawDataCollection_HLTselectedElectronFEDList:StremElectronRawFedData_*_*') 

process.HLTDebugRAW.outputCommands            += cms.untracked.vstring('keep FEDRawDataCollection_HLTselectedElectronFEDList:StremElectronRawFedData_*_*') 

process.HLTriggerRAW.outputCommands           += cms.untracked.vstring('keep FEDRawDataCollection_HLTselectedElectronFEDList:StremElectronRawFedData_*_*') 

process.L1TriggerRAW.outputCommands           += cms.untracked.vstring('keep FEDRawDataCollection_HLTselectedElectronFEDList:StremElectronRawFedData_*_*') 

process.L1TriggerRAWDEBUG.outputCommands      += cms.untracked.vstring('keep FEDRawDataCollection_HLTselectedElectronFEDList:StremElectronRawFedData_*_*') 

process.RAWDEBUGEventContent.outputCommands   += cms.untracked.vstring('keep FEDRawDataCollection_HLTselectedElectronFEDList:StremElectronRawFedData_*_*') 

process.RAWDEBUGHLTEventContent.outputCommands += cms.untracked.vstring('keep FEDRawDataCollection_HLTselectedElectronFEDList:StremElectronRawFedData_*_*') 

process.RAWEventContent.outputCommands         += cms.untracked.vstring('keep FEDRawDataCollection_HLTselectedElectronFEDList:StremElectronRawFedData_*_*') 

process.RAWRECODEBUGHLTEventContent.outputCommands += cms.untracked.vstring('keep FEDRawDataCollection_HLTselectedElectronFEDList:StremElectronRawFedData_*_*')

process.RAWRECOSIMHLTEventContent.outputCommands += cms.untracked.vstring('keep FEDRawDataCollection_HLTselectedElectronFEDList:StremElectronRawFedData_*_*')

process.RAWSIMEventContent.outputCommands        += cms.untracked.vstring('keep FEDRawDataCollection_HLTselectedElectronFEDList:StremElectronRawFedData_*_*')

process.RAWSIMHLTEventContent.outputCommands     += cms.untracked.vstring('keep FEDRawDataCollection_HLTselectedElectronFEDList:StremElectronRawFedData_*_*')

process.REPACKRAWEventContent.outputCommands     += cms.untracked.vstring('keep FEDRawDataCollection_HLTselectedElectronFEDList:StremElectronRawFedData_*_*')

process.REPACKRAWSIMEventContent.outputCommands  += cms.untracked.vstring('keep FEDRawDataCollection_HLTselectedElectronFEDList:StremElectronRawFedData_*_*')

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



