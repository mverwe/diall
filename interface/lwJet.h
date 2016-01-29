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
  float       GetRawPt()                const { return fRawPt          ; }
  float       GetRawM()                 const { return fRawM           ; }
  float       GetRefPt()                const { return fRefPt          ; }
  float       GetRefEta()               const { return fRefEta         ; }
  float       GetRefM()                 const { return fRefM           ; }
  float       GetRefDr()                const { return fRefDr          ; }
  int         GetSubEvent()             const { return fSube           ; }  
  
  Int_t       GetNConstituents()        const { return fConstIds.size(); }
  Int_t       GetConstituentId(Int_t i) const { return fConstIds[i]    ; }
  particleBase *GetConstituent(Int_t i, TClonesArray *c) const { if(!c) return 0; return dynamic_cast<particleBase*>(c->At(fConstIds[i])); }

  Int_t       GetRefParton()            const { return fRefParton      ; }
  Int_t       GetRefPartonForB()        const { return fRefPartonForB  ; }
  Float_t     GetCsvSimpleDiscr()       const { return fCsvSimpleDiscr ; }
  float       GetChargedSum()           const { return fChargedSum; }
  float       GetChargedMax()           const { return fChargedMax; }
  int         GetChargedN()             const { return fChargedN;   }
  float       GetChargedHardSum()       const { return fChargedHardSum; }
  float       GetChargedHardMax()       const { return fChargedHardMax; }
  int         GetChargedHardN()         const { return fChargedHardN;   }
  float       GetPhotonSum()            const { return fPhotonSum; }
  float       GetPhotonMax()            const { return fPhotonMax; }
  int         GetPhotonN()              const { return fPhotonN;   }
  float       GetNeutralSum()           const { return fNeutralSum; }
  float       GetNeutralMax()           const { return fNeutralMax; }
  int         GetNeutralN()             const { return fNeutralN;   }
  float       GetEmSum()                const { return fEmSum; }
  float       GetEmMax()                const { return fEmMax; }
  int         GetEmN()                  const { return fEmN; }
  float       GetMuSum()                const { return fMuSum; }
  float       GetMuMax()                const { return fMuMax; }
  int         GetMuN()                  const { return fMuN; }
  
  void        SetArea(Double_t a)          { fArea = a; }
  void        SetRawPt(Double_t pt)        { fRawPt = pt; }
  void        SetRawM(Double_t m)          { fRawM = m; }
  void        SetRefPt(Double_t pt)        { fRefPt = pt; }
  void        SetRefEta(Double_t eta)      { fRefEta = eta; }
  void        SetRefM(Double_t m)          { fRefM = m; }
  void        SetRefDr(Double_t dr)        { fRefDr = dr; }
  void        SetSubEvent(int su)          { fSube  = su; }

  void        AddConstituent(Int_t i )     { fConstIds.push_back(i); }
  void        SetRefToParton(Int_t i)      { fRefParton = i;}
  void        SetRefToPartonForB(Int_t i)  { fRefPartonForB = i; }
  void        SetCsvSimpleDiscr(Float_t d) { fCsvSimpleDiscr = d;}

  void        SetChargedProp(float max, float sum, int n);
  void        SetChargedHardProp(float max, float sum, int n);
  void        SetPhotonProp(float max, float sum, int n);
  void        SetNeutralProp(float max, float sum, int n);
  void        SetEmProp(float max, float sum, int n);
  void        SetMuProp(float max, float sum, int n);
  
 protected:
  Double_t          fArea;          //jet area
  std::vector<int>  fConstIds;      //ids of constituents
  Int_t             fRefParton;     //ref to parton
  Int_t             fRefPartonForB; //ref to parton for b
  Float_t           fCsvSimpleDiscr;//csv simple b-jet discriminator
  float             fRawPt;         //raw pt (no jec)
  float             fRawM;          //raw mass (no jec)
  float             fRefPt;         //matched gen-level jet pt
  float             fRefEta;        //matched gen-level jet eta
  float             fRefM;          //matched gen-level jet mass
  float             fRefDr;         //distance between reco and gen jet
  int               fSube;          //sub-event id (0=hard process)
  float             fChargedMax;    //max track pt
  float             fChargedSum;    //summed track pt
  int               fChargedN;      //n tracks
  float             fChargedHardMax;//max track pt with track pt>4
  float             fChargedHardSum;//summed track pt of tracks with pt>4
  int               fChargedHardN;  //n tracks with pt>4
  float             fPhotonMax;     //max photon e
  float             fPhotonSum;
  int               fPhotonN;
  float             fNeutralMax;
  float             fNeutralSum;
  int               fNeutralN;
  float             fEmMax;
  float             fEmSum;
  int               fEmN;
  float             fMuMax;
  float             fMuSum;
  int               fMuN;
    
  ClassDef(lwJet,5)
};
#endif
