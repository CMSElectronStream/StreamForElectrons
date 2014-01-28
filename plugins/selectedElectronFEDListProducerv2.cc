#include "HLTrigger/Egamma/interface/selectedElectronFEDListProducerv2.h"

selectedElectronFEDListProducerv2::selectedElectronFEDListProducerv2(const edm::ParameterSet & iConfig){
 
 // input electron collection
 if(iConfig.existsAs<std::vector<edm::InputTag> >("electronCollections")){
   electronCollections_ = iConfig.getParameter<std::vector<edm::InputTag>>("electronCollections");
   if(electronCollections_.empty()){
      throw cms::Exception("Configuration")<<"[selectedElectronFEDListProducer] empty electron collection is given --> at least one \n"; 
   }
 }
 else{ throw cms::Exception("Configuration")<<"[selectedElectronFEDListProducer] no electron collection are given --> need at least one \n";  }

 // list of gsf collections
 if(iConfig.existsAs<std::vector<int>>("isGsfElectronCollection")){
    isGsfElectronCollection_ = iConfig.getParameter<std::vector<int>>("isGsfElectronCollection");
    if(isGsfElectronCollection_.empty()){
      throw cms::Exception("Configuration")<<"[selectedElectronFEDListProducer] empty electron flag collection --> at least one \n"; 
   }
 }
 else{ throw cms::Exception("Configuration")<<"[selectedElectronFEDListProducer] no electron flag are given --> need at least one \n";  }

 if(isGsfElectronCollection_.size() < electronCollections_.size()) throw cms::Exception("Configuration")<<"[selectedElectronFEDListProducer] electron flag < electron collection  --> need at equal number to understand which are Gsf and which not \n";

 if(iConfig.existsAs<edm::InputTag>("beamSpot")){
   beamSpotTag_ = iConfig.getParameter<edm::InputTag>("beamSpot");
 }
 else{ beamSpotTag_ = edm::InputTag("hltOnlineBeamSpot"); }
   
 // ES look up table
 if(iConfig.existsAs<edm::FileInPath>("ESLookupTable")){
   ESLookupTable_ = iConfig.getParameter<edm::FileInPath>("ESLookupTable");
 }
 else{ ESLookupTable_ = edm::FileInPath("EventFilter/ESDigiToRaw/data/ES_lookup_table.dat"); }

 // ES raw data collector label
 if(iConfig.existsAs<edm::InputTag>("rawDataLabel")){
   rawDataLabel_ = iConfig.getParameter<edm::InputTag>("rawDataLabel");
 }
 else rawDataLabel_ = edm::InputTag("rawDataCollector") ;

 if(iConfig.existsAs<std::string>("outputLabelModule")){
   outputLabelModule_ = iConfig.getParameter<std::string>("outputLabelModule");
 }
 else outputLabelModule_ = "streamElectronRawData" ;

 // dR for the strip region
 if(iConfig.existsAs<double>("dRStripRegion")){
   dRStripRegion_ = iConfig.getParameter<double>("dRStripRegion");
 }
 else dRStripRegion_ = 0.5 ;

 // dPhi, dEta and maxZ for pixel dump
 if(iConfig.existsAs<double>("dPhiPixelRegion")){
   dPhiPixelRegion_ = iConfig.getParameter<double>("dPhiPixelRegion");
 }
 else dPhiPixelRegion_ = 0.5 ;

 if(iConfig.existsAs<double>("dEtaPixelRegion")){
   dEtaPixelRegion_ = iConfig.getParameter<double>("dEtaPixelRegion");
 }
 else dEtaPixelRegion_ = 0.5 ;

 if(iConfig.existsAs<double>("maxZPixelRegion")){
   maxZPixelRegion_ = iConfig.getParameter<double>("maxZPixelRegion");
 }
 else maxZPixelRegion_ = 24. ;

 // bool
 if( iConfig.existsAs<bool>("dumpEcalFedList")){
   dumpEcalFedList_ = iConfig.getParameter< bool >("dumpEcalFedList");
 }
 else dumpEcalFedList_ = true ;

 if(iConfig.existsAs<bool>("dumpTrackSiStripFedList")){
   dumpTrackSiStripFedList_ = iConfig.getParameter<bool>("dumpTrackSiStripFedList");
 }
 else dumpTrackSiStripFedList_ = true ;

 if(iConfig.existsAs<bool>("dumpTrackSiPixelFedList")){
    dumpTrackSiPixelFedList_ = iConfig.getParameter<bool>("dumpTrackSiPixelFedList");
 }
 else dumpTrackSiPixelFedList_ = true ;

 if(iConfig.existsAs<bool>("dumpAllEcalFed")){
     dumpAllEcalFed_ = iConfig.getParameter<bool>("dumpAllEcalFed");
 }
 else dumpAllEcalFed_ = false ;

 if(iConfig.existsAs<bool>("dumpAllTrackerFed")){
     dumpAllTrackerFed_ = iConfig.getParameter<bool>("dumpAllTrackerFed");
 }
 else dumpAllTrackerFed_ = false ;

 if(iConfig.existsAs<bool>("debug")){
   debug_ = iConfig.getParameter<bool>("debug");
 }
 else debug_ = false ;

 if(debug_){

  std::cout<<"[selectedElectronFEDListProducer] output Label "<<outputLabelModule_<<std::endl; 

  std::cout<<"[selectedElectronFEDListProducer] beam spot Tag "<<beamSpotTag_<<std::endl;

  std::cout<<"[selectedElectronFEDListProducer] dumpEcalFedList set to "<<dumpEcalFedList_<<" dumpTrackSiStripFedList "<<dumpTrackSiStripFedList_<<" dumpTrackSiPixelFedList "<<dumpTrackSiPixelFedList_<<std::endl;

  std::cout<<"[selectedElectronFEDListProducer] dumpAllEcalFed "<<dumpAllEcalFed_<<" dumpAllTrackerFed "<<dumpAllTrackerFed_<<std::endl;

  std::cout<<"[selectedElectronFEDListProducer] dRStripRegion "<<dRStripRegion_<<std::endl;

  std::cout<<"[selectedElectronFEDListProducer] dPhiPixelRegion "<<dPhiPixelRegion_<<" dEtaPixelRegion "<<dEtaPixelRegion_<<" maxZPixelRegion "<<maxZPixelRegion_<<std::endl;

  std::cout<<"[selectedElectronFEDListProducer] Electron Collections"<<std::endl;

  std::vector<edm::InputTag>::const_iterator Tag = electronCollections_.begin();
  std::vector<int>::const_iterator Flag = isGsfElectronCollection_.begin();
  for( ; Tag !=electronCollections_.end() && Flag!=isGsfElectronCollection_.end() ; ++Tag , ++Flag)
     std::cout<<"[selectedElectronFEDListProducer] ele collection: "<<*(Tag)<<" isGsf "<<*(Flag)<<std::endl;
     
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

selectedElectronFEDListProducerv2::~selectedElectronFEDListProducerv2(){

 if(!electronCollections_.empty()) electronCollections_.clear() ;
 if(!fedList_.empty()) fedList_.clear() ;
 if(!RawDataCollection_) delete RawDataCollection_ ;

}

void selectedElectronFEDListProducerv2::beginJob(){ 
 
  if(debug_){ std::cout<<"[selectedElectronFEDListProducer] Begin of the Job ----> "<<std::endl;
              std::cout<<"[selectedElectronFEDListProducer] event counter set to "<<eventCounter_<<std::endl;
  }
  eventCounter_ = 0 ;
 
} 

void selectedElectronFEDListProducerv2::produce(edm::Event & iEvent, const edm::EventSetup & iSetup){

  if(!fedList_.empty()) fedList_.clear(); 
  if(!RawDataCollection_) delete RawDataCollection_ ;

  // Build FED strip map --> just one time
  // Retrieve FED ids from cabling map and iterate through 
  if(eventCounter_ ==0 ){

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

   // pixel tracker cabling map
   edm::ESHandle<SiPixelFedCablingMap> pixelCablingMap;
   iSetup.get<SiPixelFedCablingMapRcd>().get(pixelCablingMap);
   PixelCabling_.reset((SiPixelFedCabling*)pixelCablingMap->cablingTree());

   edm::ESHandle<TrackerGeometry> trackerGeometry;
   iSetup.get<TrackerDigiGeometryRecord>().get( trackerGeometry );

   edm::ESHandle<SiStripRegionCabling> SiStripCabling ;
   iSetup.get<SiStripRegionCablingRcd>().get(SiStripCabling);
   StripRegionCabling_ = SiStripCabling.product();

   cabling_ = StripRegionCabling_->getRegionCabling();
   regionDimension_ = StripRegionCabling_->regionDimensions();

   // build the tracker pixel module map
   std::vector<GeomDet*>::const_iterator itTracker = trackerGeometry->dets().begin();   
   for( ; itTracker !=trackerGeometry->dets().end() ; ++itTracker){
       int subdet = (*itTracker)->geographicalId().subdetId();
       if(! (subdet == PixelSubdetector::PixelBarrel || subdet == PixelSubdetector::PixelEndcap) ) continue;
 
       PixelModule module ;
       module.x = (*itTracker)->position().x();
       module.y = (*itTracker)->position().y();
       module.z = (*itTracker)->position().z();
       module.Phi = normalizedPhi((*itTracker)->position().phi()) ; 
       module.Eta = (*itTracker)->position().eta() ;
       module.DetId  = (*itTracker)->geographicalId().rawId();
       const std::vector<sipixelobjects::CablingPathToDetUnit> path2det = PixelCabling_->pathToDetUnit(module.DetId);
       module.Fed = path2det[0].fed;
       assert(module.Fed<40);
       pixelModuleVector_.push_back(module);
   }

   std::sort(pixelModuleVector_.begin(),pixelModuleVector_.end());
 
  }

  // get event raw data
  edm::Handle<FEDRawDataCollection> rawdata;
  iEvent.getByLabel(rawDataLabel_,rawdata);

  // loop on the input electron collection vector
  edm::Handle<trigger::TriggerFilterObjectWithRefs> triggerElectronCollection;
  std::vector< edm::Ref<reco::ElectronCollection> > electrons;
  reco::ElectronRef electron ;

  // take the beam spot position
  edm::Handle<reco::BeamSpot> beamSpot;
  iEvent.getByLabel(beamSpotTag_, beamSpot);
  beamSpotPosition_ = beamSpot->position();

  std::vector<edm::InputTag>::const_iterator itTag = electronCollections_.begin();
  std::vector<int>::const_iterator itFlag = isGsfElectronCollection_.begin();

  for( ; itTag!=electronCollections_.end() && itFlag!=isGsfElectronCollection_.end(); ++itTag , ++itFlag){

   try { iEvent.getByLabel (*itTag,triggerElectronCollection);
         if(triggerElectronCollection.failedToGet()) continue ;
   }
   catch (cms::Exception &exception) {continue;}

  triggerElectronCollection->getObjects(trigger::TriggerElectron, electrons);

  std::vector<edm::Ref<reco::ElectronCollection > >::const_iterator itEle = electrons.begin();

  double radTodeg = 180. / Geom::pi();
  
  if(dumpAllEcalFed_){
   for(uint32_t iEcalFed = FEDNumbering::MINECALFEDID ; iEcalFed <= FEDNumbering::MAXECALFEDID ; iEcalFed++)
     fedList_.push_back(iEcalFed);
   for(uint32_t iESFed = FEDNumbering::MINPreShowerFEDID ; iESFed <= FEDNumbering::MAXPreShowerFEDID ; iESFed++)
     fedList_.push_back(iESFed);
  }

  if(dumpAllTrackerFed_){
   for(uint32_t iPixelFed = FEDNumbering::MINSiPixelFEDID; iPixelFed <= FEDNumbering::FEDNumbering::MAXSiPixelFEDID ; iPixelFed++)
     fedList_.push_back(iPixelFed);
   for(uint32_t iStripFed = FEDNumbering::MINSiStripFEDID; iStripFed <= FEDNumbering::FEDNumbering::MAXSiStripFEDID ; iStripFed++)
     fedList_.push_back(iStripFed);
  }

  if( !dumpAllTrackerFed_  || !dumpAllEcalFed_){
    for( ; itEle!=electrons.end() ; ++itEle){

     // get electron supercluster and the associated hit -> detID
     electron = (*itEle);
     reco::SuperClusterRef scRef = electron->superCluster();
     const std::vector<std::pair<DetId,float> >& hits = scRef->hitsAndFractions();
     // start in dump the ecal FED associated to the electron
     std::vector<std::pair<DetId,float> >::const_iterator itSChits = hits.begin();
     int hitFED = 0 ;
     if(!dumpAllEcalFed_){
      for( ; itSChits!=hits.end() ; ++itSChits){
       if((*itSChits).first.subdetId()== EcalBarrel){

         EBDetId idEBRaw ((*itSChits).first);
         GlobalPoint point = geometry_->getPosition(idEBRaw);
         hitFED = FEDNumbering::MINECALFEDID + TheMapping_->GetFED(double(point.eta()),double(point.phi())*radTodeg);
         if(hitFED <= FEDNumbering::MINECALFEDID || hitFED >= FEDNumbering::MAXECALFEDID) continue;

         if(debug_) std::cout<<"[selectedElectronFEDListProducer] electron hit detID Barrel "<<(*itSChits).first.rawId()<<" eta "<<double(point.eta())<<" phi "<< double(point.phi())*radTodeg <<" FED "<<hitFED<<std::endl;
         if(dumpEcalFedList_){
          if(!fedList_.empty()){ 
            if(std::find(fedList_.begin(),fedList_.end(),hitFED)==fedList_.end()) fedList_.push_back(hitFED);
          }
          else fedList_.push_back(hitFED);
         }
      } 
      else if((*itSChits).first.subdetId()== EcalEndcap){
         EEDetId idEERaw ((*itSChits).first);
         GlobalPoint point = geometry_->getPosition(idEERaw);
         hitFED = FEDNumbering::MINECALFEDID + TheMapping_->GetFED(double(point.eta()),double(point.phi())*radTodeg);
         if(hitFED <= FEDNumbering::MINECALFEDID || hitFED >= FEDNumbering::MAXECALFEDID) continue;

         if(debug_) std::cout<<"[selectedElectronFEDListProducer] electron hit detID Endcap "<<(*itSChits).first.rawId()<<" eta "<<double(point.eta())<<" phi "<<double(point.phi())*radTodeg <<" FED "<<hitFED<<std::endl;
         if(dumpEcalFedList_){
          if(!fedList_.empty()){ 
            if(std::find(fedList_.begin(),fedList_.end(),hitFED)==fedList_.end()) fedList_.push_back(hitFED);
          }
          else fedList_.push_back(hitFED);
          // preshower hit for each ecal endcap hit
          DetId tmpX = (dynamic_cast<const EcalPreshowerGeometry*>(geometry__ES_))->getClosestCellInPlane(point,1);
          ESDetId stripX = (tmpX == DetId(0)) ? ESDetId(0) : ESDetId(tmpX);          
          hitFED = ES_fedId_[(3-stripX.zside())/2-1][stripX.plane()-1][stripX.six()-1][stripX.siy()-1];
          if(debug_) std::cout<<"[selectedElectronFEDListProducer] ES hit plane X (deiID) "<<stripX.rawId()<<" six "<<stripX.six()<<" siy "<<stripX.siy()<<" plane "<<stripX.plane()<<" FED ID "<<hitFED<<std::endl;
          if(hitFED <= FEDNumbering::MINPreShowerFEDID || hitFED >= FEDNumbering::MAXPreShowerFEDID) continue;
          if(hitFED < 0) continue;
          if(!fedList_.empty()){ 
            if(std::find(fedList_.begin(),fedList_.end(),hitFED)==fedList_.end()) fedList_.push_back(hitFED);
           }
           else fedList_.push_back(hitFED);
         
          DetId tmpY = (dynamic_cast<const EcalPreshowerGeometry*>(geometry__ES_))->getClosestCellInPlane(point,2);
          ESDetId stripY = (tmpY == DetId(0)) ? ESDetId(0) : ESDetId(tmpY);          
          hitFED = ES_fedId_[(3-stripY.zside())/2-1][stripY.plane()-1][stripY.six()-1][stripY.siy()-1];
          if(hitFED <= FEDNumbering::MINPreShowerFEDID || hitFED >= FEDNumbering::MAXPreShowerFEDID) continue;
          if(debug_) std::cout<<"[selectedElectronFEDListProducer] ES hit plane Y (deiID) "<<stripY.rawId()<<" six "<<stripY.six()<<" siy "<<stripY.siy()<<" plane "<<stripY.plane()<<" FED ID "<<hitFED<<std::endl;
          if(hitFED < 0) continue;
          if(!fedList_.empty()){ 
            if(std::find(fedList_.begin(),fedList_.end(),hitFED)==fedList_.end()) fedList_.push_back(hitFED);
          }
          else fedList_.push_back(hitFED);      
         }    
        }   
      } // end loop on SC hit   
    }
    // get the electron track
    if( !dumpAllTrackerFed_ ){ 
       //loop on the region
       if(dumpTrackSiStripFedList_){
          double eta ;
          double phi ;
          if(*itFlag){
           eta = electron->gsfTrack()->eta();
           phi = electron->gsfTrack()->phi();
          }
          else{
           eta = electron->track()->eta();
           phi = electron->track()->phi();
          }
 	  for(uint32_t iCabling = 0; iCabling < cabling_.size(); iCabling++){
	    SiStripRegionCabling::Position pos = StripRegionCabling_->position(iCabling);
	    double dphi=fabs(pos.second-phi);
	    if (dphi>acos(-1)) dphi=2*acos(-1)-dphi;
	    double R = sqrt(pow(pos.first-eta,2)+dphi*dphi);
	    if (R-sqrt(pow(regionDimension_.first/2,2)+pow(regionDimension_.second/2,2))>dRStripRegion_) continue;
	    //get vector of subdets within region
	    const SiStripRegionCabling::RegionCabling regSubdets= cabling_[iCabling];
	    //cycle on subdets
	    for (uint32_t idet=0; idet<SiStripRegionCabling::ALLSUBDETS; idet++){ //cicle between 1 and 4 
	      //get vector of layers whin subdet of region
	      const SiStripRegionCabling::WedgeCabling regSubdetLayers=regSubdets[idet]; // at most 10 layers        
	      for (uint32_t ilayer=0; ilayer<SiStripRegionCabling::ALLLAYERS; ilayer++){
		//get map of vectors of feds withing the layer of subdet of region
		const SiStripRegionCabling::ElementCabling fedVectorMap = regSubdetLayers[ilayer]; // vector of the fed
		SiStripRegionCabling::ElementCabling::const_iterator itFedMap = fedVectorMap.begin();
		 for( ; itFedMap!=fedVectorMap.end(); itFedMap++){
		   for (uint32_t op=0; op<(itFedMap->second).size(); op++){
		     hitFED = (itFedMap->second)[op].fedId(); 
                     if(hitFED <= FEDNumbering::MINSiStripFEDID || hitFED >= FEDNumbering::MAXSiStripFEDID) continue;
                     if(debug_) std::cout<<"[selectedElectronFEDListProducer] SiStrip (FedID) "<<hitFED<<std::endl;
                     if(!fedList_.empty()){ 
                       if(std::find(fedList_.begin(),fedList_.end(),hitFED)==fedList_.end()) fedList_.push_back(hitFED);
                     }
                     else fedList_.push_back(hitFED);
                   }
                 }
	       }
	     }
	   }
	}
      
        if(dumpTrackSiPixelFedList_){

           math::XYZVector momentum;
           if(*itFlag) momentum = electron->gsfTrack()->momentum();
           else momentum = electron->track()->momentum();
           PixelRegion region (momentum,dPhiPixelRegion_,dEtaPixelRegion_,maxZPixelRegion_);

           PixelModule lowerBound (normalizedPhi(region.vector.phi())-region.dPhi, region.vector.eta()-region.dEta);
           PixelModule upperBound (normalizedPhi(region.vector.phi())+region.dPhi, region.vector.eta()+region.dEta);

           std::vector<PixelModule>::const_iterator itUp, itDn ;
           if(lowerBound.Phi >= -M_PI  && upperBound.Phi <= M_PI ){
            itDn = std::lower_bound(pixelModuleVector_.begin(),pixelModuleVector_.end(),lowerBound);
            itUp = std::upper_bound(pixelModuleVector_.begin(),pixelModuleVector_.end(),upperBound);
            pixelFedDump(itDn,itUp,region);
           }
           else{
                if(lowerBound.Phi < -M_PI) lowerBound.Phi = lowerBound.Phi+2*M_PI;
                PixelModule phi_p(M_PI,region.vector.eta()-region.dEta);
                itDn = std::lower_bound(pixelModuleVector_.begin(),pixelModuleVector_.end(),lowerBound);
                itUp = std::upper_bound(pixelModuleVector_.begin(),pixelModuleVector_.end(),phi_p);
                pixelFedDump(itDn,itUp,region);
                 
                if(upperBound.Phi < -M_PI) upperBound.Phi = upperBound.Phi-2*M_PI;
                PixelModule phi_m(-M_PI,region.vector.eta()-region.dEta);
                itDn = std::lower_bound(pixelModuleVector_.begin(),pixelModuleVector_.end(),phi_m);
                itUp = std::upper_bound(pixelModuleVector_.begin(),pixelModuleVector_.end(),upperBound);
                pixelFedDump(itDn,itUp,region);

          }
        } 
      }
     } // end loop on the electron
   } 
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

void selectedElectronFEDListProducerv2::endJob(){

 if(debug_){ std::cout<<"[selectedElectronFEDListProducer] Counted Events "<<eventCounter_<<std::endl;
             std::cout<<"[selectedElectronFEDListProducer] End of the Job ----> "<<std::endl;
 }

}

void selectedElectronFEDListProducerv2::pixelFedDump( std::vector<PixelModule>::const_iterator & itDn,  
                                                      std::vector<PixelModule>::const_iterator & itUp,
                                                      const PixelRegion & region){

  for( ; itDn != itUp ; ++itDn){
    float zmodule = itDn->z-((itDn->x-beamSpotPosition_.x())*region.cosphi+(itDn->y-
beamSpotPosition_.y())*region.sinphi)*region.atantheta;
    if ( std::abs(zmodule) > region.maxZ ) continue; 
    int hitFED = itDn->Fed;
    if(hitFED <= FEDNumbering::MINSiPixelFEDID || hitFED >= FEDNumbering::MAXSiPixelFEDID) continue;
    if(debug_) std::cout<<"[selectedElectronFEDListProducer] electron pixel hit "<<itDn->DetId<<" hitFED "<<hitFED<<std::endl;
    if(!fedList_.empty()){ 
     if(std::find(fedList_.begin(),fedList_.end(),hitFED)==fedList_.end()) fedList_.push_back(hitFED);
    }
    else fedList_.push_back(hitFED); 
 }

 return ;

}


DEFINE_FWK_MODULE(selectedElectronFEDListProducerv2);
