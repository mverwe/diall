#ifndef lwJet_h
#define lwJet_h

//
// light-weight reconstructed jet
//

#include "TClonesArray.h"

#include "UserCode/diall/interface/particleBase.h"
#include "UserCode/diall/interface/pfParticle.h"

class lwJet : public particleBase {
 public:
  
  lwJet();
  lwJet(Double_t pt, Double_t eta, Double_t phi, Double_t m, Int_t id = -1, Int_t c = 0);
  virtual ~lwJet() {;}
  lwJet(const lwJet& obj); // copy constructor
  lwJet& operator=(const lwJet& other); // assignment

  Double_t    GetArea()                 const { return fArea           ; }
  Float_t       GetRawPt()                const { return fRawPt          ; }
  Float_t       GetRefPt()                const { return fRefPt          ; }
  Float_t       GetRefEta()               const { return fRefEta         ; }
  Float_t       GetRefDr()                const { return fRefDr          ; }
  Int_t         GetSubEvent()             const { return fSube           ; }  
  
  Int_t       GetNConstituents()        const { return fConstIds.size(); }
  Int_t       GetConstituentId(Int_t i) const { return fConstIds[i]    ; }
  pfParticle *GetConstituent(Int_t i, TClonesArray *c) const { if(!c) return 0; return dynamic_cast<pfParticle*>(c->At(fConstIds[i])); }

  Int_t       GetRefParton()            const { return fRefParton      ; }
  Int_t       GetRefPartonForB()        const { return fRefPartonForB  ; }
  Float_t     GetCsvSimpleDiscr()       const { return fCsvSimpleDiscr ; }
  Float_t       GetChargedSum()           const { return fChargedSum; }
  Float_t       GetChargedMax()           const { return fChargedMax; }
  Int_t         GetChargedN()             const { return fChargedN;   }
  Float_t       GetChargedHardSum()       const { return fChargedHardSum; }
  Float_t       GetChargedHardMax()       const { return fChargedHardMax; }
  Int_t         GetChargedHardN()         const { return fChargedHardN;   }
  Float_t       GetPhotonSum()            const { return fPhotonSum; }
  Float_t       GetPhotonMax()            const { return fPhotonMax; }
  Int_t         GetPhotonN()              const { return fPhotonN;   }
  Float_t       GetNeutralSum()           const { return fNeutralSum; }
  Float_t       GetNeutralMax()           const { return fNeutralMax; }
  Int_t         GetNeutralN()             const { return fNeutralN;   }
  Float_t       GetEmSum()                const { return fEmSum; }
  Float_t       GetEmMax()                const { return fEmMax; }
  Int_t         GetEmN()                  const { return fEmN; }
  Float_t       GetMuSum()                const { return fMuSum; }
  Float_t       GetMuMax()                const { return fMuMax; }
  Int_t         GetMuN()                  const { return fMuN; }
  
  void        SetArea(Double_t a)          { fArea = a; }
  void        SetRawPt(Double_t pt)        { fRawPt = pt; }
  void        SetRefPt(Double_t pt)        { fRefPt = pt; }
  void        SetRefEta(Double_t eta)      { fRefEta = eta; }
  void        SetRefDr(Double_t dr)        { fRefDr = dr; }
  void        SetSubEvent(Int_t su)          { fSube  = su; }

  void        AddConstituent(Int_t i )     { fConstIds.push_back(i); }
  void        SetRefToParton(Int_t i)      { fRefParton = i;}
  void        SetRefToPartonForB(Int_t i)  { fRefPartonForB = i; }
  void        SetCsvSimpleDiscr(Float_t d) { fCsvSimpleDiscr = d;}

  void        SetChargedProp(Float_t max, Float_t sum, Int_t n);
  void        SetChargedHardProp(Float_t max, Float_t sum, Int_t n);
  void        SetPhotonProp(Float_t max, Float_t sum, Int_t n);
  void        SetNeutralProp(Float_t max, Float_t sum, Int_t n);
  void        SetEmProp(Float_t max, Float_t sum, Int_t n);
  void        SetMuProp(Float_t max, Float_t sum, Int_t n);
  
 protected:
  Double_t          fArea;          //jet area
  std::vector<Int_t>  fConstIds;      //ids of constituents
  Int_t             fRefParton;     //ref to parton
  Int_t             fRefPartonForB; //ref to parton for b
  Float_t           fCsvSimpleDiscr;//csv simple b-jet discriminator
  Float_t             fRawPt;         //raw pt (no jec)
  Float_t             fRefPt;         //matched gen-level jet pt
  Float_t             fRefEta;        //matched gen-level jet eta
  Float_t             fRefDr;         //distance between reco and gen jet
  Int_t               fSube;          //sub-event id (0=hard process)
  Float_t             fChargedMax;    //max track pt
  Float_t             fChargedSum;    //summed track pt
  Int_t               fChargedN;      //n tracks
  Float_t             fChargedHardMax;//max track pt with track pt>4
  Float_t             fChargedHardSum;//summed track pt of tracks with pt>4
  Int_t               fChargedHardN;  //n tracks with pt>4
  Float_t             fPhotonMax;     //max photon e
  Float_t             fPhotonSum;
  Int_t               fPhotonN;
  Float_t             fNeutralMax;
  Float_t             fNeutralSum;
  Int_t               fNeutralN;
  Float_t             fEmMax;
  Float_t             fEmSum;
  Int_t               fEmN;
  Float_t             fMuMax;
  Float_t             fMuSum;
  Int_t               fMuN;
    
  ClassDef(lwJet,3)
};
#endif
