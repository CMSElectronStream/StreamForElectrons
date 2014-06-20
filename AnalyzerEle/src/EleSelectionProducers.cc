#include "StreamForElectrons/AnalyzerEle/interface/EleSelectionProducers.h"

EleSelectionProducers::EleSelectionProducers(const edm::ParameterSet& iConfig):
  electronsTAG(iConfig.getParameter<edm::InputTag>("electronCollection")),
  conversionsProducerTAG(iConfig.getParameter<edm::InputTag>("conversionCollection")),
  BeamSpotTAG(iConfig.getParameter<edm::InputTag>("BeamSpotCollection")),
  VertexTAG(iConfig.getParameter<edm::InputTag>("vertexCollection")),
  chIsoValsTAG(iConfig.getParameter<edm::InputTag>("chIsoVals")),
  emIsoValsTAG(iConfig.getParameter<edm::InputTag>("emIsoVals")),
  nhIsoValsTAG(iConfig.getParameter<edm::InputTag>("nhIsoVals")),
  rhoTAG(iConfig.getParameter<edm::InputTag>("rhoFastJet")), 
  fiducial_selector("fiducial", electronsHandle, conversionsHandle, bsHandle, vertexHandle,
		    chIsoValsHandle, emIsoValsHandle, nhIsoValsHandle, rhoHandle),
  relIso70_selector("relIso70", electronsHandle, conversionsHandle, bsHandle, vertexHandle,
		    chIsoValsHandle, emIsoValsHandle, nhIsoValsHandle, rhoHandle),
  relIso80_selector("relIso80", electronsHandle, conversionsHandle, bsHandle, vertexHandle,
		    chIsoValsHandle, emIsoValsHandle, nhIsoValsHandle, rhoHandle),
  relIso90_selector("relIso90", electronsHandle, conversionsHandle, bsHandle, vertexHandle,
		    chIsoValsHandle, emIsoValsHandle, nhIsoValsHandle, rhoHandle),
  cIso70_selector("cIso70", electronsHandle, conversionsHandle, bsHandle, vertexHandle,
		    chIsoValsHandle, emIsoValsHandle, nhIsoValsHandle, rhoHandle),
  cIso80_selector("cIso80", electronsHandle, conversionsHandle, bsHandle, vertexHandle,
		    chIsoValsHandle, emIsoValsHandle, nhIsoValsHandle, rhoHandle),
  cIso90_selector("cIso90", electronsHandle, conversionsHandle, bsHandle, vertexHandle,
  chIsoValsHandle, emIsoValsHandle, nhIsoValsHandle, rhoHandle){

  produces< SelectionMap >("fiducial");
  produces< SelectionMap >("relIso70");
  produces< SelectionMap >("relIso80");
  produces< SelectionMap >("relIso90");
  
  produces< SelectionMap >("cIso70");
  produces< SelectionMap >("cIso80");
  produces< SelectionMap >("cIso90");
  
}


EleSelectionProducers::~EleSelectionProducers(){}


// ------------ method called to produce the data  ------------
void EleSelectionProducers::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){

  using namespace edm;
  std::vector<SelectionValue_t>  fiducial_vec;
  std::vector<SelectionValue_t>  relIso70_vec;
  std::vector<SelectionValue_t>  relIso80_vec;
  std::vector<SelectionValue_t>  relIso90_vec;
  
  std::auto_ptr<SelectionMap> fiducialMap(new SelectionMap());
  std::auto_ptr<SelectionMap> relIso70_Map(new SelectionMap());
  std::auto_ptr<SelectionMap> relIso80_Map(new SelectionMap());
  std::auto_ptr<SelectionMap> relIso90_Map(new SelectionMap());
  std::vector<SelectionValue_t>  cIso70_vec;
  std::vector<SelectionValue_t>  cIso80_vec;
  std::vector<SelectionValue_t>  cIso90_vec;
  std::auto_ptr<SelectionMap> cIso70_Map(new SelectionMap());
  std::auto_ptr<SelectionMap> cIso80_Map(new SelectionMap());
  std::auto_ptr<SelectionMap> cIso90_Map(new SelectionMap());
  
  //------------------------------ ELECTRON
  iEvent.getByLabel(electronsTAG, electronsHandle);
   
  //------------------------------ CONVERSIONS
  iEvent.getByLabel(conversionsProducerTAG, conversionsHandle);
  iEvent.getByLabel(BeamSpotTAG, bsHandle);
  iEvent.getByLabel(VertexTAG, vertexHandle);
  //------------------------------ RHO
  iEvent.getByLabel(rhoTAG,rhoHandle);

  //------------------------------ ISO DEPOSITS
  iEvent.getByLabel(chIsoValsTAG, chIsoValsHandle);
  iEvent.getByLabel(emIsoValsTAG, emIsoValsHandle);  
  iEvent.getByLabel(nhIsoValsTAG, nhIsoValsHandle);
  if(!chIsoValsHandle.isValid() or !emIsoValsHandle.isValid() or !nhIsoValsHandle.isValid()){
    chIsoValsTAG=edm::InputTag(chIsoValsTAG.label().substr(0,chIsoValsTAG.label().find("PFIso",chIsoValsTAG.label().size()-6))+"Gsf", chIsoValsTAG.instance(), chIsoValsTAG.process());
    emIsoValsTAG=edm::InputTag(emIsoValsTAG.label().substr(0,emIsoValsTAG.label().find("PFIso",emIsoValsTAG.label().size()-6))+"Gsf", emIsoValsTAG.instance(), emIsoValsTAG.process());
    nhIsoValsTAG=edm::InputTag(nhIsoValsTAG.label().substr(0,nhIsoValsTAG.label().find("PFIso",nhIsoValsTAG.label().size()-6))+"Gsf", nhIsoValsTAG.instance(), nhIsoValsTAG.process());

    iEvent.getByLabel(chIsoValsTAG, chIsoValsHandle);
    iEvent.getByLabel(emIsoValsTAG, emIsoValsHandle);  
    iEvent.getByLabel(nhIsoValsTAG, nhIsoValsHandle);

  }
  
   
#ifdef DEBUG
  std::cout << "[DEBUG] Starting loop over electrons" << std::endl;
#endif
  for(reco::GsfElectronCollection::const_iterator ele_itr = (electronsHandle)->begin(); 
      ele_itr != (electronsHandle)->end(); ele_itr++){
    const reco::GsfElectronRef eleRef(electronsHandle, ele_itr-electronsHandle->begin());
    // the new tree has one event per each electron
    //    pat::strbitset fiducial_ret;
    //pat::strbitset cIso70_ret;
    pat::strbitset cIso80_ret;
    //pat::strbitset cIso90_ret;
    //pat::strbitset relIso70_ret;
    pat::strbitset relIso80_ret;
    //pat::strbitset relIso90_ret;
    /*    
    fiducial_selector(eleRef, fiducial_ret);
    fiducial_vec.push_back(fiducial_selector.result());
    
    relIso70_selector(eleRef, relIso70_ret);*/
    relIso80_selector(eleRef, relIso80_ret);
    //    relIso90_selector(eleRef, relIso90_ret);
     
    // relIso70_vec.push_back(relIso70_selector.result()); // result gives a float
    relIso80_vec.push_back(relIso80_selector.result()); // result gives a float
    //  relIso90_vec.push_back(relIso90_selector.result()); // result gives a float
    
    //cIso70_selector(eleRef, cIso70_ret);
    cIso80_selector(eleRef, cIso80_ret);
    //cIso90_selector(eleRef, cIso90_ret);

    //cIso70_vec.push_back(cIso70_selector.result());
    cIso80_vec.push_back(cIso80_selector.result());
    // cIso90_vec.push_back(cIso90_selector.result());
    
   // if(((bool)cIso80_selector.result())){
    //  if(!(bool) cIso80_selector.result() || !(bool) cIso90_selector.result()){
//	edm::LogError("Incoherent selection") << "passing tight but not medium or loose";
//	exit (1);
 //     }
//    }

 //   if(((bool)cIso80_selector.result())){
  //    if( !(bool) cIso90_selector.result()){
//	edm::LogError("Incoherent selection") << "passing medium but not loose";
//	exit (1);
//      }
//    }
    
#ifdef DEBUG
    std::cout << "[DEBUG] relIso80 ret=" << relIso80_selector.bitMask() << std::endl;
    std::cout << "[DEBUG] relIso80 ret= (float)" << (SelectionValue_t) relIso80_selector.bitMask() << std::endl;
    std::cout << "[DEBUG] cIso80 ret=" << cIso80_selector.bitMask() << std::endl;
    std::cout << "[DEBUG] cIso80 ret= (float)" << (SelectionValue_t) cIso80_selector.bitMask() << std::endl;

#endif
  }

  //prepare product 
  // declare the filler of the ValueMap
  //SelectionMap::Filler fiducial_filler(*fiducialMap);
  //SelectionMap::Filler relIso90_filler(*relIso90_Map);
  SelectionMap::Filler relIso80_filler(*relIso80_Map);
  //SelectionMap::Filler relIso70_filler(*relIso70_Map);
  //SelectionMap::Filler cIso90_filler(*cIso90_Map);
  SelectionMap::Filler cIso80_filler(*cIso80_Map);
  //SelectionMap::Filler cIso70_filler(*cIso70_Map);
  
  //fill and insert valuemap
  //fiducial_filler.insert(electronsHandle,fiducial_vec.begin(),fiducial_vec.end());
  
  //cIso90_filler.insert(electronsHandle,cIso90_vec.begin(),cIso90_vec.end());
  cIso80_filler.insert(electronsHandle,cIso80_vec.begin(),cIso80_vec.end());
  //cIso70_filler.insert(electronsHandle,cIso70_vec.begin(),cIso70_vec.end());

  //relIso90_filler.insert(electronsHandle,relIso90_vec.begin(),relIso90_vec.end());
  relIso80_filler.insert(electronsHandle,relIso80_vec.begin(),relIso80_vec.end());
  //relIso70_filler.insert(electronsHandle,relIso70_vec.begin(),relIso70_vec.end());
  
  //fiducial_filler.fill();
  //relIso90_filler.fill();
  relIso80_filler.fill();
  //relIso70_filler.fill();

  //cIso90_filler.fill();
  cIso80_filler.fill();
  //cIso70_filler.fill();
  
  //------------------------------
  // put the ValueMap in the event
  
  //iEvent.put(fiducialMap, "fiducial");
 // iEvent.put(relIso90_Map,"relIso90");
  iEvent.put(relIso80_Map,"relIso80");
  //iEvent.put(relIso70_Map,"relIso70");
  //iEvent.put(cIso90_Map,  "cIso90");
  iEvent.put(cIso80_Map,  "cIso80");
  //iEvent.put(cIso70_Map,  "cIso70");

}

// ------------ method called once each job just before starting event loop  ------------
void EleSelectionProducers::beginJob(){}

// ------------ method called once each job just after ending the event loop  ------------
void EleSelectionProducers::endJob() {}

// ------------ method called when starting to processes a run  ------------
void EleSelectionProducers::beginRun(edm::Run&, edm::EventSetup const&){}

// ------------ method called when ending the processing of a run  ------------
void EleSelectionProducers::endRun(edm::Run&, edm::EventSetup const&){}

// ------------ method called when starting to processes a luminosity block  ------------
void EleSelectionProducers::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&){}

// ------------ method called when ending the processing of a luminosity block  ------------
void EleSelectionProducers::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&){}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void EleSelectionProducers::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(EleSelectionProducers);
