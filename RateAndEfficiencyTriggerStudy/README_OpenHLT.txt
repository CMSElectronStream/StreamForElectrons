###############################
##### Open HLT Software #######
###############################

Install the package

cmsrel CMSSW_7_1_0_pre8
cd CMSSW_7_1_0_pre8/src
cmsenv

git cms-addpkg HLTrigger/Configuration
cd HLTrigger/Configuration/test
./cmsDriver.sh
cp -r /afs/cern.ch/user/z/zdemirag/public/forOpenHLT/CMSSW_7XY OpenHLT
cd OpenHLT

#### code setup

Useful Twiki: 1) Open HLT Twiki: https://twiki.cern.ch/twiki/bin/viewauth/CMS/NewOpenHLT
              2) Global HLT : https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideGlobalHLT
              3) TSG Samples : https://twiki.cern.ch/twiki/bin/viewauth/CMS/TriggerStudiesFall13MC62X


How to  run open HLT example: 

1) Create the reduced HLT trigger python from the full menù: hltGetConfiguration /dev/CMSSW_7_1_0/GRun/V45 --full --offline --no-output --data --process TEST --globaltag auto:hltonline_GRun --unprescale --paths HLT_IsoMu20_eta2p1_TriCentralPFNoPUJet60_40_25_v1, IsoMu20_eta2p1_v*,IsoMu24_eta2p1_v* > hlt.py

2) Run openHLT in producer mode (file with new HLT collection inside run just one time): 

   python openHLT.py -p -i /store/data/Run2012D/SingleMu/RAW/v1/000/208/307/FE4830E1-C93A-E211-BDBC-00237DDBE0E2.root -o Prod.root -t hlt_modified.py - n 5000 --go

3) Then filterning mode changing tresholds :
   
   python openHLT.py -i Prod.root -o Filt.root -t hlt_modified.py -n 5000 --go

openHLT.py code -> Options:
                   
                   1) --run-producers : run the code in the producer mode if is set to true
                   2) --input-root-files : list of input files separeted by space
                   3) --output-root-file : output file name
                   4) --hlt-config : standard hlt configuration file
                   5) --other-changes : process.[module name].[parameter]=cms.[data type]( value ) to be added to the python code  
                   6) --n-events : maximum number of events to process
                   7) --crab-job : run inside a crab job
                   8) --openhlt-template-file : open HLT template file
                   9) --go : start HLT with the go file 


It works in the following way: 1) open the openHLT template $HLTFILE -> hlt_module 

                               2) $CONFIG -> cmd='verbose=%r \nisCrabJob=%r  \nrunProducers=%r \nrunOpen=%r \nifiles=%r \nofile="%s" \nmaxNrEvents=%d' % (args.verbose,args.crab_job,
                                                  args.run_producers,args.run_producers,args.input_root_files,args.output_root_file,maxNrEvents)

                               3) apply other changes: temp=temp.replace("$OTHERCHANGES", other_changes.strip())
   
                               4) open(oHLTconfig_out, "w").write(temp) -> write this in the go file

                               5) cmd="time cmsRun "+oHLTconfig_out -> only if go execute



openHLT.TEMPLATE:

 1) from $HLTFILE import * -> everytging is imported from the origin HLTMenu file that you want to run

 2) Create input files: 
  
    for i in range(len(ifiles)):
     fname=filePrefex+ifiles[i].strip()
     print "< :",fname
     process.source.fileNames.extend([fname,])
     print "> : ("+os.environ["USER"]+"@"+socket.gethostname()+":) "+ofile
     print "--------------"
     if verbose: print "Maximum number of events:", maxNrEvents

 3) process.load('Configuration/EventContent/EventContent_cff') -> load of the event content
 
 4) Light output format -> no RAW FED inside

    process.output = cms.OutputModule("PoolOutputModule",
                                       splitLevel = cms.untracked.int32(0),
                                       outputCommands =cms.untracked.vstring("drop *","keep *_TriggerResults_*_*","keep *_hltTriggerSummaryAOD_*_*"),
                                       fileName = cms.untracked.string(ofile),
                                       dataset = cms.untracked.PSet(dataTier = cms.untracked.string('HLTDEBUG'),))

   if outputRAW:
     process.output.outputCommands=cms.untracked.vstring("drop *","keep *_rawDataCollector_*_*","keep *_addPileupInfo_*_*","keep *_TriggerResults_*_*","keep *_hltTriggerSummaryAOD_*_*")

 5)  process.HLTOutput_openhlt = cms.EndPath(process.output) -> probably here we have to introduce our producer for the stream or change the streamA  in the hlt.py

 6) Loop  on all the modules and dump only  producers or filters storing the parameters

 
 