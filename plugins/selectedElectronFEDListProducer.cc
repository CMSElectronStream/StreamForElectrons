#include "HLTrigger/Egamma/interface/selectedElectronFEDListProducer.h"

selectedElectronFEDListProducer::selectedElectronFEDListProducer(const edm::ParameterSet &  iConfig){

 if( iConfig.existsAs<std::vector<edm::InputTag> >("electronCollections") ){
   electronCollections_ = iConfig.getParameter< std::vector<edm::InputTag> >("electronCollections");
   if(electronCollections_.empty()){
      throw cms::Exception("Configuration")<<"[selectedElectronFEDListProducer] empty electron collection is given --> at least one \n"; 
   }
 }
 else{ throw cms::Exception("Configuration")<<"[selectedElectronFEDListProducer] no electron collection are given --> need at least one \n";  }

 if( iConfig.existsAs<edm::FileInPath>("ESLookupTable") ){
   ESLookupTable_ = iConfig.getParameter< edm::FileInPath >("ESLookupTable");
 }
 else{ ESLookupTable_ = edm::FileInPath("EventFilter/ESDigiToRaw/data/ES_lookup_table.dat"); }

 if( iConfig.existsAs<bool>("dumpEcalFedList") ){
   dumpEcalFedList_ = iConfig.getParameter< bool >("dumpEcalFedList");
 }
 else dumpEcalFedList_ = true ;

 if( iConfig.existsAs<bool>("dumpTrackSiStripFedList") ){
   dumpTrackSiStripFedList_ = iConfig.getParameter< bool >("dumpTrackSiStripFedList");
 }
 else dumpTrackSiStripFedList_ = true ;

 if( iConfig.existsAs<bool>("dumpTrackSiPixelFedList") ){
     dumpTrackSiPixelFedList_ = iConfig.getParameter< bool >("dumpTrackSiPixelFedList");
 }
 else dumpTrackSiPixelFedList_ = true ;

 if( iConfig.existsAs<std::string>("outputLabelModule") ){
   outputLabelModule_ = iConfig.getParameter< std::string >("outputLabelModule");
 }
 else outputLabelModule_ = "streamElectronRawData" ;

   
 if( iConfig.existsAs<bool>("debug") ){
   debug_ = iConfig.getParameter< bool >("debug");
 }
 else debug_ = false ;

 if( iConfig.existsAs<edm::InputTag>("rawDataLabel") ){
   rawDataLabel_ = iConfig.getParameter<edm::InputTag>("rawDataLabel");
 }
 else rawDataLabel_ = edm::InputTag("rawDataCollector") ;

 if(debug_){

  std::cout<<"[selectedElectronFEDListProducer] output Label "<<outputLabelModule_<<std::endl; 

  std::cout<<"[selectedElectronFEDListProducer] dumpEcalFedList set to "<<dumpEcalFedList_<<" dumpTrackSiStripFedList "<<dumpTrackSiStripFedList_<<
             " dumpTrackSiPixelFedList "<<dumpTrackSiPixelFedList_<<std::endl;

  std::cout<<"[selectedElectronFEDListProducer] Electron Collections"<<std::endl;
  std::vector<edm::InputTag>::const_iterator Tag = electronCollections_.begin();
  for( ; Tag !=electronCollections_.end(); ++Tag)
     std::cout<<"[selectedElectronFEDListProducer] ele collection: "<<*(Tag)<<std::endl;

  std::cout<<"[selectedElectronFEDListProducer] rawDataInput "<<rawDataLabel_<<std::endl; 

 }
 
 // initialize pre-shower fed id --> look up table
 for (int i=0; i<2; ++i)
  for (int j=0; j<2; ++j)
   for (int k=0 ;k<40; ++k)
    for (int m=0; m<40; m++)
          ES_fedId_[i][j][k][m] = -1;
 
 // read in look-up table
 int nLines, iz, ip, ix, iy, fed, kchip, pace, bundle, fiber, optorx;
 std::ifstream ES_file;
 ES_file.open(ESLookupTable_.fullPath().c_str());
 if(debug_) std::cout<<"[selectedElectronFEDListProducer] Look Up table for ES "<<ESLookupTable_.fullPath().c_str()<<std::endl;
 if( ES_file.is_open() ) {
     ES_file >> nLines;
     for (int i=0; i<nLines; ++i) {
       ES_file >> iz >> ip >> ix >> iy >> fed >> kchip >> pace >> bundle >> fiber >> optorx ;
       ES_fedId_[(3-iz)/2-1][ip-1][ix-1][iy-1] = fed;
     }
 } 
 else std::cout<<"[selectedElectronFEDListProducer] Look up table file can not be found in"<<ESLookupTable_.fullPath().c_str() <<std::endl;
 
 ES_file.close();

 produces<FEDRawDataCollection>(outputLabelModule_);

}

selectedElectronFEDListProducer::~selectedElectronFEDListProducer(){

 if(!electronCollections_.empty()) electronCollections_.clear() ;
 if(!fedList_.empty()) fedList_.clear() ;
 if(!RawDataCollection_) delete RawDataCollection_ ;

}

void selectedElectronFEDListProducer::beginJob(const edm::EventSetup & iSetup){ 
 
  if(debug_){ std::cout<<"[selectedElectronFEDListProducer] Begin of the Job ----> "<<std::endl;
              std::cout<<"[selectedElectronFEDListProducer] event counter set to "<<eventCounter_<<std::endl;
  }
  eventCounter_ = 0 ;
 
} 

void selectedElectronFEDListProducer::produce(edm::Event & iEvent, const edm::EventSetup & iSetup){

  if(!fedList_.empty()) fedList_.clear(); 
  if(!RawDataCollection_) delete RawDataCollection_ ;

 // produce FED,detID strip map
  edm::ESHandle<SiStripFedCabling> stripFedCabling;
  iSetup.get<SiStripFedCablingRcd>().get(stripFedCabling);
  stripFedCabling_ = stripFedCabling.product();

  // get the ecal electronics map
  edm::ESHandle<EcalElectronicsMapping > ecalmapping;
  iSetup.get<EcalMappingRcd >().get(ecalmapping);
  TheMapping_ = ecalmapping.product();
 
  // get the calo geometry_
  edm::ESHandle<CaloGeometry> caloGeometry; 
  iSetup.get<CaloGeometryRecord>().get(caloGeometry);
  geometry_ = caloGeometry.product();

  //ES geometry_
  geometry__ES_ = caloGeometry->getSubdetectorGeometry(DetId::Ecal, EcalPreshower);

  // Strip tracker cabling map
  edm::ESHandle<SiStripDetCabling> stripDetCabling;
  iSetup.get<SiStripDetCablingRcd>().get(stripDetCabling);
  stripReverter_ = stripDetCabling.product();

  // pixel tracker cabling map
  edm::ESHandle<SiPixelFedCablingMap> pixelCablingMap;
  iSetup.get<SiPixelFedCablingMapRcd>().get(pixelCablingMap);
  frameReverter_ = new SiPixelFrameReverter(iSetup,(SiPixelFedCabling*)pixelCablingMap.product());
 

  // Build FED strip map --> just one time
  // Retrieve FED ids from cabling map and iterate through 
  if(eventCounter_ ==0 && stripFedDetIdMap_.empty()){
   if(debug_) std::cout<<"[selectedElectronFEDListProducer] Build strip FEDid, DetId map"<<std::endl;
   std::vector<uint16_t>::const_iterator itFed = stripFedCabling_->feds().begin();
   for ( ; itFed != stripFedCabling_->feds().end(); ++itFed ) {
     const std::vector<FedChannelConnection>& stripConnection = stripFedCabling_->connections(*itFed);
     std::vector<FedChannelConnection>::const_iterator iStrip = stripConnection.begin();
     for ( ; iStrip != stripConnection.end(); ++iStrip ) {
       stripFedDetIdMap_[(*iStrip).detId()] = (*itFed);
   }
  }
  if(debug_){
     std::map<uint32_t,uint32_t>::const_iterator itMap = stripFedDetIdMap_.begin();
     for( ; itMap !=stripFedDetIdMap_.end() ; ++itMap)
       std::cout<<"[selectedElectronFEDListProducer] strip detID "<<itMap->first<<" strip FED "<<stripFedDetIdMap_[itMap->first]<<std::endl;
    }
  }

  // get event raw data
  edm::Handle<FEDRawDataCollection> rawdata;
  iEvent.getByLabel(rawDataLabel_,rawdata);

  // loop on the input electron collection vector
  edm::Handle<trigger::TriggerFilterObjectWithRefs> triggerElectronCollection;
  std::vector< edm::Ref<reco::ElectronCollection> > electrons;
  reco::ElectronRef electron ;
  
  std::vector<edm::InputTag>::const_iterator itTag = electronCollections_.begin();

  for( ; itTag!=electronCollections_.end() ; ++itTag){

   try { iEvent.getByLabel (*itTag,triggerElectronCollection);
         if(triggerElectronCollection.failedToGet()) continue ;
   }
   catch (cms::Exception &exception) {continue;}

   triggerElectronCollection->getObjects(trigger::TriggerElectron, electrons);

   std::vector<edm::Ref<reco::ElectronCollection > >::const_iterator itEle = electrons.begin();

   double radTodeg = 180. / Geom::pi();
   for( ; itEle!=electrons.end() ; ++itEle){

    // get electron supercluster and the associated hit -> detID
    electron = (*itEle);
    reco::SuperClusterRef scRef = electron->superCluster();
    const std::vector<std::pair<DetId,float> >& hits = scRef->hitsAndFractions();
    // start in dump the ecal FED associated to the electron
    std::vector<std::pair<DetId,float> >::const_iterator itSChits = hits.begin();
    int hitFED = 0 ;
    for( ; itSChits!=hits.end() ; ++itSChits){
     
     if((*itSChits).first.subdetId()== EcalBarrel){

         EBDetId idEBRaw ((*itSChits).first);
         GlobalPoint point = geometry_->getPosition(idEBRaw);
         hitFED = FEDNumbering::MINECALFEDID + TheMapping_->GetFED(double(point.eta()),double(point.phi())*radTodeg);

         if(debug_) std::cout<<"[selectedElectronFEDListProducer] electron hit detID Barrel "<<(*itSChits).first.rawId()<<" eta "<<double(point.eta())<<" phi "<< double(point.phi())*radTodeg <<" FED "<<hitFED<<std::endl;

         if(!fedList_.empty()){ 
            if(std::find(fedList_.begin(),fedList_.end(),hitFED)==fedList_.end()) fedList_.push_back(hitFED);
         }
         else fedList_.push_back(hitFED);
     } 
     else if((*itSChits).first.subdetId()== EcalEndcap){
         EEDetId idEERaw ((*itSChits).first);
         GlobalPoint point = geometry_->getPosition(idEERaw);
         hitFED = FEDNumbering::MINECALFEDID + TheMapping_->GetFED(double(point.eta()),double(point.phi())*radTodeg);

         if(debug_) std::cout<<"[selectedElectronFEDListProducer] electron hit detID Endcap "<<(*itSChits).first.rawId()<<" eta "<<double(point.eta())<<" phi "<<double(point.phi())*radTodeg <<" FED "<<hitFED<<std::endl;
         if(!fedList_.empty()){ 
            if(std::find(fedList_.begin(),fedList_.end(),hitFED)==fedList_.end()) fedList_.push_back(hitFED);
         }
         else fedList_.push_back(hitFED);
         // preshower hit for each ecal endcap hit
         DetId tmpX = (dynamic_cast<const EcalPreshowerGeometry*>(geometry__ES_))->getClosestCellInPlane(point,1);
         ESDetId stripX = (tmpX == DetId(0)) ? ESDetId(0) : ESDetId(tmpX);          
         hitFED = ES_fedId_[(3-stripX.zside())/2-1][stripX.plane()-1][stripX.six()-1][stripX.siy()-1];
         if(debug_) std::cout<<"[selectedElectronFEDListProducer] ES hit plane X (deiID) "<<stripX.rawId()<<" six "<<stripX.six()<<" siy "<<stripX.siy()<<" plane "<<stripX.plane()<<" FED ID "<<hitFED<<std::endl;
         if(hitFED < 0) continue;
         if(!fedList_.empty()){ 
            if(std::find(fedList_.begin(),fedList_.end(),hitFED)==fedList_.end()) fedList_.push_back(hitFED);
          }
          else fedList_.push_back(hitFED);
         
         DetId tmpY = (dynamic_cast<const EcalPreshowerGeometry*>(geometry__ES_))->getClosestCellInPlane(point,2);
         ESDetId stripY = (tmpY == DetId(0)) ? ESDetId(0) : ESDetId(tmpY);          
         hitFED = ES_fedId_[(3-stripY.zside())/2-1][stripY.plane()-1][stripY.six()-1][stripY.siy()-1];
         if(debug_) std::cout<<"[selectedElectronFEDListProducer] ES hit plane Y (deiID) "<<stripY.rawId()<<" six "<<stripY.six()<<" siy "<<stripY.siy()<<" plane "<<stripY.plane()<<" FED ID "<<hitFED<<std::endl;
         if(hitFED < 0) continue;
         if(!fedList_.empty()){ 
            if(std::find(fedList_.begin(),fedList_.end(),hitFED)==fedList_.end()) fedList_.push_back(hitFED);
         }
         else fedList_.push_back(hitFED);          
       }   
    } // end loop on SC hit   

    // get the electron track
    reco::TrackRef eleTrack = electron->track();
    trackingRecHit_iterator itEleTrack = eleTrack->recHitsBegin();
    //loop on the track hit
    for( ; itEleTrack!=eleTrack->recHitsEnd(); ++itEleTrack){
       DetId trackHit ((*itEleTrack)->rawId());

       if( trackHit.det() == DetId::Tracker && (trackHit.subdetId() == SiStripDetId::TIB || trackHit.subdetId() == SiStripDetId::TID || trackHit.subdetId() == SiStripDetId::TOB || trackHit.subdetId() == SiStripDetId::TEC)) {
          hitFED = stripFedDetIdMap_[trackHit.rawId()];
          if(debug_) std::cout<<"[selectedElectronFEDListProducer] electron strip hit "<<trackHit.rawId()<<" subdetector "<<trackHit.subdetId()<<" hitFED "<<hitFED<<std::endl;
          if(!fedList_.empty()){ 
               if(std::find(fedList_.begin(),fedList_.end(),hitFED)==fedList_.end()) fedList_.push_back(hitFED);
          }
          else fedList_.push_back(hitFED);
       }
      else{
             hitFED = frameReverter_->findFedId(trackHit.rawId());
             if(debug_) std::cout<<"[selectedElectronFEDListProducer] electron pixel hit "<<trackHit.rawId()<<" subdetector "<<trackHit.subdetId()<<" hitFED "<<hitFED<<std::endl;
             if(!fedList_.empty()){ 
               if(std::find(fedList_.begin(),fedList_.end(),hitFED)==fedList_.end()) fedList_.push_back(hitFED);
             }
             else fedList_.push_back(hitFED); 
       }
     }
  } // end loop on the electron
 } // end loop on the electron collection

 if(debug_){
  if(!fedList_.empty()){ 
    std::cout<<"[selectedElectronFEDListProducer] fed point ";
    for( unsigned int i =0; i< fedList_.size(); i++) 
      std::cout<<fedList_.at(i)<<"  ";
  }
  std::cout<<"  "<<std::endl;
 }

 // make the final raw data collection
 RawDataCollection_ = new FEDRawDataCollection();
 std::vector<uint32_t>::const_iterator itfedList = fedList_.begin();
 for( ; itfedList!=fedList_.end() ; ++itfedList){
   const FEDRawData& data = rawdata->FEDData(*itfedList);
   if(data.size()>0){
           FEDRawData& fedData = RawDataCollection_->FEDData(*itfedList);
           fedData.resize(data.size());
           memcpy(fedData.data(),data.data(),data.size());
    } 
  } 

  std::auto_ptr<FEDRawDataCollection> streamFEDRawProduct(RawDataCollection_);
  iEvent.put(streamFEDRawProduct,outputLabelModule_);
  eventCounter_ ++ ;
}

void selectedElectronFEDListProducer::endJob(){

 if(debug_){ std::cout<<"[selectedElectronFEDListProducer] Counted Events "<<eventCounter_<<std::endl;
             std::cout<<"[selectedElectronFEDListProducer] End of the Job ----> "<<std::endl;
 }

}


DEFINE_FWK_MODULE(selectedElectronFEDListProducer);