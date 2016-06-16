//
// light-weight reconstructed jet
//

#include "UserCode/diall/interface/lwJet.h"

ClassImp(lwJet)

//__________________________________________________________
lwJet::lwJet() :
particleBase(),
  fArea(-1),
  fConstIds(),
  fRefParton(-1),
  fRefPartonForB(-1),
  fCsvSimpleDiscr(-1),
  fSubJetPt(0),
  fSubJetEta(0),
  fSubJetPhi(0),
  fSubJetM(0),
  fNDropped(-1),
  fRawPt(-1),
  fRawM(-1),
  fRefPt(-1),
  fRefEta(-999),
  fRefPhi(-999),
  fRefM(-1),
  fRefPtG(-1),
  fRefEtaG(-999),
  fRefPhiG(-999),
  fRefMG(-1),
  fRefDr(-1),
  fRefSubJetPt(0),
  fRefSubJetEta(0),
  fRefSubJetPhi(0),
  fRefSubJetM(0),
  fRefNDropped(-1),
  fSube(-1),
  fChargedMax(-1.),
  fChargedSum(-1.),
  fChargedN(-1),
  fChargedHardMax(-1.),
  fChargedHardSum(-1.),
  fChargedHardN(-1),
  fPhotonMax(-1.),
  fPhotonSum(-1.),
  fPhotonN(-1),
  fNeutralMax(-1.),
  fNeutralSum(-1.),
  fNeutralN(-1),
  fEmMax(-1.),
  fEmSum(-1.),
  fEmN(-1),
  fMuMax(-1.),
  fMuSum(-1.),
  fMuN(-1)
{
  //default constructor
  for(int i = 0; i<3; ++i) {
    fTau[i] = -1.;
    fRefTau[i] = -1.;
  }
}

//__________________________________________________________
lwJet::lwJet(Double_t pt, Double_t eta, Double_t phi, Double_t m, Int_t id, Int_t c) :
  particleBase(pt,eta,phi,m,id,c),
  fArea(-1),
  fConstIds(),
  fRefParton(-1),
  fRefPartonForB(-1),
  fCsvSimpleDiscr(-1),
  fSubJetPt(0),
  fSubJetEta(0),
  fSubJetPhi(0),
  fSubJetM(0),
  fNDropped(-1),
  fRawPt(-1),
  fRawM(-1),
  fRefPt(-1),
  fRefEta(-999),
  fRefPhi(-999),
  fRefM(-1),
  fRefPtG(-1),
  fRefEtaG(-999),
  fRefPhiG(-999),
  fRefMG(-1),
  fRefDr(-1),
  fRefSubJetPt(0),
  fRefSubJetEta(0),
  fRefSubJetPhi(0),
  fRefSubJetM(0),
  fRefNDropped(-1),
  fSube(-1),
  fChargedMax(-1.),
  fChargedSum(-1.),
  fChargedN(-1),
  fChargedHardMax(-1.),
  fChargedHardSum(-1.),
  fChargedHardN(-1),
  fPhotonMax(-1.),
  fPhotonSum(-1.),
  fPhotonN(-1),
  fNeutralMax(-1.),
  fNeutralSum(-1.),
  fNeutralN(-1),
  fEmMax(-1.),
  fEmSum(-1.),
  fEmN(-1),
  fMuMax(-1.),
  fMuSum(-1.),
  fMuN(-1)
{
  //standard constructor
  for(int i = 0; i<3; ++i) {
    fTau[i] = -1.;
    fRefTau[i] = -1.;
  }
}

//__________________________________________________________
lwJet::lwJet(const lwJet &obj) :
  particleBase(obj),
  fArea(obj.fArea),
  fConstIds(obj.fConstIds),
  fRefParton(obj.fRefParton),
  fRefPartonForB(obj.fRefPartonForB),
  fCsvSimpleDiscr(obj.fCsvSimpleDiscr),
  fSubJetPt(obj.fSubJetPt),
  fSubJetEta(obj.fSubJetEta),
  fSubJetPhi(obj.fSubJetPhi),
  fSubJetM(obj.fSubJetM),
  fNDropped(obj.fNDropped),
  fRawPt(obj.fRawPt),
  fRawM(obj.fRawM),
  fRefPt(obj.fRefPt),
  fRefEta(obj.fRefEta),
  fRefPhi(obj.fRefPhi),
  fRefM(obj.fRefM),
  fRefPtG(obj.fRefPtG),
  fRefEtaG(obj.fRefEtaG),
  fRefPhiG(obj.fRefPhiG),
  fRefMG(obj.fRefMG),
  fRefDr(obj.fRefDr),
  fRefSubJetPt(obj.fRefSubJetPt),
  fRefSubJetEta(obj.fRefSubJetEta),
  fRefSubJetPhi(obj.fRefSubJetPhi),
  fRefSubJetM(obj.fRefSubJetM),
  fRefNDropped(obj.fRefNDropped),
  fSube(obj.fSube),
  fChargedMax(obj.fChargedMax),
  fChargedSum(obj.fChargedSum),
  fChargedN(obj.fChargedN),
  fChargedHardMax(obj.fChargedHardMax),
  fChargedHardSum(obj.fChargedHardSum),
  fChargedHardN(obj.fChargedHardN),
  fPhotonMax(obj.fPhotonMax),
  fPhotonSum(obj.fPhotonSum),
  fPhotonN(obj.fPhotonN),
  fNeutralMax(obj.fNeutralMax),
  fNeutralSum(obj.fNeutralSum),
  fNeutralN(obj.fNeutralN),
  fEmMax(obj.fEmMax),
  fEmSum(obj.fEmSum),
  fEmN(obj.fEmN),
  fMuMax(obj.fMuMax),
  fMuSum(obj.fMuSum),
  fMuN(obj.fMuN)
{
  //copy constructor
  for(int i = 0; i<3; ++i) {
    fTau[i] = obj.fTau[i];
    fRefTau[i] = obj.fRefTau[i];
  }
}

//__________________________________________________________
lwJet &lwJet::operator=(const lwJet &other)
{
  //assignment operator

  if(this!=&other) {
    lwJet::operator=(other);
    fArea           = other.fArea;
    fConstIds       = other.fConstIds;
    fRefParton      = other.fRefParton;
    fRefPartonForB  = other.fRefPartonForB;
    fCsvSimpleDiscr = other.fCsvSimpleDiscr;
    fSubJetPt = other.fSubJetPt;
    fSubJetEta = other.fSubJetEta;
    fSubJetPhi = other.fSubJetPhi;
    fSubJetM = other.fSubJetM;
    fNDropped = other.fNDropped;
    fRawPt          = other.fRawPt;
    fRawM           = other.fRawM;
    fRefPt          = other.fRefPt;
    fRefEta         = other.fRefEta;
    fRefPhi         = other.fRefPhi;
    fRefM           = other.fRefM;
    fRefPtG         = other.fRefPtG;
    fRefEtaG        = other.fRefEtaG;
    fRefPhiG        = other.fRefPhiG;
    fRefMG          = other.fRefMG;
    fRefDr          = other.fRefDr;
    fRefSubJetPt = other.fRefSubJetPt;
    fRefSubJetEta = other.fRefSubJetEta;
    fRefSubJetPhi = other.fRefSubJetPhi;
    fRefSubJetM = other.fRefSubJetM;
    fRefNDropped = other.fRefNDropped;
    fSube           = other.fSube;
    fChargedMax = other.fChargedMax;
    fChargedSum = other.fChargedSum;
    fChargedN = other.fChargedN;
    fChargedHardMax = other.fChargedHardMax;
    fChargedHardSum = other.fChargedHardSum;
    fChargedHardN = other.fChargedHardN;
    fPhotonMax = other.fPhotonMax;
    fPhotonSum = other.fPhotonSum;
    fPhotonN = other.fPhotonN;
    fNeutralMax = other.fNeutralMax;
    fNeutralSum = other.fNeutralSum;
    fNeutralN = other.fNeutralN;
    fEmMax = other.fEmMax;
    fEmSum = other.fEmSum;
    fEmN = other.fEmN;
    fMuMax = other.fMuMax;
    fMuSum = other.fMuSum;
    fMuN = other.fMuN;
  }
  for(int i = 0; i<3; ++i) {
    fTau[i] = other.fTau[i];
    fRefTau[i] = other.fRefTau[i];
  }
  return *this;
}

//__________________________________________________________
void lwJet::SetChargedProp(float max, float sum, int n) {
  fChargedMax = max;
  fChargedSum = sum;
  fChargedN = n;
}

//__________________________________________________________
void lwJet::SetChargedHardProp(float max, float sum, int n) {
  fChargedHardMax = max;
  fChargedHardSum = sum;
  fChargedHardN = n;
}

//__________________________________________________________
void lwJet::SetPhotonProp(float max, float sum, int n) {
  fPhotonMax = max;
  fPhotonSum = sum;
  fPhotonN = n;
}

//__________________________________________________________
void lwJet::SetNeutralProp(float max, float sum, int n) {
  fNeutralMax = max;
  fNeutralSum = sum;
  fNeutralN = n;
}

//__________________________________________________________
void lwJet::SetEmProp(float max, float sum, int n) {
  fEmMax = max;
  fEmSum = sum;
  fEmN = n;
}

//__________________________________________________________
void lwJet::SetMuProp(float max, float sum, int n) {
  fMuMax = max;
  fMuSum = sum;
  fMuN = n;
}

