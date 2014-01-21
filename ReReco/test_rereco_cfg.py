# Auto generated configuration file
# using: 
# Revision: 1.222.2.6 
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: step2 -s RAW2DIGI,L1Reco,RECO --data --datatier RECO --eventcontent RECO --conditions GR10_P_V11::All --scenario pp --no_exec --magField AutoFromDBCurrent --process reRECO --customise Configuration/GlobalRuns/reco_TLR_38X.py --cust_function customisePPData --filein=outSkim_1_1_GlC.root
import FWCore.ParameterSet.Config as cms

process = cms.Process('EcalAlignment')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')

process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.EventContent.EventContent_cff')

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    annotation = cms.untracked.string('step2 nevts:1'),
    name = cms.untracked.string('PyReleaseValidation')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.options = cms.untracked.PSet(
)

# Input source
process.source = cms.Source("PoolSource",
  # fileNames = cms.untracked.vstring('file:/data2/amassiro/CMSSWRoot/WZtoAnything_TuneZ2_7TeV-pythia6-tauola_Spring11-PU_S1_START311_V1G1-v1/F0F54048-4A50-E011-9CDE-003048D47792.root')   

    fileNames = cms.untracked.vstring(
        'file:/data/amassiro/CMSSWRoot/DATA2012/SingleElectron_Run2012B_RAW/B865DABE-BDA2-E111-854F-BCAEC53296F7.root'
    ) )

#.root
#process.source.inputCommands = cms.untracked.vstring("drop *_MEtoEDMConverter_*_*", "keep FEDRawDataCollection_*_*_*")
process.source.inputCommands = cms.untracked.vstring("drop *_*_*_RECO", "drop *_MEtoEDMConverter_*_*", "keep FEDRawDataCollection_*_*_*")

# Output definition

process.RECOoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    outputCommands = process.RECOEventContent.outputCommands,
    fileName = cms.untracked.string('outSkim_V3.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('RECO')
    )
)

process.out = cms.OutputModule("PoolOutputModule",
  outputCommands = cms.untracked.vstring(
       'keep *',
       #'keep *_gsfElectrons_*_*'
       #'keep *_*_*_EcalAlignmentBIS'
       #'drop *_gsfElectron_*_*',
       #'drop *_gsfElectronCores_*_*'
        ),
  fileName = cms.untracked.string("outCMSSW.root")
)


# Additional output definition

# Other statements
process.GlobalTag.globaltag = 'GR_R_53_V16D::All'


process.GlobalTag.toGet = cms.VPSet(
      #cms.PSet(record = cms.string("EcalIntercalibConstantsRcd"),
             #tag = cms.string("EcalIntercalibConstants_Bon_V20100803"),
             #connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_ECAL")
             #),
    #cms.PSet(record = cms.string("EcalADCToGeVConstantRcd"),
             #tag = cms.string("EcalADCToGeVConstant_Bon_V20100803"),
             #connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_ECAL")
             #),


  #  cms.PSet(record = cms.string("EBAlignmentRcd"),
  #           tag = cms.string("EBAlignment_measured_v07_offline"),
  #           connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_34X_ECAL")   #### New ####
  #           ),
  #  cms.PSet(record = cms.string("EEAlignmentRcd"),
  #           tag = cms.string("EEAlignment_measured_v07_offline"),
  #           connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_34X_ECAL")  #### New ####
  #           )

    #cms.PSet(record = cms.string("EBAlignmentRcd"),
             #tag = cms.string("EBAlignment_measured_v08_offline"),
             #connect = cms.untracked.string("sqlite_file:EBAlign_2012.db")   #### New ####
             #),
    #cms.PSet(record = cms.string("EEAlignmentRcd"),
             #tag = cms.string("EEAlignment_measured_v08_offline"),
             #connect = cms.untracked.string("sqlite_file:EEAlign_2012.db")  #### New ####
             #),

    #cms.PSet(record = cms.string("ESAlignmentRcd"),
             #tag = cms.string("ESAlignment_measured_v01_offline"),
             #connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_PRESHOWER")
             #),
    #cms.PSet(record = cms.string("EcalLaserAlphasRcd"),
             #tag = cms.string("EcalLaserAlphas_test_prompt"),
             #connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_ECAL")
             #),
    #cms.PSet(record = cms.string("EcalLaserAPDPNRatiosRcd"),
             #tag = cms.string("EcalLaserAPDPNRatios_v0_online"),
             #connect = cms.untracked.string("frontier://FrontierPrep/CMS_COND_ECAL")
             #),
    #cms.PSet(record = cms.string("GlobalPositionRcd"),
             #tag = cms.string("GlobalAlignment_v2_offline"),
             #connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_ALIGNMENT")
             #),

    cms.PSet(record = cms.string("TrackerAlignmentRcd"),
             tag = cms.string("Alignments"),
             connect = cms.untracked.string("sqlite_file:/afs/cern.ch/cms/CAF/CMSALCA/ALCA_TRACKERALIGN/MP/MPproduction/mp1280/jobData/jobm/alignments_MP_fixed.db")
             )
    #cms.PSet(record = cms.string("TrackerAlignmentErrorRcd"),
             #tag = cms.string("TrackerAlignmentErrors_GR10_v2_offline"),
             #connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_ALIGNMENT")
             #)
    )


# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction)
process.endjob_step = cms.Path(process.endOfProcess)
#process.RECOoutput_step = cms.EndPath(process.RECOoutput)

# Schedule definition
#process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.endjob_step,process.RECOoutput_step)



#--------------------------
# paths
#--------------------------


process.schedule = cms.Schedule(
   process.raw2digi_step,        # | -> reconstruction
   process.L1Reco_step,          # | -> reconstruction
   process.reconstruction_step,  # | -> reconstruction
   process.endjob_step           # | -> reconstruction
   #process.RECOoutput_step,      # | -> reconstruction
   #process.pEcalAlignment        # | -> selections and ntuple
)

