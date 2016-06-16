//
// container with heavy-ion event properties
//

#include "UserCode/diall/interface/hiEventContainer.h"

ClassImp(hiEventContainer)

//__________________________________________________________
hiEventContainer::hiEventContainer() :
TNamed("hiEventContainer","hiEventContainer"),
  fRun(-1),
  fEvt(-1),
  fLumi(-1),
  fVx(0.),
  fVy(0.),
  fVz(0.),
  fHiBin(0),
  fCent(0.),
  fNPV(0),
  fPFJet80(0),
  fPhoton30(0),
  fTrk24(0),
  fTrk45(0),
  fColl(0),
  fHBHENoise(0),
  fHBHENoiseLoose(0),
  fPrimaryVertexFilter(0),
  fClusterCompatibilityFilter(0),
  fHFCoincFilter(0),
  fRhoMap(0)
{
  //default constructor
}

//__________________________________________________________
hiEventContainer::hiEventContainer(const char *name) :
  TNamed(name,name),
  fRun(-1),
  fEvt(-1),
  fLumi(-1),
  fVx(0.),
  fVy(0.),
  fVz(0.),
  fHiBin(0),
  fCent(0.),
  fNPV(0),
  fPFJet80(0),
  fPhoton30(0),
  fTrk24(0),
  fTrk45(0),
  fColl(0),
  fHBHENoise(0),
  fHBHENoiseLoose(0),
  fPrimaryVertexFilter(0),
  fClusterCompatibilityFilter(0),
  fHFCoincFilter(0),
  fRhoMap(0)
{
  //standard constructor
}
