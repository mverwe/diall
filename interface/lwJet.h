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
  
  
  Int_t       GetNConstituents()        const { return fConstIds.size(); }
  Int_t       GetConstituentId(Int_t i) const { return fConstIds[i]    ; }
  pfParticle *GetConstituent(Int_t i, TClonesArray *c) const { if(!c) return 0; return dynamic_cast<pfParticle*>(c->At(fConstIds[i])); }

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
  float             fRawPt;
  float             fChargedMax;
  float             fChargedSum;
  int               fChargedN;
  float             fChargedHardMax;
  float             fChargedHardSum;
  int               fChargedHardN;
  float             fPhotonMax;
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
    
  ClassDef(lwJet,2)
};
#endif
