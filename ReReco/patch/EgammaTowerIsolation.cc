//*****************************************************************************
// File:      EgammaTowerIsolation.cc
// ----------------------------------------------------------------------------
// OrigAuth:  Matthias Mozer
// Institute: IIHE-VUB
//=============================================================================
//*****************************************************************************

//CMSSW includes
#include "RecoEgamma/EgammaIsolationAlgos/interface/EgammaTowerIsolation.h"
#include<cassert>


// #include<iostream>
namespace etiStat {
  Count::~Count() { 
    //    std::cout << "\nEgammaTowerIsolationNew " << create << "/" << comp << "/" << float(span)/float(comp)  
    //        << std::endl<< std::endl;
    }

  Count Count::count;
}



EgammaTowerIsolationNew<1> *EgammaTowerIsolation::newAlgo=nullptr;
const CaloTowerCollection* EgammaTowerIsolation::oldTowers=nullptr;
uint32_t EgammaTowerIsolation::id15=0;

EgammaTowerIsolation::EgammaTowerIsolation (float extRadiusI,
                        float intRadiusI,
                        float etLow,
                        signed int depth,
                        const CaloTowerCollection* towers ) :  
  depth_(depth), 
  extRadius(extRadiusI),
  intRadius(intRadiusI)
{
  assert(0==etLow);

  // cheating  (test of performance)
  if (newAlgo==nullptr ||  towers!=oldTowers || towers->size()!=newAlgo->nt || (towers->size()>15 && (*towers)[15].id()!=id15)) {
    delete newAlgo;
    newAlgo = new EgammaTowerIsolationNew<1>(&extRadius,&intRadius,*towers);
    oldTowers=towers;
    if (towers->size()>15) id15 = (*towers)[15].id();
  }
}


double  EgammaTowerIsolation::getSum (bool et, reco::SuperCluster const & sc, const std::vector<CaloTowerDetId> * detIdToExclude) const{

  if (0!=detIdToExclude) assert(0==intRadius);

  // hack
  newAlgo->setRadius(&extRadius,&intRadius);

  EgammaTowerIsolationNew<1>::Sum sum;
  newAlgo->compute(et, sum, sc, 
          (detIdToExclude==0) ? nullptr : &((*detIdToExclude).front()),
          (detIdToExclude==0) ? nullptr : (&(*detIdToExclude).back())+1
          );
  
  switch(depth_){
  case AllDepths: return detIdToExclude==0 ? sum.he[0] : sum.heBC[0]; 
  case Depth1: return detIdToExclude==0 ? sum.he[0]-sum.h2[0] : sum.heBC[0]-sum.h2BC[0]; 
  case Depth2:return detIdToExclude==0 ? sum.h2[0] : sum.h2BC[0]; 
  default: return 0;
  }
  return 0;
}

